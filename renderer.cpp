#include "renderer.h"
#include <iostream>
#include <glm/glm.hpp>

float vertices[] = {
	 // Vertices          // UV
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f
};

const unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

const char* vertex_shader_src = "#version 330 core\n"
"layout (location = 0) in vec3 a_pos;\n"
"layout (location = 1) in vec2 a_uv;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(a_pos, 1.0);\n"
"	uv = a_uv;\n"
"}\0";

const char* fragment_shader_src = "#version 330 core\n"
"in vec2 uv;"
"out vec4 color;\n"
"uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"	color = texture(tex, uv);\n"
"}\0";

void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	auto renderer = (Renderer*)glfwGetWindowUserPointer(window);
	renderer->screen_width = width;
	renderer->screen_height = height;
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

	int width = 800;
	int height = 600;

	GLFWwindow* window = glfwCreateWindow(width, height, "GLFW Window", NULL, NULL);
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
	Result<Shader> default_shader = Shader::create(vertex_shader_src, fragment_shader_src);
	if (default_shader.is_error()) {
		return Result<Renderer>::Wrap(
			"Failed to create default shader:",
			default_shader.error()
		);
	}
	value.default_shader = default_shader.unwrap();
	value.screen_width = width;
	value.screen_height = height;

	// set up attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

	return Result<Renderer>::Result(value);
}

void Renderer::draw_frame() {
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);

	glClearColor(0.6f, 0.6f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(RenderCommand &command : command_queue) {
		this->execute_command(command);
	}
	command_queue.clear();

	glfwSwapBuffers(window);
	glfwPollEvents();

	glfwMakeContextCurrent(nullptr);
}

void Renderer::queue_command(RenderCommand command) {
	this->command_queue.push_back(command);
}

void Renderer::execute_command(RenderCommand command) {
	glm::mat4 projection = glm::ortho(
		0.0f, (float)this->screen_width,
		0.0f, (float)this->screen_height,
		0.1f, 100.0f
	);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(command.position, -99.0f + command.layer));
	model = glm::rotate(model, glm::radians(command.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(command.scale, 1.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));

	this->default_shader.bind();
	this->default_shader.set_mat4("model", model);
	this->default_shader.set_mat4("view", view);
	this->default_shader.set_mat4("projection", projection);

	for (Bindable* bindable : command.bindables) {
		bindable->bind();
	}

	glBindVertexArray(this->quad_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->quad_ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool Renderer::wants_next_frame() {
	return !glfwWindowShouldClose(window);
}

GLFWwindow* Renderer::get_window_ptr() {
	return this->window;
}

void Renderer::export_type(sol::state &target) {
	sol::usertype<Renderer> renderer_type = target.new_usertype<Renderer>("Renderer");
	renderer_type["queue_command"] = &Renderer::queue_command;
	renderer_type["screen_width"] = sol::readonly(&Renderer::screen_width);
	renderer_type["screen_height"] = sol::readonly(&Renderer::screen_height);
}

RenderCommand::RenderCommand() {
	this->position = glm::vec2(0.0f, 0.0f);
	this->scale = glm::vec2(1.0f, 1.0f);
	this->rotation = 0.0f;
	this->layer = 1.0f;
}

RenderCommand& RenderCommand::with(Bindable* bindable) {
	this->bindables.push_back(bindable);
	return *this;
}

RenderCommand& RenderCommand::with_position(glm::vec2 position) {
	this->position = position;
	return *this;
}

RenderCommand& RenderCommand::with_position_xy(float x, float y) {
	this->position.x = x;
	this->position.y = y;
	return *this;
}

RenderCommand& RenderCommand::with_scale(glm::vec2 scale) {
	this->scale = scale;
	return *this;
}

RenderCommand& RenderCommand::with_scale_xy(float x, float y) {
	this->scale.x = x;
	this->scale.y = y;
	return *this;
}

RenderCommand& RenderCommand::with_rotation(float rotation) {
	this->rotation = rotation;
	return *this;
}

RenderCommand& RenderCommand::with_layer(float layer) {
	this->layer = layer;
	return *this;
}

void RenderCommand::export_type(sol::state &target) {
	target.new_usertype<RenderCommand>(
		"RenderCommand",
		"position", &RenderCommand::position,
		"scale", &RenderCommand::scale,
		"rotation", &RenderCommand::rotation,
		"layer", &RenderCommand::layer,
		"with", &RenderCommand::with,
		"with_position", sol::overload(
			&RenderCommand::with_position,
			&RenderCommand::with_position_xy
		),
		"with_scale", sol::overload(
			&RenderCommand::with_scale,
			&RenderCommand::with_scale_xy
		),
		"with_rotation", &RenderCommand::with_rotation,
		"with_layer", &RenderCommand::with_layer
	);
}