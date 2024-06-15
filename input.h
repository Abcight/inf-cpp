#pragma once

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sol/sol.hpp>
#include <glm/glm.hpp>

/// <summary>
/// An object that tracks user input (keyboard/mouse) for
/// a specified GLFW window.
/// </summary>
class Input {
private:
	GLFWwindow* window_ptr;
	bool key_map_press[GLFW_KEY_LAST];
	bool key_map_hold[GLFW_KEY_LAST];
public:
	void update_keymap();
	bool get_key(char key);
	bool get_key_down(char key);
	glm::vec2 get_cursor_position();
	Input(GLFWwindow* window_ptr);
	static void export_type(sol::state& target);
};