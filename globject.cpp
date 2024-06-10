#include "globject.h"
#include <iostream>

template<>
GlObject<SINGULAR_DESTRUCTOR>::GlObject() {
	this->counter = std::make_shared<void*>(nullptr);
	this->gpu_destructor = nullptr;
	this->handle = 0;
}

template<>
GlObject<SINGULAR_DESTRUCTOR>::~GlObject() {
	if (this->counter.use_count() == 1) {
		this->gpu_destructor(this->handle);
	}
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>::GlObject() {
	this->counter = std::make_shared<void*>(nullptr);
	this->gpu_destructor = nullptr;
	this->handle = 0;
}

template<>
GlObject<MULTIPLE_DESTRUCTOR>::~GlObject() {
	if (this->counter.use_count() == 1) {
		this->gpu_destructor(1, &this->handle);
	}
}