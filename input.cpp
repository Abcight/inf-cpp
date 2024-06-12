#include "input.h"

Input::Input(GLFWwindow* window_ptr) {
	this->window_ptr = window_ptr;
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		this->key_map_hold[i] = false;
		this->key_map_press[i] = false;
	}
}

void Input::update_keymap() {
	for (int key = 0; key < GLFW_KEY_LAST; key++) {
		bool hold = glfwGetKey(this->window_ptr, key);
		this->key_map_press[key] = !this->key_map_hold[key] && hold;
		this->key_map_hold[key] = hold;
	}
}

bool Input::get_key(char key) {
	return this->key_map_hold[key];
}

bool Input::get_key_down(char key) {
	return this->key_map_press[key];
}

glm::vec2 Input::get_cursor_position() {
	double xpos, ypos;
	int width, height;
	glfwGetWindowSize(this->window_ptr, &width, &height);
	glfwGetCursorPos(this->window_ptr, &xpos, &ypos);
	return glm::vec2((float)xpos, (float)height - (float)ypos);
}

void Input::export_type(sol::state& target) {
	sol::usertype<Input> type = target.new_usertype<Input>("Input");
	type["get_key"] = &Input::get_key;
	type["get_key_down"] = &Input::get_key_down;
	type["get_cursor_position"] = &Input::get_cursor_position;
}