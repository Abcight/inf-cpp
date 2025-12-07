#pragma once

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

/// <summary>
/// Represents a manageable GlObject. Deriving this will
/// automatically perform memory management for the underlying
/// OpenGL object, so long as the gpu_destructor is provided.
/// </summary>
template <typename T>
class GlObject {
private:
	std::shared_ptr<void*> counter;
	T gpu_destructor;
public:
	unsigned int handle;
	virtual void extra_delete() {};
	GlObject(T destructor);
	~GlObject();
};

typedef void (APIENTRYP SINGULAR_DESTRUCTOR)(GLuint handle);
typedef void (APIENTRYP MULTIPLE_DESTRUCTOR)(GLsizei n, const GLuint* handles);