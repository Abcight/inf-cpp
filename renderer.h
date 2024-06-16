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

static bool glfw_present;

/// <summary>
/// A singular command issued to the renderer during userland script execution.
/// </summary>
class RenderCommand {
private:
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;
	float layer;
	Framebuffer* target = nullptr;
	Bindable** bindables;
	int bindables_length = 0;
public:
	RenderCommand();
	static void export_type(sol::state &target);
	RenderCommand& to(Framebuffer* buffer);
	RenderCommand& with(Bindable* bindable);
	RenderCommand& with_position(glm::vec2 position);
	RenderCommand& with_position_xy(float x, float y);
	RenderCommand& with_scale(glm::vec2 scale);
	RenderCommand& with_scale_xy(float x, float y);
	RenderCommand& with_rotation(float rotation);
	RenderCommand& with_layer(float layer);
	friend class Renderer;
};

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

	bool wants_next_frame();
	void queue_command(RenderCommand command);
	void execute_command(RenderCommand command);
	void draw_frame();
	GLFWwindow* get_window_ptr();
};

void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);