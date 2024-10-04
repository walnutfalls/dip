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

#include <GLFW/glfw3.h>

namespace dip {
    class app {
    public:
        explicit app(core::glfw_context &context, core::input_manager &input);

        cv::Mat &a() { return mat1; }
        cv::Mat &b() { return mat2; }
        cv::Mat &result() { return output; }

        boost::signals2::signal<void(const cv::Mat &)> aChanged;
        boost::signals2::signal<void(const cv::Mat &)> bChanged;
        boost::signals2::signal<void(const cv::Mat &)> resultChanged;

        [[nodiscard]] ui &ui() { return _ui; }

    private:
        dip::ui _ui;

        std::string img1{cv::samples::findFile("assets/ppm/apple-20.ppm")};
        std::string img2{cv::samples::findFile("assets/ppm/cup-10.ppm")};

        cv::Mat mat1{cv::imread(img1, cv::IMREAD_COLOR)};
        cv::Mat mat2{cv::imread(img2, cv::IMREAD_COLOR)};
        cv::Mat output;

        std::unordered_map<connectivity, dip::ccl> label_makers{
            {four, {N4_connectivity, false, 0.5f}},
            {eight, {N8_connectivity, false, 0.5f}},
            {m, {N4_connectivity, true, 0.5f}},
        };

        command_interpreter interpreter{mat1, output};

        boost::signals2::connection tildeListener;

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
            _ui.commandIssued.connect([&](const std::string &cmd) {
                try {
                    if (const auto parsed_command = interpreter.interpret_command(cmd)) {
                        _ui.operation = *parsed_command;
                    }
                    if (interpreter.get_command()[0] == "cd") {
                        _ui.history[0] = "cwd: " + interpreter.get_command()[1];
                    }
                    if (interpreter.get_command()[0] == "ls") {
                        _ui.history[0] = interpreter.out_text();
                    }

                    aChanged(mat1);
                    bChanged(mat2);
                    resultChanged(output);
                } catch (const std::exception &e) {
                    _ui.write_output(e.what());
                }
            })
        };


        boost::signals2::connection opListener = _ui.opChanged.connect([&](const operation op) {
            const auto max_width = std::max(mat1.cols, mat2.cols);
            const auto max_height = std::max(mat1.rows, mat2.rows);

            cv::Mat a, b;
            cv::copyMakeBorder(mat1, a, 0, max_height - mat1.rows, 0, max_width - mat1.cols, cv::BORDER_CONSTANT, 0);
            cv::copyMakeBorder(mat2, b, 0, max_height - mat2.rows, 0, max_width - mat2.cols, cv::BORDER_CONSTANT, 0);

            switch (op) {
                case add: {
                    interpreter.add(a, b);
                    resultChanged(output);
                }
                break;
                case sub: {
                    interpreter.sub(a, b);
                    resultChanged(output);
                }
                break;
                case mul: {
                    interpreter.mul(a, b);
                    resultChanged(output);
                }
                break;
                case negate: {
                    interpreter.inv(a);
                    resultChanged(output);
                }
                break;
                case log: {
                    interpreter.log(a, _ui.log_base, _ui.log_c);
                    resultChanged(output);
                }
                break;
                case gamma: {
                    interpreter.pow(a, _ui.gamma_val, _ui.gamma_c);
                    resultChanged(output);
                }
                break;
                case label: {
                    if (const auto it = label_makers.find(_ui.connectivity); it != label_makers.end()) {
                        it->second.set_sensitivity(_ui.ccl_sensitivity);
                        output = it->second.build_labels_map(a);
                        resultChanged(output);
                    }
                }
                break;
                default:
                    break;
            }
        });
    };
}
