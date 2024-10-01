#pragma once

#include <filesystem>
#include <GLFW/glfw3.h>


namespace dip {
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
    };
}

