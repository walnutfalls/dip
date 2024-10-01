#pragma once

#include <cstdint>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "startup_config.hpp"

namespace core
{

	class glfw_context
	{
	public:
		explicit glfw_context(startup_config& conf);
		~glfw_context();

		GLFWwindow* window();

		void swap_buffers() const;

		[[nodiscard]] const GLFWwindow* window() const;
		[[nodiscard]] std::uint32_t width() const;
		[[nodiscard]] std::uint32_t height() const;
		[[nodiscard]] bool is_minimized() const;
		[[nodiscard]] bool is_fullscreen() const;

		void set_should_close(bool) const;
		void set_minimize_callback(GLFWwindowiconifyfun) const;
		void set_fullscreen(bool);

	private:
		GLFWwindow* _window;
		GLFWmonitor* _monitor;
		const GLFWvidmode* _mode;
		startup_config& _conf;

		std::uint32_t _width;
		std::uint32_t _height;
	};

}
