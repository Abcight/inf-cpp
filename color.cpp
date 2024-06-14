#include "color.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// =====================================================
// This is a free and open color pallete by mrmrs
// https://clrs.cc/ courtesy of https://github.com/mrmrs
// =====================================================
const Color Color::NAVY		(0x001f3f);
const Color Color::BLUE		(0x0074D9);
const Color Color::AQUA		(0x7FDBFF);
const Color Color::TEAL		(0x39CCCC);
const Color Color::PURPLE	(0xB10DC9);
const Color Color::FUCHSIA  (0xF012BE);
const Color Color::MAROON	(0x85144b);
const Color Color::RED		(0xFF4136);
const Color Color::ORANGE	(0xFF851B);
const Color Color::YELLOW	(0xFFDC00);
const Color Color::OLIVE	(0x3D9970);
const Color Color::GREEN	(0x2ECC40);
const Color Color::LIME		(0x01FF70);
const Color Color::BLACK	(0x111111);
const Color Color::GRAY		(0xAAAAAA);
const Color Color::SILVER	(0xDDDDDD);
const Color Color::WHITE	(0xFFFFFF);

Color::Color() {
	this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

Color::Color(glm::vec4 color) {
	this->color = color;
}

Color::Color(float r, float g, float b, float a) {
	this->color = glm::vec4(r, g, b, a);
}

Color::Color(int hex) {
	float r = ((hex >> 16) & 0xFF) / 255.0f;
	float g = ((hex >> 8) & 0xFF) / 255.0f;
	float b = ((hex) & 0xFF) / 255.0f;
	this->color = glm::vec4(r, g, b, 1.0f);
}

void Color::bind() {
	int program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	glUniform4f(
		glGetUniformLocation(program, "color"),
		this->color.x,
		this->color.y,
		this->color.z,
		this->color.a
	);
}

void Color::export_type(sol::state& target) {
	target.new_usertype<Color>(
		"Color",
		sol::constructors<
			Color(),
			Color(int),
			Color(float, float, float, float)
		>(),
		"lerp", [](Color a, Color b, float t) { return glm::mix(a.color, b.color, t); }
	);
	sol::table color_table = target["Color"];
	color_table.set(
		"NAVY", Color::NAVY,
		"BLUE", Color::BLUE,
		"AQUA", Color::AQUA,
		"TEAL", Color::TEAL,
		"PURPLE", Color::PURPLE,
		"FUCHSIA", Color::FUCHSIA,
		"MAROON", Color::MAROON,
		"RED", Color::RED,
		"ORANGE", Color::ORANGE,
		"YELLOW", Color::YELLOW,
		"OLIVE", Color::OLIVE,
		"GREEN", Color::GREEN,
		"LIME", Color::LIME,
		"BLACK", Color::BLACK,
		"GRAY", Color::GRAY,
		"SILVER", Color::SILVER,
		"WHITE", Color::WHITE
	);
}