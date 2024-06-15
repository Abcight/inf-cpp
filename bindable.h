#pragma once

#include <sol/sol.hpp>

/// <summary>
/// This is an abstract class that represents a GL-Bindable object.
/// </summary>
class Bindable {
public:
	virtual void bind() = 0;
};