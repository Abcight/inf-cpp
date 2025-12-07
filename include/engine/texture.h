#pragma once

#include "bindable.h"
#include "globject.h"
#include "result.h"
#include <sol/sol.hpp>

/// <summary>
/// An object that represents an OpenGL texture.
/// It can be either loaded from file (stb lib) or allocated.
/// For more, see https://www.khronos.org/opengl/wiki/Texture
/// </summary>
class Texture : Bindable, public GlObject<MULTIPLE_DESTRUCTOR> {
private:
	int width;
	int height;
	int channel_count;
public:
	Texture();
	Texture(unsigned int width, unsigned int height);
	
	// Copy constructor - shares the OpenGL texture handle
	Texture(const Texture& other) = default;
	
	// Move constructor
	Texture(Texture&& other) noexcept = default;
	
	// Copy assignment - shares the OpenGL texture handle
	Texture& operator=(const Texture& other) = default;
	
	// Move assignment
	Texture& operator=(Texture&& other) noexcept = default;

	// Attempts to load an image file from a given path and store it
	// inside the GPU's VRAM.
	static Result<Texture> open(std::string path);

	// Exports this type to the target lua virtual machine
	static void export_type(sol::state& target);

	// If this is set to true, the texture will use interpolation
	// when being stretched or resized.
	void set_smoothing(bool smoothing);

	// Sets this texture as active
	void bind();
};