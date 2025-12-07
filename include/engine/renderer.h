#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <sol/sol.hpp>
#include <vector>
#include "result.h"
#include "shader.h"
#include "bindable.h"
#include "framebuffer.h"
#include "command.h"


/// <summary>
/// An object that owns a system Window and allows drawing inside of it.
/// Multiple renderers are technically possible, but this is not exposed in the userland.
/// </summary>
class Renderer {
private:
	GLFWwindow* window;
	unsigned int quad_vao;
	unsigned int quad_vbo;
	unsigned int quad_ebo;
	unsigned int screen_width;
	unsigned int screen_height;
	Shader default_shader;
	std::vector<RenderCommand> command_queue;

	friend void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:
	static Result<Renderer> create(int width, int height, std::string title);
	static void export_type(sol::state &target);

	// Does the program want to continue?
	bool wants_next_frame() const;

	// Pushes a command into the command queue
	void queue_command(RenderCommand command);

	// Executes a command immediatelly
	void execute_command(RenderCommand command);

	// Executes all queued commands
	void draw_frame();

	// Exit and pause execution, kill the system window
	void quit();

	// Returns the underlying system window pointer
	// This is the window this renderer will be drawing to
	GLFWwindow* get_window_ptr() const;
};

void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);