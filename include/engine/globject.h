#pragma once

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

typedef void (APIENTRYP SINGULAR_DESTRUCTOR)(GLuint handle);
typedef void (APIENTRYP MULTIPLE_DESTRUCTOR)(GLsizei n, const GLuint* handles);

/// <summary>
/// Represents a manageable GlObject. Deriving this will
/// automatically perform memory management for the underlying
/// OpenGL object, so long as the gpu_destructor is provided.
/// Uses proper reference counting so copies share the same OpenGL resource.
/// </summary>
template <typename T>
class GlObject {
protected:
	// Shared pointer to the handle - copies will share this
	// The custom deleter will call the GPU destructor when the last reference is destroyed
	std::shared_ptr<GLuint> handle_ptr;
	T gpu_destructor;
	
	// Synchronize the handle member with the shared_ptr value
	void sync_handle() {
		if (handle_ptr) {
			handle = *handle_ptr;
		} else {
			handle = 0;
		}
	}
	
public:
	// Public handle - kept in sync with handle_ptr for backward compatibility
	// Note: Direct writes to handle won't update handle_ptr, use set_handle() instead
	unsigned int handle;
	
	virtual void extra_delete() {};
	
	// Constructor
	GlObject(T destructor);
	
	// Copy constructor - shares the handle
	GlObject(const GlObject& other);
	
	// Move constructor
	GlObject(GlObject&& other) noexcept;
	
	// Copy assignment - shares the handle
	GlObject& operator=(const GlObject& other);
	
	// Move assignment
	GlObject& operator=(GlObject&& other) noexcept;
	
	~GlObject();
	
	// Helper to set handle and update shared_ptr with proper deleter
	// This should be called when the handle is first created
	void set_handle(GLuint h);
	
	// Helper to get pointer to handle (for OpenGL functions that need GLuint*)
	GLuint* get_handle_ptr() {
		if (!handle_ptr) {
			// Initialize with 0, deleter will be set up when handle is actually created
			handle_ptr = std::shared_ptr<GLuint>(new GLuint(0), [](GLuint*){});
			handle = 0;
		}
		return handle_ptr.get();
	}
};