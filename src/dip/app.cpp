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

std::vector<cv::Mat> dip::app_state::calc_hist_rgb(const cv::Mat &image) {
    std::vector<cv::Mat> hist_rgb(mat1.channels());
    std::vector<cv::Mat> bgr_planes(mat1.channels());

    cv::split(image, &bgr_planes[0]);

    for (size_t i = 0; i < mat1.channels(); i++) {
        hist_rgb[i] = my_calc_hist(bgr_planes[i]);
        //cv::calcHist( &bgr_planes[i], 1, 0, cv::Mat(), hist_rgb[i], 1, &histSize, histRange, uniform, accumulate );
    }
    return hist_rgb;
}

cv::Mat dip::app_state::make_histogram_image(const std::vector<cv::Mat> &hist_rgb) const {
    constexpr int histSize = 256;
    constexpr int hist_w = 1024;
    constexpr int hist_h = 1024;

    const int bin_w = cvRound(static_cast<double>(hist_w) / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    for (size_t i = 0; i < mat1.channels(); i++) {
        cv::normalize(hist_rgb[i], hist_rgb[i], 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    }

    cv::Scalar colors[4] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 255},
    };

    for (int i = 1; i < histSize; i++) {
        for (int j = 0; j < hist_rgb.size(); j++) {
            cv::line(
                histImage,
                cv::Point(bin_w * (i - 1), hist_h - cvRound(hist_rgb[j].at<float>(i - 1))),
                cv::Point(bin_w * (i), hist_h - cvRound(hist_rgb[j].at<float>(i))),
                colors[j],
                2,
                8,
                0);
        }
    }

    return histImage;
}

cv::Mat dip::app_state::my_calc_hist(const cv::Mat &mono_input) {
    cv::Mat hist(256, 1, CV_32F, cv::Scalar(0.0f));

    for (int row = 0; row < mono_input.rows; row++) {
        for (int col = 0; col < mono_input.cols; col++) {
            const std::uint8_t intensity = mono_input.at<std::uint8_t>(row, col);
            hist.at<float>(intensity) += 1;
        }
    }

    return hist;
}


dip::app::app(core::glfw_context &context, core::input_manager &input)
    : _ui(context.window()) {
    _listeners.push_back(input.KeyEvents[GLFW_KEY_TAB].connect([&]() {
        std::string sug = interpreter.next_suggested(_ui.get_command());
        _ui.set_command(sug);
    }));

    compute_histograms();
}

void dip::app::handle_op(const operation op) {
    const auto max_width = std::max(_state.mat1.cols, _state.mat2.cols);
    const auto max_height = std::max(_state.mat1.rows, _state.mat2.rows);

    cv::Mat a, b;
    cv::copyMakeBorder(_state.mat1, a, 0, max_height - _state.mat1.rows, 0, max_width - _state.mat1.cols, cv::BORDER_CONSTANT, 0);
    cv::copyMakeBorder(_state.mat2, b, 0, max_height - _state.mat2.rows, 0, max_width - _state.mat2.cols, cv::BORDER_CONSTANT, 0);

    switch (op) {
        case add: {
            interpreter.add(a, b);
            resultChanged(_state.output);
        }
        break;
        case sub: {
            interpreter.sub(a, b);
            resultChanged(_state.output);
        }
        break;
        case mul: {
            interpreter.mul(a, b);
            resultChanged(_state.output);
        }
        break;
        case negate: {
            interpreter.inv(a);
            resultChanged(_state.output);
        }
        break;
        case log: {
            interpreter.log(a, _ui.log_base, _ui.log_c);
            resultChanged(_state.output);
        }
        break;
        case gamma: {
            interpreter.pow(a, _ui.gamma_val, _ui.gamma_c);
            resultChanged(_state.output);
        }
        break;
        case label: {
            if (const auto it = label_makers.find(_ui.connectivity); it != label_makers.end()) {
                it->second.set_sensitivity(_ui.ccl_sensitivity);
                _state.output = it->second.build_labels_map(a);
                resultChanged(_state.output);
            }
        }
        break;
        default:
            break;
    }

    compute_histograms();
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

        compute_histograms();
        aChanged(_state.mat1);
        bChanged(_state.mat2);
        resultChanged(_state.output);
    } catch (const std::exception &e) {
        _ui.write_output(e.what());
    }
}

