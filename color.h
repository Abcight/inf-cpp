#pragma once

#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include "bindable.h"

/// <summary>
/// A convenience class that encapsulates Vec4 into distinct
/// color values, while providing a bindable interface.
/// </summary>
class Color : Bindable {
private:
	glm::vec4 color;
public:
	const static Color NAVY;
	const static Color BLUE;
	const static Color AQUA;
	const static Color TEAL;
	const static Color PURPLE;
	const static Color FUCHSIA;
	const static Color MAROON;
	const static Color RED;
	const static Color ORANGE;
	const static Color YELLOW;
	const static Color OLIVE;
	const static Color GREEN;
	const static Color LIME;
	const static Color BLACK;
	const static Color GRAY;
	const static Color SILVER;
	const static Color WHITE;

	// Constructs the default color (white)
	Color();

	// Decomposes a vector (r, g, b, a) into a color
	Color(glm::vec4 color);

	// Creates a color from the given rgba values
	Color(float r, float g, float b, float a);

	// Decomposes a hex value into (r,g,b) components and constructs a color
	Color(int hex);

	// Bind the color to the currently active shader
	// This will affect any 'uniform vec4 color' in a GLSL program
	void bind();

	// Exports this type to the target lua virtual machine
	static void export_type(sol::state& target);
};