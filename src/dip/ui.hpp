#pragma once

#include <GLFW/glfw3.h>

#include <boost/signals2.hpp>
#include "operation.hpp"

#include <filesystem>
#include <imgui.h>


namespace dip {
    enum connectivity {
        four,
        eight,
        m,
    };

    enum histogram_op {
        a,
        b,
        output,
        match
    };


    class ui {
        constexpr static size_t HistoryLength = 16;

    public:
        explicit ui(GLFWwindow *window);

        ~ui();

        void draw();

        void drawControls(int width, int height);
        void drawControls2(int width, int height);

        int InputTextCallback(ImGuiInputTextCallbackData *data);

        void drawConsole(int width, int height);

        void drawHistograms();

        void drawFilter();


        void set_command(const std::string &cmd);

        [[nodiscard]] std::string get_command() const { return command; }

        void write_output(const std::string &output);

        bool bilinear{false};

        std::string ppm1;
        std::string ppm2;

        boost::signals2::signal<void(const std::string &)> ppm1Changed;
        boost::signals2::signal<void(const std::string &)> ppm2Changed;
        boost::signals2::signal<void(operation)> opChanged;
        boost::signals2::signal<void(const std::string &)> saveClicked;
        boost::signals2::signal<void(const std::string &)> commandIssued;

        boost::signals2::signal<void(histogram_op)> histogram_equalize;

        bool split_view{true};
        operation operation{add};
        float log_c{1.f};
        float log_base{2.f};
        float gamma_val{1};
        float gamma_c{1};
        float ccl_sensitivity{0.5f};
        connectivity connectivity{four};

        float histogramA[3][256]{};
        float histogramB[3][256]{};
        float histogramOut[3][256]{};

        int console_visible{true};

        int kernel_size{3};

        std::string history[HistoryLength]{
            "cwd: " + std::filesystem::current_path().string(),
            "Available commands:",
            "ls",
            "cd <path>",
            "[load|save] <ppm_path>",
            "[add|sum|mul] -i <ppm_path1> <ppm_path2> -i <out_ppm_path>",
            "inv -i <ppm_path1> -o <out_ppm_path>",
            "log -i <ppm_path1> -o <out_ppm_path> -c <const> -b <base>",
            "pow -i <ppm_path1> -o <out_ppm_path> -c <const> -gamma <gamma>"
        };

        std::string _full_history;
        GLFWwindow *_window;

    private:
        char command[1024]{'\0'};


        bool _dirty{false};
    };
}
