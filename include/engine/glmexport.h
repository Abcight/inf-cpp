#pragma once

#include <sol/sol.hpp>
#include <glm/common.hpp>

/// <summary>
/// Exports GLM types to the target Lua VM.
/// </summary>
/// <param name="target">A lua VM reference</param>
static void export_glm(sol::state& target) {
	sol::usertype<glm::vec2> vec2 = target.new_usertype<glm::vec2>(
		"Vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y,
		"lerp", [](glm::vec2 a, glm::vec2 b, float t) { return glm::mix(a, b, t); }
	);

	sol::usertype<glm::vec3> vec3 = target.new_usertype<glm::vec3>(
		"Vec3",
		sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		"lerp", [](glm::vec3 a, glm::vec3 b, float t) { return glm::mix(a, b, t); }
	);

	sol::usertype<glm::vec4> vec4 = target.new_usertype<glm::vec4>(
		"Vec4",
		sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"a", &glm::vec4::a,
		"lerp", [](glm::vec4 a, glm::vec4 b, float t) { return glm::mix(a, b, t); }
	);
}