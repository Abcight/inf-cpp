#pragma once

#include "bindable.h"
#include "globject.h"
#include "result.h"
#include <sol/sol.hpp>

class Texture : Bindable, GlObject<MULTIPLE_DESTRUCTOR> {
private:
	int width;
	int height;
	int channel_count;
public:
	static Result<Texture> Open(std::string path);
	static void export_type(sol::state& target);
	void set_smoothing(bool smoothing);
	void bind();
};