#pragma once

#include <string>

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
    app(GLFWwindow* window);

    cv::Mat& a() { return cvmat1; }
    cv::Mat& b() { return cvmat2; }
    cv::Mat& result() { return outmat; }

    boost::signals2::signal<void(const cv::Mat&)> aChanged;
    boost::signals2::signal<void(const cv::Mat&)> bChanged;
    boost::signals2::signal<void(const cv::Mat&)> resultChanged;

    [[nodiscard]] ui& ui() { return _ui; }

private:
    dip::ui _ui;

    std::string img1 { cv::samples::findFile("assets/ppm/apple-20.ppm") };
    std::string img2 { cv::samples::findFile("assets/ppm/cup-10.ppm") };

    cv::Mat cvmat1 { cv::imread(img1, cv::IMREAD_COLOR) };
    cv::Mat cvmat2 { cv::imread(img2, cv::IMREAD_COLOR) };
    cv::Mat outmat;

    std::unordered_map<dip::connectivity, dip::ccl> label_makers {
        {four, {dip::N4_connectivity, false, 0.5f}},
        {eight, {dip::N8_connectivity, false, 0.5f}},
        {m, {dip::N4_connectivity, true, 0.5f}},
    };

    command_interpreter interpreter { outmat };

    boost::signals2::connection ppm1Listener { _ui.ppm1Changed.connect([&](const std::string &path) {
        cvmat1 = cv::imread(path, cv::IMREAD_COLOR);
        aChanged(cvmat1);
    })};

    boost::signals2::connection ppm2Listener { _ui.ppm2Changed.connect([&](const std::string &path) {
        cvmat2 = cv::imread(path, cv::IMREAD_COLOR);
        bChanged(cvmat1);
    })};

    boost::signals2::connection saveListener { _ui.saveClicked.connect([&](const std::string &path) {
        cv::imwrite(path, outmat);
    })};

    boost::signals2::connection cmdListener { _ui.commandIssued.connect([&](const std::string &cmd) {
        if (const auto parsed_command = interpreter.interpret_command(cmd)) {
            _ui.operation = *parsed_command;
        }
        resultChanged(outmat);
    })};


    boost::signals2::connection opListener = _ui.opChanged.connect([&](const operation op) {
        const auto max_width = std::max(cvmat1.cols, cvmat2.cols);
        const auto max_height = std::max(cvmat1.rows, cvmat2.rows);

        cv::Mat a,b;
        cv::copyMakeBorder(cvmat1,a, 0,max_height - cvmat1.rows,0,max_width - cvmat1.cols,cv::BORDER_CONSTANT,0);
        cv::copyMakeBorder(cvmat2,b,0,max_height - cvmat2.rows,0,max_width - cvmat2.cols,cv::BORDER_CONSTANT,0);

        switch (op) {
            case add: {
                interpreter.add(a, b);
                resultChanged(outmat);
            }
            break;
            case sub: {
                interpreter.sub(a, b);
                resultChanged(outmat);
            }
            break;
            case mul: {
                interpreter.mul(a, b);
                resultChanged(outmat);
            }
            break;
            case negate: {
                interpreter.inv(a);
                resultChanged(outmat);
            }
            break;
            case log: {
                interpreter.log(a, _ui.log_base, _ui.log_c);
                resultChanged(outmat);
            }
            break;
            case gamma: {
                interpreter.pow(a, _ui.gamma_val, _ui.gamma_c);
                resultChanged(outmat);
            }
            break;
            case label: {
                if (const auto it = label_makers.find(_ui.connectivity); it != label_makers.end()) {
                    it->second.set_sensitivity(_ui.ccl_sensitivity);
                    outmat = it->second.build_labels_map(a);
                    resultChanged(outmat);
                }
            }
            break;
            default:
                break;
        }
    });

};

}
