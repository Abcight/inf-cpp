#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>
#include "result.h"

static bool glfw_present;

/// <summary>
/// A singular command issued to the renderer during userland script execution.
/// </summary>
class RenderCommand {
public:
	glm::f64vec2 position;
	glm::f64vec2 scale;
	double rotation;
};

/// <summary>
/// An object that owns a system Window and allows drawing inside of it.
/// Multiple renderers are technically possible, but this is not exposed in the userland.
/// </summary>
class Renderer {
private:
	GLFWwindow* window;
	int quad_vao;
	int quad_vbo;
	int quad_ebo;
public:
	static Result<Renderer> create();

	bool wants_next_frame();
	void queue_command(RenderCommand command);
	void draw_frame();
};

void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);