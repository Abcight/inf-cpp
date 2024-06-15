#pragma once

#include "bindable.h"
#include "globject.h"
#include "result.h"
#include <sol/sol.hpp>

/// <summary>
/// An object that represents an OpenGL texture.
/// It can be either loaded from file (stb lib) or allocated.
/// </summary>
class Texture : Bindable, public GlObject<MULTIPLE_DESTRUCTOR> {
private:
	int width;
	int height;
	int channel_count;
public:
	Texture();
	Texture(unsigned int width, unsigned int height);
	static Result<Texture> open(std::string path);
	static void export_type(sol::state& target);
	void set_smoothing(bool smoothing);
	void bind();
};