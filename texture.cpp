#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Result<Texture> Texture::Open(std::string path) {
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

	glGenTextures(1, &result.handle);
	glBindTexture(GL_TEXTURE_2D, result.handle);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		result.width,
		result.height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		bytes
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

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