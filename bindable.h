#pragma once

#include <sol/sol.hpp>

class Bindable {
public:
	virtual void bind() = 0;
};