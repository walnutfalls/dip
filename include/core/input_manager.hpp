#pragma once


#include <cstdint>

#include <util/running_average.hpp>

#define GLFW_INCLUDE_NONE
#include <boost/signals2.hpp>
#include <GLFW/glfw3.h>

#include <Eigen/Core>


#define THUMBPAD_EPSILON 0.3f

namespace core
{
    class input_manager
    {
        std::uint8_t _glfw_mouse_button_codes[16];
        std::uint16_t _glfw_key_codes[128];

    public:
        std::unordered_map<int, boost::signals2::signal<void()>> KeyEvents;

        explicit input_manager(GLFWwindow *window);

        [[nodiscard]] bool is_key_down(std::uint16_t key) const;
        [[nodiscard]] bool was_key_pressed(std::uint16_t key) const;
        [[nodiscard]] bool was_key_released(std::uint16_t key) const;
        [[nodiscard]] bool was_mouse_clicked(std::uint8_t mouse_button) const;
        [[nodiscard]] bool was_mouse_released(std::uint8_t mouse_button) const;

        void update();

        Eigen::Vector2f mouse_delta() { return _mouse_delta.average(); }

        void skip_frame() { _skip_frame = true; }
        void set_mouse_disabled(const bool value) { _is_mouse_disabled = value; }

    private:
        GLFWwindow *_window;
        double _last_x, _last_y;
        util::running_average<Eigen::Vector2f, float> _mouse_delta;

        int* _current_key_states;
        int* _last_key_states;

        int* _current_mouse_button_states;
        int* _last_mouse_button_states;

        int _key_state_buffer_a[GLFW_KEY_LAST]{};
        int _key_state_buffer_b[GLFW_KEY_LAST]{};
        int _mouse_button_state_a[GLFW_MOUSE_BUTTON_8 +1]{};
        int _mouse_button_state_b[GLFW_MOUSE_BUTTON_8 +1]{};

        Eigen::Vector2f _last_click_position;

        bool _skip_frame{};
        bool _is_mouse_disabled{};

        static void (*build_mouse_callback(input_manager& r))(GLFWwindow* window, int, int, int)
        {
            static input_manager& manager = r;

            void (*callback)(GLFWwindow*, int, int, int) = ([](GLFWwindow* window, int button, int action, int mods) {
                (void)button;
                (void)action;
                (void)mods; // silence unused param warnings

                double x, y;
                glfwGetCursorPos(window, &x, &y);
                manager._last_click_position = {static_cast<float>(x), static_cast<float>(y)};
            });

            return callback;
        }

        
    };
}

