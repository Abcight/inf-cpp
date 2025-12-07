#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

Result<Shader> Shader::create(std::string vertex_source, std::string fragment_source) {
	// allocate data to take compilation info out
	int success;
	char info_log[512];
	const char* tmp;

	// upload vertex shader to the GPU
	tmp = vertex_source.c_str();
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &tmp, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		return Result<Shader>(
			std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n\t") + info_log
		);
	}

	// upload fragment shader to the GPU
	tmp = fragment_source.c_str();
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &tmp, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		return Result<Shader>(
			std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n\t") + info_log
		);
	}

	// link both shaders into a single program
	Shader result;
	GLuint program_handle = glCreateProgram();
	result.set_handle(program_handle);
	glAttachShader(result.handle, vertex_shader);
	glAttachShader(result.handle, fragment_shader);
	glLinkProgram(result.handle);

	// remove link artifacts
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// get program uniforms from the GPU and cache them
	int uniform_count = 0;
	std::vector<GLchar> uniform_name(256);
	glGetProgramiv(result.handle, GL_ACTIVE_UNIFORMS, &uniform_count);
	for (int uniform = 0; uniform < uniform_count; uniform++) {
		int array_size = 0;
		int actual_length = 0;
		unsigned int type = 0;
		glGetActiveUniform(
			result.handle,
			uniform,
			uniform_name.size(),
			&actual_length,
			&array_size,
			&type,
			&uniform_name[0]
		);
		std::string name((char*)&uniform_name[0], actual_length);
		result.uniforms[name] = uniform;
	}

	return Result<Shader>(result);
}

void Shader::bind() {
	glUseProgram(this->handle);
}

void Shader::set_int(const char* name, int value) {
	if (!this->uniforms.count(name))
		return;

	glUniform1i(this->uniforms[name], value);
}

void Shader::set_float(const char* name, float value) {
	if (!this->uniforms.count(name))
		return;

	glUniform1f(this->uniforms[name], value);
}

void Shader::set_mat2(const char* name, glm::mat2 value) {
	if (!this->uniforms.count(name))
		return;

	glUniformMatrix2fv(this->uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_mat3(const char* name, glm::mat3 value) {
	if (!this->uniforms.count(name))
		return;

	glUniformMatrix3fv(this->uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_mat4(const char* name, glm::mat4 value) {
	if (!this->uniforms.count(name))
		return;

	glUniformMatrix4fv(this->uniforms[name], 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_vec2(const char* name, glm::vec2 value) {
	if (!this->uniforms.count(name))
		return;

	glUniform2f(this->uniforms[name], value.x, value.y);
}

void Shader::set_vec3(const char* name, glm::vec3 value) {
	if (!this->uniforms.count(name))
		return;

	glUniform3f(this->uniforms[name], value.x, value.y, value.z);
}

void Shader::set_vec4(const char* name, glm::vec4 value) {
	if (!this->uniforms.count(name))
		return;

	glUniform4f(this->uniforms[name], value.x, value.y, value.z, value.a);
}