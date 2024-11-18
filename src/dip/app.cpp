#include "app.hpp"

#include <GLFW/glfw3.h>
#include <filesystem>
#include <cstdio>

#include <cmath>

#include "fft.hpp"
#include "mat_tostring.hpp"


namespace dip {
    void ft_tests() {
        {
            std::vector<std::complex<double>> data = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}};
            std::vector<float> data2 = { 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0 };
            //four1(data2, 8, 1);
            four1(data, 1);
        }
        {
            cv::Mat samples = cv::Mat(1, 5, CV_32F);
            samples.at<float>(0) = 1;
            samples.at<float>(1) = 2;
            samples.at<float>(2) = 3;
            samples.at<float>(3) = 4;
            samples.at<float>(4) = 5;

            cv::Mat dftResult;
            cv::dft(samples, dftResult, cv::DFT_COMPLEX_OUTPUT);
            cv::Mat out = dft_fwd_1d(samples);
            std::string a = matToString<float, 2>(out);
            std::string b = matToString<float, 2>(dftResult);
            cv::Mat recovered = dft_bak_1d(out);
            cv::Mat x;
            cv::idft(out, x, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
            std::string c = matToString<float, 1>(recovered);
            std::string d = matToString<float, 1>(x);

            std::cout << a << "\n" << b << "\n" << c << "\n" << d << std::endl;
        }

        {
            cv::Mat matrix = (cv::Mat_<float>(5, 5) <<  1,  2,  3,  4,  5,
                                                   6,  7,  8,  9, 10,
                                                  11, 12, 13, 14, 15,
                                                  16, 17, 18, 19, 20,
                                                  21, 22, 23, 24, 25);

            cv::Mat matrix_dft = dft_fwd_2d(matrix);
            std::string e = matToString<float, 2>(matrix_dft);

            cv::Mat matrix_dft_bak = dft_bak_2d(matrix_dft);
            std::string f = matToString<float, 1>(matrix_dft_bak);

            cv::Mat sep_dft = dft_fwd_2d_separable(matrix);
            std::string g = matToString<float, 2>(sep_dft);

            cv::Mat sep_dft_bak = dft_bak_2d(sep_dft);
            std::string h = matToString<float, 1>(sep_dft_bak);

            cv::Mat sep_fft = fft_2d(matrix);
            std::string i = matToString<float, 2>(sep_dft);

            cv::Mat sep_fft_bak = dft_bak_2d(sep_fft);
            std::string j = matToString<float, 1>(sep_dft_bak);

            std::cout << e << "\n" << f << "\n" << g << "\n" << h << "\n" << i << "\n" << j << std::endl;
        }

        {
            cv::Mat matrix = (cv::Mat_<float>(4, 4) <<  10,  10,  15,  15,
                                                   10,  15,  10,  15,
                                                  10, 10, 15, 15,
                                                  10, 10, 10, 15);

            cv::Mat matrix_dft = dft_fwd_2d(matrix);
            std::string e = matToString<float, 2>(matrix_dft);

            cv::Mat matrix_dft_bak = dft_bak_2d(matrix_dft);
            std::string f = matToString<float, 1>(matrix_dft_bak);

            cv::Mat sep_dft = dft_fwd_2d_separable(matrix);
            std::string g = matToString<float, 2>(sep_dft);

            cv::Mat sep_dft_bak = dft_bak_2d(sep_dft);
            std::string h = matToString<float, 1>(sep_dft_bak);


            cv::Mat sep_fft = fft_2d(matrix);
            std::string i = matToString<float, 2>(sep_dft);

            cv::Mat sep_fft_bak = dft_bak_2d(sep_fft);
            std::string j = matToString<float, 1>(sep_dft_bak);


            std::cout << e << "\n" << f << "\n" << g << "\n" << h << std::endl;
        }
    }
}

