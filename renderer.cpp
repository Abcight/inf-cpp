#include "renderer.h"
#include <iostream>

const float vertices[] = {
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
};

const unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

const char* vertex_shader_src = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragment_shader_src = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

	// create vertex array object
	glGenVertexArrays(1, &value.quad_vao);
	glBindVertexArray(value.quad_vao);

	// create element buffer object
	glGenBuffers(1, &value.quad_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, value.quad_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// upload quad data to the GPU
	glGenBuffers(1, &value.quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, value.quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// upload shader program to the GPU
	Result<Shader> default_shader = Shader::Create(vertex_shader_src, fragment_shader_src);
	if (default_shader.is_error()) {
		return Result<Renderer>::Wrap(
			"Failed to create default shader:",
			default_shader.error()
		);
	}
	value.default_shader = default_shader.unwrap();

	// set up attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return Result<Renderer>::Result(value);
}

void Renderer::draw_frame() {
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);

	glClearColor(0.6f, 0.6f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->default_shader.bind();
	glBindVertexArray(this->quad_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->quad_ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
	glfwPollEvents();

	glfwMakeContextCurrent(nullptr);
}

void Renderer::queue_command(RenderCommand command) {

}

bool Renderer::wants_next_frame() {
	return !glfwWindowShouldClose(window);
}