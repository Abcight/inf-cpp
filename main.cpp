#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "renderer.h"
#include "texture.h"

int main() {
	sol::state vm;
	vm.open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::io,
		sol::lib::table
	);
	vm.do_string("print('Hello, virtual world!')");

	Result<Renderer> renderer_result = Renderer::create();
	if (renderer_result.consume(std::cout)) {
		glfwTerminate();
		return 0;
	}

	Renderer& render = renderer_result.unwrap();
	Result<Texture> texture = Texture::Open("wall.jpg");
	if (texture.consume(std::cout)) {
		glfwTerminate();
		return 0;
	}
	texture.unwrap().bind();

	while (render.wants_next_frame())
	{
		render.queue_command(RenderCommand{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(100.0f, 100.0f),
			0.0f,
			0.0f
		});

		render.draw_frame();
	}

	glfwTerminate();
	return 0;
}