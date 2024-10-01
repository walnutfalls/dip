#include <core/cursor_state.hpp>


core::cursor_state::cursor_state(GLFWwindow* window) : _window(window)
{
}

void core::cursor_state::enable() const {
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void core::cursor_state::disable() const {
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void core::cursor_state::push_enable()
{
	_previous_state = glfwGetInputMode(_window, GLFW_CURSOR);
	enable();
}

void core::cursor_state::push_disable()
{
	_previous_state = glfwGetInputMode(_window, GLFW_CURSOR);
	disable();
}

void core::cursor_state::pop() const {
	glfwSetInputMode(_window, GLFW_CURSOR, _previous_state);
}
