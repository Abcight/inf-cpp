#include <filesystem>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "audio.h"

static void export_glm(sol::state &target) {
	sol::usertype<glm::vec2> vec2 = target.new_usertype<glm::vec2>(
		"Vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y,
		"lerp", [](glm::vec2 a, glm::vec2 b, float t) { return glm::mix(a, b, t); }
	);

	sol::usertype<glm::vec3> vec3 = target.new_usertype<glm::vec3>(
		"Vec3",
		sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		"lerp", [](glm::vec3 a, glm::vec3 b, float t) { return glm::mix(a, b, t); }
	);

	sol::usertype<glm::vec4> vec4 = target.new_usertype<glm::vec4>(
		"Vec4",
		sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"a", &glm::vec4::a,
		"lerp", [](glm::vec4 a, glm::vec4 b, float t) { return glm::mix(a, b, t); }
	);
}

int main() {
	// set working path
	std::filesystem::current_path("./test/");

	// initialize renderer & input
	Result<Renderer> renderer_result = Renderer::create();
	if (renderer_result.consume(std::cout)) {
		glfwTerminate();
		return 0;
	}
	Renderer& renderer = renderer_result.unwrap();
	Input input(renderer.get_window_ptr());

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
	Input::export_type(vm);
	Audio::export_type(vm);
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
	vm["Input"] = &input;

	// start the main loop
	auto last_frame_time = std::chrono::steady_clock::now();
	while (renderer.wants_next_frame())
	{
		input.update_keymap();

		auto frame_time = std::chrono::steady_clock::now();
		auto frame_dt = std::chrono::duration_cast<std::chrono::milliseconds>(frame_time - last_frame_time);
		vm["G_DT"] = frame_dt.count() / 1000.0f;
		auto result = vm["Update"]();
		last_frame_time = frame_time;

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