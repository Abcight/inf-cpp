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
	static irrklang::ISoundEngine* ENGINE;
	irrklang::ISoundSource* handle = nullptr;
	int data_size;
	char* data;
public:
	static Result<Audio> open(std::string path);
	static void export_type(sol::state& target);
	void play();
};