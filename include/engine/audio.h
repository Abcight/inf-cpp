#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <sol/sol.hpp>
#include <memory>
#include <vector>
#include "result.h"

/// <summary>
/// Represents a singular audio file loaded into the program's memory.
/// Supports playing through the OpenAL sound engine.
/// </summary>
class Audio {
private:
	// Reference-counted OpenAL buffer
	std::shared_ptr<ALuint> buffer;
	
	// Whether OpenAL is initialized
	static bool initialized;
	static ALCdevice* device;
	static ALCcontext* context;
	
	// Track active sources for cleanup
	static std::vector<ALuint> active_sources;
	
	// Initialize OpenAL if not already done
	static void init_openal();
	
	// Clean up stopped sources
	static void cleanup_stopped_sources();
	
	// Helper to get buffer ID (0 if no buffer)
	ALuint get_buffer_id() const {
		return buffer ? *buffer : 0;
	}
	
public:
	// Default constructor
	Audio() = default;
	
	// Share the buffer via reference counting
	Audio(const Audio& other) = default;
	
	// Move transfers buffer ownership
	Audio(Audio&& other) noexcept = default;
	
	// Share the buffer via reference counting
	Audio& operator=(const Audio& other) = default;
	
	Audio& operator=(Audio&& other) noexcept = default;
	
	~Audio() = default;
	
	// Attempts to load an audio file from a given path
	static Result<Audio> open(std::string path);

	// Exports this type to the target lua virtual machine
	static void export_type(sol::state& target);

	// Plays the audio
	void play();
};