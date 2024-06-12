#include <ik/irrKlang.h>
#include <fstream>
#include "audio.h"

irrklang::ISoundEngine* Audio::ENGINE = irrklang::createIrrKlangDevice();;

Result<Audio> Audio::open(std::string path) {
	Audio audio;

	std::ifstream in(path, std::ios_base::binary);
	if (!in) {
		return Result<Audio>(std::string("Couldn't open audio file at '") + path + "'");
	}

	in.seekg(0, std::ios::end);

	audio.data_size = in.tellg();
	audio.data = new char[audio.data_size];
	audio.handle = ENGINE->addSoundSourceFromMemory(
		audio.data,
		audio.data_size,
		path.c_str(),
		false
	);

	in.seekg(0, std::ios::beg);
	in.read(&audio.data[0], audio.data_size);

	return Result<Audio>(audio);
}

void Audio::play() {
	ENGINE->play2D(this->handle);
}

void Audio::export_type(sol::state& target) {
	Result<Audio>::export_type(target);

	sol::usertype<Audio> audio = target.new_usertype<Audio>(
		"Audio",
		"open", &Audio::open,
		"play", &Audio::play
	);
}