void dip::app::compute_histograms() {
    _state.mat1_hist_rgb = _state.calc_hist_rgb(_state.mat1);
    _state.mat2_hist_rgb = _state.calc_hist_rgb(_state.mat2);
    _state.out_hist_rgb = _state.calc_hist_rgb(_state.output);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 256; j++) {
            _ui.histogramA[i][j] = _state.mat1_hist_rgb[i].at<float>(j) / 256;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 256; j++) {
            _ui.histogramB[i][j] = _state.mat2_hist_rgb[i].at<float>(j) / 256;
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 256; j++) {
            _ui.histogramOut[i][j] = _state.out_hist_rgb[i].at<float>(j) / 256;
        }
    }
}

void dip::app::handle_equalize(histogram_op target) {
    switch (target) {
        case a:
            equalize(_state.mat1, _state.mat1_hist_rgb);
            aChanged(_state.mat1);
        break;
        case b:
            equalize(_state.mat2, _state.mat2_hist_rgb);
            bChanged(_state.mat2);
            break;
        case histogram_op::output:
            equalize(_state.output, _state.out_hist_rgb);
            resultChanged(_state.output);
            break;
        case match:
            hist_match_a_b();
            aChanged(_state.mat1);
            resultChanged(_state.output);
        break;
        default: ;
    }

    compute_histograms();
}

void dip::app::hist_match_a_b() {
    compute_histograms();

    _state.output = _state.mat1;
    equalize(_state.output, _state.mat1_hist_rgb);

    const float num_pixels = static_cast<float>(_state.mat2.rows) * static_cast<float>(_state.mat2.cols);
    std::vector<cv::Mat> G_inv(_state.mat2_hist_rgb.size());
    std::transform(_state.mat2_hist_rgb.begin(), _state.mat2_hist_rgb.end(), G_inv.begin(), [&](const cv::Mat &hist) {
       return eq_histogram_inv(hist, num_pixels);
    });

    _state.output.forEach<cv::Vec3b>([&](cv::Vec3b &color, const int* position) -> void {
        for (int i = 0; i < 3; i++) {
            auto s = color[i];

            const float value = G_inv[i].at<float>(s);

            if (value != 0) {
                color[i] = static_cast<uchar>(value);
            } else if (color[i] != 0) {
                G_inv_fill_interpolate<uchar>(color[i], G_inv[i]);
            }
        }
    });

    compute_histograms();
}

void dip::app::equalize(cv::Mat &image, const std::vector<cv::Mat> &histograms) {
    const auto num_pixels = static_cast<float>(image.rows * image.cols);

    std::vector<cv::Mat> T(histograms.size());
    std::transform(histograms.begin(), histograms.end(), T.begin(), [&](const cv::Mat &hist) {
       return eq_histogram(hist, num_pixels);
    });

    image.forEach<cv::Vec3b>([&](cv::Vec3b &color, const int* position) -> void {
        color[0] = cvRound(T[0].at<float>(color[0], 0));
        color[1] = cvRound(T[1].at<float>(color[1], 0));
        color[2] = cvRound(T[2].at<float>(color[2], 0));
    });
}

cv::Mat dip::app::eq_histogram(cv::Mat hist, float num_pixels) {
    cv::Mat T = cv::Mat::zeros(hist.rows, hist.cols, CV_32F);

    hist = hist * (1.f / num_pixels);

    float sum = 0;
    for (int i = 0; i < hist.rows; i++) {
        sum += hist.at<float>(i);
        T.at<float>(i) = std::roundf(sum * 255);
    }

    return T;
}


cv::Mat dip::app::eq_histogram_inv(cv::Mat hist, const float num_pixels) {
    cv::Mat T = eq_histogram(hist, num_pixels);
    cv::Mat G (hist.rows, hist.cols, CV_32F, cv::Scalar(0));

    T.forEach<float>([&](float &color, const int* position) -> void {
        G.at<float>(static_cast<int>(color)) = static_cast<float>(*position);
    });

    return G;
}
