#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <iostream>
#include <cstring>
#include "audio.h"

bool Audio::initialized = false;
ALCdevice* Audio::device = nullptr;
ALCcontext* Audio::context = nullptr;
std::vector<ALuint> Audio::active_sources;

void Audio::init_openal() {
	if (initialized) {
		// Ensure context is still current
		ALCcontext* current = alcGetCurrentContext();
		if (current != context) {
			alcMakeContextCurrent(context);
		}
		return;
	}
	
	device = alcOpenDevice(nullptr);
	if (!device) {
		std::cerr << "Failed to open OpenAL device" << std::endl;
		return;
	}
	
	context = alcCreateContext(device, nullptr);
	if (!context) {
		std::cerr << "Failed to create OpenAL context" << std::endl;
		alcCloseDevice(device);
		device = nullptr;
		return;
	}
	
	if (!alcMakeContextCurrent(context)) {
		std::cerr << "Failed to make OpenAL context current" << std::endl;
		alcDestroyContext(context);
		alcCloseDevice(device);
		context = nullptr;
		device = nullptr;
		return;
	}
	
	// Set listener position and gain for 2D audio
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alListenerf(AL_GAIN, 1.0f);
	
	// Set listener orientation (forward: -Z, up: +Y)
	ALfloat orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, orientation);
	
	initialized = true;
}

void Audio::cleanup_stopped_sources() {
	if (!initialized || !context) return;
	
	// Ensure context is current
	ALCcontext* current = alcGetCurrentContext();
	if (current != context) {
		if (!alcMakeContextCurrent(context)) {
			return;
		}
	}
	
	// Remove stopped sources
	auto it = active_sources.begin();
	while (it != active_sources.end()) {
		ALint state;
		alGetSourcei(*it, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING && state != AL_PAUSED) {
			alDeleteSources(1, &(*it));
			it = active_sources.erase(it);
		} else {
			++it;
		}
	}
}

Result<Audio> Audio::open(std::string path) {
	init_openal();
	
	if (!initialized || !context || !device) {
		return Result<Audio>(std::string("OpenAL not properly initialized"));
	}
	
	// Ensure context is current
	ALCcontext* current = alcGetCurrentContext();
	if (current != context) {
		if (!alcMakeContextCurrent(context)) {
			return Result<Audio>(std::string("Failed to make OpenAL context current"));
		}
	}
	
	// Clear any previous errors
	alGetError();
	
	Audio audio;
	
	// Open audio file with libsndfile
	SF_INFO sfinfo;
	SNDFILE* file = sf_open(path.c_str(), SFM_READ, &sfinfo);
	if (!file) {
		return Result<Audio>(std::string("Couldn't open audio file at '") + path + "': " + sf_strerror(nullptr));
	}
	
	// Check format
	if (sfinfo.channels < 1 || sfinfo.channels > 2) {
		sf_close(file);
		return Result<Audio>(std::string("Unsupported channel count: ") + std::to_string(sfinfo.channels));
	}
	
	// Read audio data
	std::vector<short> data(sfinfo.frames * sfinfo.channels);
	sf_count_t frames_read = sf_readf_short(file, data.data(), sfinfo.frames);
	sf_close(file);
	
	if (frames_read <= 0) {
		return Result<Audio>(std::string("Failed to read audio data from '") + path + "'");
	}
	
	// Validate samplerate
	if (sfinfo.samplerate <= 0) {
		return Result<Audio>(std::string("Invalid sample rate: ") + std::to_string(sfinfo.samplerate));
	}
	
	// Make sure OpenAL context is current
	if (!alcMakeContextCurrent(context)) {
		return Result<Audio>(std::string("Failed to make OpenAL context current"));
	}
	
	// Clear any previous errors
	alGetError();
	
	// Create OpenAL buffer
	ALuint buffer_id = 0;
	alGenBuffers(1, &buffer_id);
	ALenum error = alGetError();
	if (error != AL_NO_ERROR || buffer_id == 0) {
		if (buffer_id != 0) {
			alDeleteBuffers(1, &buffer_id);
		}
		return Result<Audio>(std::string("Failed to generate OpenAL buffer: ") + std::to_string(error));
	}
	
	// Determine format
	ALenum format;
	if (sfinfo.channels == 1) {
		format = AL_FORMAT_MONO16;
	} else if (sfinfo.channels == 2) {
		format = AL_FORMAT_STEREO16;
	} else {
		alDeleteBuffers(1, &buffer_id);
		return Result<Audio>(std::string("Unsupported channel count: ") + std::to_string(sfinfo.channels));
	}
	
	// Calculate actual data size (only use frames that were actually read)
	size_t data_size = frames_read * sfinfo.channels * sizeof(short);
	
	// Upload buffer data
	alBufferData(buffer_id, format, data.data(), data_size, sfinfo.samplerate);
	
	error = alGetError();
	if (error != AL_NO_ERROR) {
		alDeleteBuffers(1, &buffer_id);
		return Result<Audio>(std::string("Failed to upload buffer data: ") + std::to_string(error) + 
		                     " (size: " + std::to_string(data_size) + 
		                     ", rate: " + std::to_string(sfinfo.samplerate) + ")");
	}
	
	// Create shared_ptr with custom deleter to manage buffer lifetime
	// The deleter will be called when the last Audio object referencing this buffer is destroyed
	// Capture the context to ensure it's available when deleting
	ALCcontext* current_context = context;
	audio.buffer = std::shared_ptr<ALuint>(new ALuint(buffer_id), [current_context](ALuint* buf) {
		if (buf && *buf != 0) {
			// Make sure context is current before deleting
			ALCcontext* prev_ctx = alcGetCurrentContext();
			if (prev_ctx != current_context) {
				alcMakeContextCurrent(current_context);
			}
			alDeleteBuffers(1, buf);
			// Restore previous context if it was different
			if (prev_ctx != current_context && prev_ctx) {
				alcMakeContextCurrent(prev_ctx);
			}
		}
		delete buf;
	});
	
	return Result<Audio>(audio);
}

