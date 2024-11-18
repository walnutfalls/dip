#ifndef __CURSOR_STATE_HPP_
#define __CURSOR_STATE_HPP_


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace core
{
	class cursor_state
	{
	public:
		explicit cursor_state(GLFWwindow* window);

		void enable() const;
		void disable() const;

		void push_enable();
		void push_disable();
		void pop() const;

	private:
		GLFWwindow* _window;
		int _previous_state {GLFW_CURSOR_NORMAL};
	};
}


#endif