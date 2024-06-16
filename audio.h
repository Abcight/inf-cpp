#pragma once
#include <ik/irrKlang.h>
#include <sol/sol.hpp>
#include "result.h"

/// <summary>
/// Represents a singular audio file loaded into the program's memory.
/// Supports playing through the irrklang sound engine.
/// </summary>
class Audio {
private:
	// The globally available audio engine pointer
	static irrklang::ISoundEngine* ENGINE;

	// The audio handle created by irrklang
	irrklang::ISoundSource* handle = nullptr;

	// The number of bytes in this audio file
	int data_size;

	// Audio file data (bytes)
	char* data;
public:
	// Attempts to load an audio file from a given path
	static Result<Audio> open(std::string path);

	// Exports this type to the target lua virtual machine
	static void export_type(sol::state& target);

	// Plays the audio
	void play();
};