const double e = std::exp(1.0);



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

    ///////////
    ft_tests();
    /////////////
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
                compute_histograms();
            }
        }
        case fourier: {
            interpreter.fourier(_state.mat1, _state.mat2, _ui.fft_bins);

            // cv::Mat f_input;
            // cv::Mat planes[3];
            // cv::split(a, planes);
            // planes[0].convertTo(f_input, CV_32F);
            //
            //
            // const int M = next_pow_2(std::max(f_input.rows, f_input.cols));
            // const int top = (M - f_input.rows) / 2;
            // const int bottom = M - f_input.rows - top;
            // const int left = (M - f_input.cols) / 2;
            // const int right = M - f_input.cols - left;
            // cv::copyMakeBorder(f_input, f_input, top, bottom, left, right, cv::BORDER_WRAP);
            //
            // for (int i = 0; i < 10; i++) {
            //     if (f_input.rows < 2) break;
            //
            //     std::cout << f_input.rows << "Scale down: " << i << std::endl;
            //
            //     {
            //         auto start = std::chrono::high_resolution_clock::now();
            //         auto dft = dft_fwd_2d(f_input);
            //         auto end = std::chrono::high_resolution_clock::now();
            //         auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            //         std::cout << "DFT_FWD: " << duration.count() << " microseconds" << std::endl;
            //
            //         start = std::chrono::high_resolution_clock::now();
            //         dft_bak_2d(dft);
            //         end = std::chrono::high_resolution_clock::now();
            //         duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            //         std::cout << "DFT_BAK: " << duration.count() << " microseconds" << std::endl;
            //     }
            //
            //     {
            //         cv::Mat in = cv::Mat::zeros(a.rows, a.cols, CV_32F);
            //
            //
            //         auto start = std::chrono::high_resolution_clock::now();
            //         auto dft = dft_fwd_2d_separable(in);
            //         auto end = std::chrono::high_resolution_clock::now();
            //         auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            //         std::cout << "DFT_FWD_SEP: " << duration.count() << " microseconds" << std::endl;
            //     }
            //
            //     {
            //         auto start = std::chrono::high_resolution_clock::now();
            //         auto dft = fft_2d(f_input);
            //         auto end = std::chrono::high_resolution_clock::now();
            //         auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            //         std::cout << "FFT: " << duration.count() << " microseconds" << std::endl;
            //
            //         start = std::chrono::high_resolution_clock::now();
            //         ifft_2d(dft);
            //         end = std::chrono::high_resolution_clock::now();
            //         duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            //         std::cout << "IFFT: " << duration.count() << " microseconds" << std::endl;
            //     }
            //
            //     cv::resize(f_input, f_input, cv::Size(f_input.rows / 2, f_input.cols / 2), 0, 0, cv::INTER_LINEAR);
            // }




            bChanged(_state.mat2);
            resultChanged(_state.output);

            compute_histograms();
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

    _state.output = _state.mat1.clone();
    equalize(_state.output, _state.mat1_hist_rgb);

    const float num_pixels = static_cast<float>(_state.mat2.rows) * static_cast<float>(_state.mat2.cols);
    std::vector<cv::Mat> G_inv(_state.mat2_hist_rgb.size());
    std::transform(_state.mat2_hist_rgb.begin(), _state.mat2_hist_rgb.end(), G_inv.begin(), [&](const cv::Mat &hist) {
       return eq_histogram_inv(hist, num_pixels);
    });

    _state.output.forEach<cv::Vec3b>([&](cv::Vec3b &color, const int* position) -> void {
        for (int i = 0; i < 3; i++) {
            color[i] = static_cast<uchar>(G_inv[i].at<float>(color[i]));
        }
    });

    compute_histograms();
}

void dip::app::run_gaussian(const int kernel_size, const float stddev) {
    cv::Mat kernel = gaussian_kernel(kernel_size, stddev);
    const int a = kernel_size / 2;

    _state.output = cv::Mat::zeros(_state.mat1.rows, _state.mat1.cols, CV_8UC3);

    cv::Mat padded (_state.mat1.rows, _state.mat1.cols, CV_8UC3);
    cv::copyMakeBorder(_state.mat1, padded, a, a, a, a, cv::BORDER_DEFAULT);

    padded.forEach<cv::Vec3b>([&](cv::Vec3b &color, const int* position) -> void {
        if (position[0] < a || position[0] > _state.mat1.cols ) {
            return;
        }

        if (position[1] < a || position[1] > _state.mat1.rows ) {
            return;
        }

        cv::Vec3b value;
        for (int x = -a; x < a; x++) {
            for (int y = -a; y < a; y++) {
                const auto& pixel = padded.at<cv::Vec3b>(position[0] + x, position[1] + y);
                value += kernel.at<float>(x + a, y + a) * pixel;
            }
        }

         _state.output.at<cv::Vec3b>(position[0] - a, position[1] - a) = value;
    });

    compute_histograms();
    resultChanged(_state.output);
    aChanged(padded);
}

