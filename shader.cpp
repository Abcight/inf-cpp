#include "shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Result<Shader> Shader::Create(std::string vertex_source, std::string fragment_source) {
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
			std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED:") + info_log
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
			std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:") + info_log
		);
	}

	// link both shaders into a single program
	Shader result;
	result.program_handle = glCreateProgram();
	glAttachShader(result.program_handle, vertex_shader);
	glAttachShader(result.program_handle, fragment_shader);
	glLinkProgram(result.program_handle);

	// remove link artifacts
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return Result<Shader>(result);
}

void Shader::bind() {
	glUseProgram(this->program_handle);
}