#pragma once

#include "bindable.h"
#include "result.h"
#include <sol/sol.hpp>

class Texture : Bindable {
private:
	int width;
	int height;
	int channel_count;
	unsigned int handle;
public:
	static Result<Texture> Open(std::string path);
	static void export_type(sol::state& target);
	void set_smoothing(bool smoothing);
	void bind();
};