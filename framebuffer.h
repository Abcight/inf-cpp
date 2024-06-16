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
	static Result<Framebuffer> create(unsigned int width, unsigned int height);
	static void export_type(sol::state& target);
	static void reset_to_default();
	virtual void extra_delete();
	void clear();
	void bind();
};