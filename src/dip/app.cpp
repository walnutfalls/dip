#include "app.hpp"

#include <GLFW/glfw3.h>
#include <filesystem>
#include <cstdio>


dip::app_state::app_state() {
    const std::string img1{cv::samples::findFile("assets/ppm/apple-20.ppm")};
    const std::string img2{cv::samples::findFile("assets/ppm/cup-10.ppm")};
    mat1 = cv::imread(img1, cv::IMREAD_COLOR);
    mat2 = cv::imread(img2, cv::IMREAD_COLOR);
}

dip::app::app(core::glfw_context& context, core::input_manager& input)
    : _ui(context.window()) {
    _listeners.push_back(input.KeyEvents[GLFW_KEY_TAB].connect([&]() {
        std::string sug = interpreter.next_suggested(_ui.get_command());
        _ui.set_command(sug);
    }));
}

void dip::app::handle_op(const operation op) {
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
}

void dip::app::handle_cmd(const std::string &cmd) {
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
}
