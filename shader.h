#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bindable.h"
#include "globject.h"
#include "result.h"

/// <summary>
/// An object that represents a program running on the GPU.
/// All shader programs are expected to be written in GLSL.
/// For more, see https://www.khronos.org/opengl/wiki/Shader_Compilation
/// </summary>
class Shader : Bindable, GlObject<SINGULAR_DESTRUCTOR> {
private:
	std::map<std::string, int> uniforms;
public:
	Shader() : GlObject(glDeleteProgram) { }
	static Result<Shader> create(std::string vertex_source, std::string fragment_source);
	void bind();
	void set_int(const char* name, int value);
	void set_float(const char* name, float value);
	void set_mat2(const char* name, glm::mat2 value);
	void set_mat3(const char* name, glm::mat3 value);
	void set_mat4(const char* name, glm::mat4 value);
	void set_vec2(const char* name, glm::vec2 value);
	void set_vec3(const char* name, glm::vec3 value);
	void set_vec4(const char* name, glm::vec4 value);
};