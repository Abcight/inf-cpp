#include <filesystem>
#include <chrono>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "audio.h"
#include "color.h"
#include "framebuffer.h"
#include "glmexport.h"

int main(int argc, char* argv[]) {
	// select the project path
	// if no path is specified, look for "game" subdirectory 
	std::string target;
	if (argc <= 1) {
		target = "./game";
	} else {
		target = std::string(argv[1]);
	}

	// set working path
	std::filesystem::current_path(target);

	// initialize the vm
	sol::state vm;
	vm.open_libraries(
		sol::lib::base,
		sol::lib::package,
		sol::lib::math,
		sol::lib::io,
		sol::lib::table,
		sol::lib::os
	);

	Renderer::export_type(vm);
	RenderCommand::export_type(vm);
	Framebuffer::export_type(vm);
	Texture::export_type(vm);
	Input::export_type(vm);
	Audio::export_type(vm);
	Color::export_type(vm);
	export_glm(vm);

	// read config
	vm.safe_script_file("config.lua", &sol::script_pass_on_error);
	int width = vm.get_or("window_width", 640);
	int height = vm.get_or("window_height", 480);
	std::string title = vm.get_or("window_title", std::string("2D Engine"));

	// initialize renderer & input
	Result<Renderer> renderer_result = Renderer::create(width, height, title);
	if (renderer_result.consume(std::cout)) {
		glfwTerminate();
		return 0;
	}
	Renderer& renderer = renderer_result.unwrap();
	Input input(renderer.get_window_ptr());

	// export engine vars and load main script file
	vm["Renderer"] = &renderer;
	vm["Input"] = &input;

	auto result = vm.safe_script_file("main.lua", &sol::script_pass_on_error);
	if (!result.valid())
	{
		sol::error err = result;
		std::cout << "Error loading main.lua:" << std::endl;
		std::cout << err.what() << std::endl;
		std::cout << "Tried to load from: " << std::filesystem::current_path() << std::endl;
		glfwSetWindowShouldClose(renderer.get_window_ptr(), true);
		glfwTerminate();
		// Only wait for input if stdin is a terminal
		if (isatty(STDIN_FILENO)) {
			std::cout << "[Press RETURN to exit]" << std::endl;
			std::cin.get();
		}
		return 1;
	}

	// perform the main loop
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