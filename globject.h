#pragma once

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

template <typename T>
class GlObject {
private:
	std::shared_ptr<void*> counter;
public:
	unsigned int handle;
	T gpu_destructor;
	GlObject();
	~GlObject();
};

typedef void (APIENTRYP SINGULAR_DESTRUCTOR)(GLuint handle);
typedef void (APIENTRYP MULTIPLE_DESTRUCTOR)(GLsizei n, const GLuint* handles);