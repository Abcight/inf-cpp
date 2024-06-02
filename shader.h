#pragma once
#include <string>

#include "bindable.h"
#include "result.h"

class Shader : Bindable {
private:
	int program_handle;
public:
	static Result<Shader> Create(std::string vertex_source, std::string fragment_source);
	void bind();
};