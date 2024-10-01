#pragma once

#include <filesystem>
#include <GLFW/glfw3.h>

#include <boost/signals2.hpp>



namespace dip {
    enum operation {
        split,
        add,
        sub,
        mul,
        negate,
        log,
        gamma
    };

    class ui {
    public:
        explicit ui(GLFWwindow* window);
        ~ui();

        void draw();

        bool my_tool_active{};
        float my_color[4]{};
        bool bilinear{false};

        std::string ppm1;
        std::string ppm2;

        boost::signals2::signal<void(const std::string&)> ppm1Changed;
        boost::signals2::signal<void(const std::string&)> ppm2Changed;
        boost::signals2::signal<void(operation)> opChanged;

        operation operation { operation::split };
        float log_c {1.f};
        float gamma_val {1};
        float gamma_c {1};
    };
}

