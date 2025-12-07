#pragma once
#include <sol/sol.hpp>

class LuaType {
public:
	virtual void export_type(sol::state target) = 0;
};