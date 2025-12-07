#pragma once

#include <sol/sol.hpp>
#include "result.h"
#include "bindable.h"
#include "globject.h"
#include "texture.h"

/// <summary>
/// Represents an OpenGL Framebuffer Object.
/// For more, see https://www.khronos.org/opengl/wiki/Framebuffer_Object
/// </summary>
class Framebuffer : GlObject<MULTIPLE_DESTRUCTOR>, Bindable {
private:
	Texture texture;
	unsigned int render_buffer = 0;
public:
	Framebuffer() : GlObject(glDeleteFramebuffers) { };
	
	// Copy constructor - shares the OpenGL framebuffer handle and texture
	// Note: render_buffer is also shared (deleted only when last copy is destroyed)
	Framebuffer(const Framebuffer& other) = default;
	
	// Move constructor
	Framebuffer(Framebuffer&& other) noexcept = default;
	
	// Copy assignment - shares the OpenGL framebuffer handle and texture
	Framebuffer& operator=(const Framebuffer& other) = default;
	
	// Move assignment
	Framebuffer& operator=(Framebuffer&& other) noexcept = default;
	
	virtual void extra_delete();

	// Attempts to create a framebuffer with the given dimensions
	static Result<Framebuffer> create(unsigned int width, unsigned int height);

	// Exports this type to the target lua virtual machine
	static void export_type(sol::state& target);

	// Unbinds all framebuffers
	static void reset_to_default();

	// Clears the framebuffer's depth and color values
	void clear();

	// Sets this framebuffer as active
	void bind();
};