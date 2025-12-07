#include "globject.h"
#include <iostream>
#include <functional>

// Base template constructor
template<typename T>
GlObject<T>::GlObject(T destructor) : handle_ptr(nullptr), gpu_destructor(destructor), handle(0) {
}

// Base template copy constructor
template<typename T>
GlObject<T>::GlObject(const GlObject& other) 
	: handle_ptr(other.handle_ptr), gpu_destructor(other.gpu_destructor), handle(0) {
	sync_handle();
}

// Base template move constructor
template<typename T>
GlObject<T>::GlObject(GlObject&& other) noexcept
	: handle_ptr(std::move(other.handle_ptr)), gpu_destructor(other.gpu_destructor), handle(0) {
	sync_handle();
	other.handle = 0;
}

// Base template copy assignment
template<typename T>
GlObject<T>& GlObject<T>::operator=(const GlObject& other) {
	if (this != &other) {
		handle_ptr = other.handle_ptr;
		gpu_destructor = other.gpu_destructor;
		sync_handle();
	}
	return *this;
}

// Base template move assignment
template<typename T>
GlObject<T>& GlObject<T>::operator=(GlObject&& other) noexcept {
	if (this != &other) {
		handle_ptr = std::move(other.handle_ptr);
		gpu_destructor = other.gpu_destructor;
		sync_handle();
		other.handle = 0;
	}
	return *this;
}

// Base template destructor
template<typename T>
GlObject<T>::~GlObject() {
	// shared_ptr deleter will handle GPU cleanup automatically
	// extra_delete() needs special handling - see specializations
}

// Specialization for SINGULAR_DESTRUCTOR
template<>
GlObject<SINGULAR_DESTRUCTOR>::GlObject(SINGULAR_DESTRUCTOR destructor) 
	: handle_ptr(nullptr), gpu_destructor(destructor), handle(0) {
}

template<>
GlObject<SINGULAR_DESTRUCTOR>::GlObject(const GlObject& other)
	: handle_ptr(other.handle_ptr), gpu_destructor(other.gpu_destructor), handle(0) {
	sync_handle();
}

template<>
GlObject<SINGULAR_DESTRUCTOR>::GlObject(GlObject&& other) noexcept
	: handle_ptr(std::move(other.handle_ptr)), gpu_destructor(other.gpu_destructor), handle(0) {
	sync_handle();
	other.handle = 0;
}

template<>
GlObject<SINGULAR_DESTRUCTOR>& GlObject<SINGULAR_DESTRUCTOR>::operator=(const GlObject& other) {
	if (this != &other) {
		handle_ptr = other.handle_ptr;
		gpu_destructor = other.gpu_destructor;
		sync_handle();
	}
	return *this;
}

template<>
GlObject<SINGULAR_DESTRUCTOR>& GlObject<SINGULAR_DESTRUCTOR>::operator=(GlObject&& other) noexcept {
	if (this != &other) {
		handle_ptr = std::move(other.handle_ptr);
		gpu_destructor = other.gpu_destructor;
		sync_handle();
		other.handle = 0;
	}
	return *this;
}

template<>
GlObject<SINGULAR_DESTRUCTOR>::~GlObject() {
	// Check if we're the last reference before shared_ptr is destroyed
	// If so, call extra_delete() to clean up additional resources
	if (handle_ptr && handle_ptr.use_count() == 1) {
		extra_delete();
	}
	// shared_ptr deleter will handle GPU cleanup automatically
}

// Specialization for MULTIPLE_DESTRUCTOR
template<>
GlObject<MULTIPLE_DESTRUCTOR>::GlObject(MULTIPLE_DESTRUCTOR destructor)
	: handle_ptr(nullptr), gpu_destructor(destructor), handle(0) {
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>::GlObject(const GlObject& other)
	: handle_ptr(other.handle_ptr), gpu_destructor(other.gpu_destructor), handle(0) {
	sync_handle();
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>::GlObject(GlObject&& other) noexcept
	: handle_ptr(std::move(other.handle_ptr)), gpu_destructor(other.gpu_destructor), handle(0) {
	sync_handle();
	other.handle = 0;
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>& GlObject<MULTIPLE_DESTRUCTOR>::operator=(const GlObject& other) {
	if (this != &other) {
		handle_ptr = other.handle_ptr;
		gpu_destructor = other.gpu_destructor;
		sync_handle();
	}
	return *this;
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>& GlObject<MULTIPLE_DESTRUCTOR>::operator=(GlObject&& other) noexcept {
	if (this != &other) {
		handle_ptr = std::move(other.handle_ptr);
		gpu_destructor = other.gpu_destructor;
		sync_handle();
		other.handle = 0;
	}
	return *this;
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>::~GlObject() {
	// Check if we're the last reference before shared_ptr is destroyed
	// If so, call extra_delete() to clean up additional resources
	if (handle_ptr && handle_ptr.use_count() == 1) {
		extra_delete();
	}
	// shared_ptr deleter will handle GPU cleanup automatically
}

// Implementation of set_handle() for SINGULAR_DESTRUCTOR
template<>
void GlObject<SINGULAR_DESTRUCTOR>::set_handle(GLuint h) {
	if (!handle_ptr) {
		// Create shared_ptr with custom deleter that calls the GPU destructor
		SINGULAR_DESTRUCTOR deleter_func = gpu_destructor;
		handle_ptr = std::shared_ptr<GLuint>(
			new GLuint(h),
			[deleter_func](GLuint* handle) {
				if (handle && *handle != 0) {
					deleter_func(*handle);
				}
				delete handle;
			}
		);
	} else {
		*handle_ptr = h;
	}
	handle = h;
}

// Implementation of set_handle() for MULTIPLE_DESTRUCTOR
template<>
void GlObject<MULTIPLE_DESTRUCTOR>::set_handle(GLuint h) {
	if (!handle_ptr) {
		// Create shared_ptr with custom deleter that calls the GPU destructor
		MULTIPLE_DESTRUCTOR deleter_func = gpu_destructor;
		handle_ptr = std::shared_ptr<GLuint>(
			new GLuint(h),
			[deleter_func](GLuint* handle) {
				if (handle && *handle != 0) {
					deleter_func(1, handle);
				}
				delete handle;
			}
		);
	} else {
		*handle_ptr = h;
	}
	handle = h;
}