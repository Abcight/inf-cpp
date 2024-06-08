#pragma once

#include "bindable.h"
#include "result.h"

class Texture : Bindable {
private:
	int width;
	int height;
	int channel_count;
	unsigned int handle;
public:
	static Result<Texture> Open(std::string path);
	void set_smoothing(bool smoothing);
	void bind();
};