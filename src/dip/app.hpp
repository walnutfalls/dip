#pragma once

#include <string>
#include <core/glfw_context.hpp>
#include <core/input_manager.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "connectivity.hpp"
#include "ui.hpp"
#include "ccl.hpp"
#include "command_interpreter.hpp"

namespace dip {
    struct app_state {
        app_state();

        cv::Mat mat1;
        cv::Mat mat2;
        cv::Mat output;
    };

    class app {
    public:
        explicit app(core::glfw_context &context, core::input_manager &input);

        app_state &state() { return _state; }

        boost::signals2::signal<void(const cv::Mat &)> aChanged;
        boost::signals2::signal<void(const cv::Mat &)> bChanged;
        boost::signals2::signal<void(const cv::Mat &)> resultChanged;

        [[nodiscard]] ui &ui() { return _ui; }

    private:
        dip::ui _ui;

        app_state _state{};

        cv::Mat &mat1{_state.mat1};
        cv::Mat &mat2{_state.mat2};
        cv::Mat &output{_state.output};

        std::unordered_map<connectivity, dip::ccl> label_makers{
            {four, {N4_connectivity, false, 0.5f}},
            {eight, {N8_connectivity, false, 0.5f}},
            {m, {N4_connectivity, true, 0.5f}},
        };

        command_interpreter interpreter{mat1, output};

        boost::signals2::connection ppm1Listener{
            _ui.ppm1Changed.connect([&](const std::string &path) {
                mat1 = cv::imread(path, cv::IMREAD_COLOR);
                aChanged(mat1);
            })
        };

        boost::signals2::connection ppm2Listener{
            _ui.ppm2Changed.connect([&](const std::string &path) {
                mat2 = cv::imread(path, cv::IMREAD_COLOR);
                bChanged(mat2);
            })
        };

        boost::signals2::connection saveListener{
            _ui.saveClicked.connect([&](const std::string &path) {
                cv::imwrite(path, output);
            })
        };

        boost::signals2::connection cmdListener{
            _ui.commandIssued.connect([this](const std::string &cmd) {
                this->handle_cmd(cmd);
            })
        };

        boost::signals2::connection opListener{
            _ui.opChanged.connect([this](operation op) {
                this->handle_op(op);
            })
        };

        std::vector<boost::signals2::connection> _listeners;

        void handle_op(operation op);
        void handle_cmd(const std::string &cmd);
    };
}
