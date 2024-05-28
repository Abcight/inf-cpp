#include "renderer.h"
#include <iostream>

void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// TODO: Remove this later
	// auto renderer = (Renderer*)glfwGetWindowUserPointer(window);
	glViewport(0, 0, width, height);
}

Result<Renderer> Renderer::create() {
	if (!glfw_present) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfw_present = true;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Window", NULL, NULL);
	if (window == NULL)
		return Result<Renderer>::Result("Failed to create GLFW window");

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return Result<Renderer>::Result("Failed to initialize GLAD");

	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
	glViewport(0, 0, 800, 600);

	Renderer value;
	value.window = window;

	return Result<Renderer>::Result(value);
}

void Renderer::draw_frame() {
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);

	glClearColor(0.6f, 0.6f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(window);
	glfwPollEvents();

	glfwMakeContextCurrent(nullptr);
}

void Renderer::queue_command(RenderCommand command) {

}

bool Renderer::wants_next_frame() {
	return !glfwWindowShouldClose(window);
}