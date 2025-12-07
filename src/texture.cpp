#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Texture::Texture() : GlObject(glDeleteTextures) {
	this->width = 0;
	this->height = 0;
	this->channel_count = 0;
	this->set_handle(0);
}

Texture::Texture(unsigned int width, unsigned int height) : GlObject(glDeleteTextures) {
	this->width = width;
	this->height = height;
	this->channel_count = 0;

	GLuint tex_handle;
	glGenTextures(1, &tex_handle);
	this->set_handle(tex_handle);
	glBindTexture(GL_TEXTURE_2D, this->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Result<Texture> Texture::open(std::string path) {
	Texture result;

	unsigned char* bytes;

	stbi_set_flip_vertically_on_load(true);
	bytes = stbi_load(
		path.c_str(),
		&result.width,
		&result.height,
		&result.channel_count,
		0
	);

	if (!bytes) {
		return Result<Texture>(std::string("STB_IMAGE Load failure, at '") + path + "'");
	}

	// Determine OpenGL format based on channel count
	GLenum internal_format = GL_RGB;
	GLenum format = GL_RGB;
	if (result.channel_count == 1) {
		internal_format = GL_RED;
		format = GL_RED;
	} else if (result.channel_count == 2) {
		internal_format = GL_RG;
		format = GL_RG;
	} else if (result.channel_count == 3) {
		internal_format = GL_RGB;
		format = GL_RGB;
	} else if (result.channel_count == 4) {
		internal_format = GL_RGBA;
		format = GL_RGBA;
	} else {
		stbi_image_free(bytes);
		return Result<Texture>(std::string("Unsupported channel count: ") + std::to_string(result.channel_count));
	}

	GLuint tex_handle;
	glGenTextures(1, &tex_handle);
	result.set_handle(tex_handle);
	glBindTexture(GL_TEXTURE_2D, result.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, result.width, result.height, 0, format, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

	result.set_smoothing(true);

	return Result<Texture>(result);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, this->handle);
}

void Texture::set_smoothing(bool smoothing) {
	this->bind();
	if (smoothing) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void Texture::export_type(sol::state& target) {
	Result<Texture>::export_type(target);

	sol::usertype<Texture> type = target.new_usertype<Texture>("Texture");
	type["open"] = &Texture::open;
	type["width"] = sol::readonly(&Texture::width);
	type["height"] = sol::readonly(&Texture::height);
}