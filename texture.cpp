#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Result<Texture> Texture::Open(std::string path) {
	Texture result;
	result.gpu_destructor = glDeleteTextures;

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
	type["open"] = &Texture::Open;
	type["width"] = sol::readonly(&Texture::width);
	type["height"] = sol::readonly(&Texture::height);
}