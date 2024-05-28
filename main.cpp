#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "renderer.h"

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
	if (renderer_result.is_error()) {
		std::cout << renderer_result.error();
		glfwTerminate();
		return 0;
	}

	Renderer& render= renderer_result.unwrap();
	while (render.wants_next_frame())
	{
		render.draw_frame();
	}

	glfwTerminate();
	return 0;
}