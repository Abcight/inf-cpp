#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "framebuffer.h"

Result<Framebuffer> Framebuffer::create(unsigned int width, unsigned int height) {
	Framebuffer value;

	glGenFramebuffers(1, &value.handle);
	glBindFramebuffer(GL_FRAMEBUFFER, value.handle);

	value.texture = Texture(width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, value.texture.handle, 0);

	glGenRenderbuffers(1, &value.render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, value.render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, value.render_buffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return Result<Framebuffer>("ERROR::FRAMEBUFFER:: Framebuffer couldn't be created!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return Result<Framebuffer>(value);
}

void Framebuffer::extra_delete() {
	glDeleteRenderbuffers(1, &this->render_buffer);
}

void Framebuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
}

void Framebuffer::reset_to_default() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::clear() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::export_type(sol::state& target) {
	Result<Framebuffer>::export_type(target);
	target.new_usertype<Framebuffer>(
		"Framebuffer",
		"create", &Framebuffer::create,
		"new", &Framebuffer::create,
		"bind", &Framebuffer::bind,
		"texture", &Framebuffer::texture,
		"reset_to_default", &Framebuffer::reset_to_default,
		"clear", &Framebuffer::clear
	);
}