#pragma once

#include <GLFW/glfw3.h>

#include <boost/signals2.hpp>
#include "operation.hpp"


namespace dip {


    enum connectivity {
        four,
        eight,
        m,
    };

    class ui {
    public:
        explicit ui(GLFWwindow* window);
        ~ui();

        void draw();
        void drawControls();
        void drawConsole();

        bool my_tool_active{};
        float my_color[4]{};
        bool bilinear{false};

        std::string ppm1;
        std::string ppm2;

        boost::signals2::signal<void(const std::string&)> ppm1Changed;
        boost::signals2::signal<void(const std::string&)> ppm2Changed;
        boost::signals2::signal<void(operation)> opChanged;
        boost::signals2::signal<void(const std::string&)> saveClicked;
        boost::signals2::signal<void(const std::string&)> commandIssued;

        operation operation { operation::split };
        float log_c {1.f};
        float log_base {2.f};
        float gamma_val {1};
        float gamma_c {1};
        float ccl_sensitivity { 0.5f };
        connectivity connectivity { connectivity::four };
        char command[1024]{};
        std::string history[3];
        std::string fullhist;
        GLFWwindow *_window;
    };
}

