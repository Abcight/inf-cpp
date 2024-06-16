#include "globject.h"
#include <iostream>

template<typename T>
GlObject<T>::GlObject(T destructor) {
	this->counter = std::make_shared<void*>(nullptr);
	this->gpu_destructor = destructor;
	this->handle = 0;
}

template<>
GlObject<SINGULAR_DESTRUCTOR>::GlObject(SINGULAR_DESTRUCTOR destructor) {
	this->counter = std::make_shared<void*>(nullptr);
	this->gpu_destructor = destructor;
	this->handle = 0;
}

template<>
GlObject<SINGULAR_DESTRUCTOR>::~GlObject() {
	if (this->counter.use_count() == 1) {
		this->gpu_destructor(this->handle);
		this->extra_delete();
	}
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>::GlObject(MULTIPLE_DESTRUCTOR destructor) {
	this->counter = std::make_shared<void*>(nullptr);
	this->gpu_destructor = destructor;
	this->handle = 0;
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>::~GlObject() {
	if (this->counter.use_count() == 1) {
		this->gpu_destructor(1, &this->handle);
		this->extra_delete();
	}
}