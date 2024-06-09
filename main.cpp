#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "renderer.h"
#include "texture.h"

static void export_glm(sol::state &target) {
	sol::usertype<glm::vec2> vec2_type = target.new_usertype<glm::vec2>("Vec2");
	vec2_type["x"] = &glm::vec2::x;
	vec2_type["y"] = &glm::vec2::y;

	sol::usertype<glm::vec3> vec3_type = target.new_usertype<glm::vec3>("Vec3");
	vec3_type["x"] = &glm::vec3::x;
	vec3_type["y"] = &glm::vec3::y;
	vec3_type["z"] = &glm::vec3::z;

	sol::usertype<glm::vec4> vec4_type = target.new_usertype<glm::vec4>("Vec4");
	vec4_type["x"] = &glm::vec4::x;
	vec4_type["y"] = &glm::vec4::y;
	vec4_type["z"] = &glm::vec4::z;
	vec4_type["a"] = &glm::vec4::a;
}

int main() {
	// set working path
	std::filesystem::current_path("./test/");

	// initialize renderer
	Result<Renderer> renderer_result = Renderer::create();
	if (renderer_result.consume(std::cout)) {
		glfwTerminate();
		return 0;
	}
	Renderer& renderer = renderer_result.unwrap();

	// initialize the vm
	sol::state vm;
	vm.open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::io,
		sol::lib::table,
		sol::lib::os
	);

	Renderer::export_type(vm);
	RenderCommand::export_type(vm);
	Texture::export_type(vm);
	export_glm(vm);

	auto result = vm.safe_script_file("main.lua", &sol::script_pass_on_error);
	if (!result.valid())
	{
		sol::error error = result;
		std::cout << error.what() << std::endl;
		glfwTerminate();
		return 0;
	}


	vm["Renderer"] = &renderer;

	// start the main loop
	while (renderer.wants_next_frame())
	{
		auto result = vm["Update"]();
		if (!result.valid())
		{
			sol::error error = result;
			std::cout << error.what() << std::endl;
		}

		renderer.draw_frame();
	}

	glfwTerminate();
	return 0;
}