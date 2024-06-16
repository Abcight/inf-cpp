#pragma once

#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sol/sol.hpp>
#include <glm/glm.hpp>

/// <summary>
/// An object that tracks user input (keyboard/mouse) for a specified GLFW window.
/// For more, see https://www.glfw.org/docs/3.3/group__keys.html
/// </summary>
class Input {
private:
	GLFWwindow* window_ptr;
	bool key_map_press[GLFW_KEY_LAST];
	bool key_map_hold[GLFW_KEY_LAST];
public:
	Input(GLFWwindow* window_ptr);

	// Force the input system to query for updates
	void update_keymap();

	// Returns true if the given key is pressed
	bool get_key(char key);

	// Returns true if the given key is pressed once
	bool get_key_down(char key);

	// Returns mouse cursor coordinates
	glm::vec2 get_cursor_position();

	// Exports this type to the target lua virtual machine
	static void export_type(sol::state& target);
};