void dip::app::run_sobel(int c) {
    auto [fst, snd] = sobel_kernels(c);
    const cv::Mat x_conv = convolve(_state.mat1, fst);
    const cv::Mat y_conv = convolve(_state.mat1, snd);
    const cv::Mat sum = x_conv + y_conv;

    double minVal, maxVal;
    cv::minMaxLoc(sum, &minVal, &maxVal);
    sum.convertTo(_state.output, CV_8UC3, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

    compute_histograms();
    resultChanged(_state.output);
}

void dip::app::run_unsharp(int k, int kernel_size, float stddev) {
    run_gaussian(kernel_size, stddev);
    cv::Mat mask = _state.mat1 - _state.output;
    _state.output = _state.mat1 + (mask * k);
    compute_histograms();
    resultChanged(_state.output);
}

cv::Mat dip::app::convolve(const cv::Mat &img, cv::Mat kernel) {
    cv::flip(kernel, kernel, 0);

    const int a = kernel.rows / 2;

    cv::Mat result = cv::Mat::zeros(img.rows, img.cols, CV_16SC3);

    cv::Mat padded;
    cv::copyMakeBorder(img, padded, a, a, a, a, cv::BORDER_DEFAULT);

    padded.forEach<cv::Vec3b>([&](cv::Vec3b &color, const int* position) -> void {
        if (position[0] < a || position[0] > img.cols) {
            return;
        }

        if (position[1] < a || position[1] > img.rows) {
            return;
        }

        cv::Vec3s value;
        for (int x = -a; x < a; x++) {
            for (int y = -a; y < a; y++) {
                const cv::Vec3s pixel = padded.at<cv::Vec3b>(position[0] + x, position[1] + y);
                value += kernel.at<float>(x + a, y + a) * pixel;
            }
        }

         result.at<cv::Vec3s>(position[0] - a, position[1] - a) = value;
    });

    return result;
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

    cv::Mat p = hist * (1.f / num_pixels);

    std::cout << p << std::endl;

    float sum = 0;
    for (int i = 0; i < p.rows; i++) {
        sum += p.at<float>(i);
        T.at<float>(i) = std::roundf(sum * 255);
    }

    return T;
}


cv::Mat dip::app::eq_histogram_inv(const cv::Mat &hist, const float num_pixels) {
    cv::Mat G = eq_histogram(hist, num_pixels);
    cv::Mat G_inv (hist.rows, hist.cols, CV_32F, cv::Scalar(0));

    G.forEach<float>([&](const float &color, const int* position) -> void {
        G_inv.at<float>(cvRound(color)) = static_cast<float>(*position);
    });

    for (int s = 0; s < G_inv.rows; s++) {
        if (G_inv.at<float>(s) == 0.f) {
            G_inv.at<float>(s) = G_inv_fill_interpolate(s, G_inv);
        }
    }

    return G_inv;
}

cv::Mat dip::app::gaussian_kernel(const int kernel_size, const float stddev) {
    cv::Mat kernel = cv::Mat::zeros(kernel_size, kernel_size, CV_32F);

    const float twoSigSq = 2.f * stddev * stddev;
    const float K = 1.f/static_cast<float>(CV_PI * twoSigSq);

    float sum = 0;

    const int a = kernel_size / 2;

    for (int x = -a; x < a; x++) {
        for (int y = -a; y < a; y++) {
            const float exponent = -1.f * static_cast<float>(x * x + y * y) / twoSigSq;
            const float value = K * std::expf(exponent);

            kernel.at<float>(x+a, y+a) = value;
            sum += value;
        }
    }

    for (int i = 0; i < kernel_size; i++) {
        for (int j = 0; j < kernel_size; j++) {
            kernel.at<float>(i, j) /= sum;
        }
    }

    return kernel;
}

std::pair<cv::Mat, cv::Mat> dip::app::sobel_kernels(int c) {
    std::pair kernels {
        cv::Mat(3, 3, CV_32F, cv::Scalar(0)),
        cv::Mat(3, 3, CV_32F, cv::Scalar(0))
    };

    kernels.first.at<float>(0, 0) = -1.0f;
    kernels.first.at<float>(1, 0) = -static_cast<float>(c);
    kernels.first.at<float>(2, 0) = -1.0f;
    kernels.first.at<float>(0, 2) = 1.0f;
    kernels.first.at<float>(1, 2) = static_cast<float>(c);
    kernels.first.at<float>(2, 2) = 1.0f;

    kernels.second.at<float>(0, 0) = -1.0f;
    kernels.second.at<float>(0, 1) = -static_cast<float>(c);
    kernels.second.at<float>(0, 2) = -1.0f;
    kernels.second.at<float>(2, 0) = 1.0f;
    kernels.second.at<float>(2, 1) = static_cast<float>(c);
    kernels.second.at<float>(2, 2) = 1.0f;

    return kernels;
}