void Audio::play() {
	ALuint buffer_id = get_buffer_id();
	if (buffer_id == 0) return;
	
	init_openal();
	
	if (!initialized || !context) {
		std::cerr << "OpenAL not initialized" << std::endl;
		return;
	}
	
	// Ensure context is current
	ALCcontext* current = alcGetCurrentContext();
	if (current != context) {
		if (!alcMakeContextCurrent(context)) {
			std::cerr << "Failed to make context current" << std::endl;
			return;
		}
	}
	
	// Clean up stopped sources before creating new ones
	cleanup_stopped_sources();
	
	// Clear any errors
	alGetError();
	
	// Create a new source each time for simplicity (OpenAL can handle many sources)
	// This allows multiple simultaneous plays of the same sound
	ALuint play_source = 0;
	alGenSources(1, &play_source);
	ALenum error = alGetError();
	if (error != AL_NO_ERROR || play_source == 0) {
		std::cerr << "Failed to create OpenAL source: " << error << std::endl;
		return;
	}
	
	alSourcei(play_source, AL_BUFFER, static_cast<ALint>(buffer_id));
	error = alGetError();
	if (error != AL_NO_ERROR) {
		std::cerr << "Failed to attach buffer " << buffer_id << " to source " << play_source << ": " << error << std::endl;
		alDeleteSources(1, &play_source);
		return;
	}
	
	// Set source properties for 2D audio
	alSource3f(play_source, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(play_source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcef(play_source, AL_GAIN, 1.0f);
	alSourcei(play_source, AL_LOOPING, AL_FALSE);
	
	error = alGetError();
	if (error != AL_NO_ERROR) {
		std::cerr << "Error setting source properties: " << error << std::endl;
		alDeleteSources(1, &play_source);
		return;
	}
	
	// Play the sound
	alSourcePlay(play_source);
	
	// Check for errors
	error = alGetError();
	if (error != AL_NO_ERROR) {
		std::cerr << "OpenAL error during playback: " << error << std::endl;
		alDeleteSources(1, &play_source);
		return;
	}
	
	// Track the source for cleanup
	active_sources.push_back(play_source);
}

void Audio::export_type(sol::state& target) {
	Result<Audio>::export_type(target);
	target.new_usertype<Audio>(
		"Audio",
		"open", &Audio::open,
		"play", &Audio::play
	);
}