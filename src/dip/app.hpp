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

    template <typename Tch>
    void G_inv_fill_interpolate(Tch& s, const cv::Mat& G_inv) {
        // find the nearest mapping, and interpolate
        //| 4 | 0 | 0 | 0 | 0 | 9 |
        //  0   1  ^2   3   4   5
        //  range=5, we want: (1-2/5)*4 + (1-3/5) * 9
        int left = s;
        int right = s;

        while (right < G_inv.rows && G_inv.at<float>(right) == 0) {
            ++right;
        }

        while (left >= 0 && G_inv.at<float>(left) == 0) {
            --left;
        }

        const auto range = static_cast<float>(right - left);
        const float lc = 1.f - static_cast<float>(s - left)/range;
        const float rc = 1.f - static_cast<float>(right - s)/range;
        s = cvRound(lc * G_inv.at<float>(left) + rc * G_inv.at<float>(right));
    }

    struct app_state {
        app_state();

        cv::Mat mat1;
        cv::Mat mat2;
        cv::Mat output;

        std::vector<cv::Mat> mat1_hist_rgb;
        std::vector<cv::Mat> mat2_hist_rgb;
        std::vector<cv::Mat> out_hist_rgb;

        std::vector<cv::Mat> calc_hist_rgb(const cv::Mat& image);

        static cv::Mat my_calc_hist(const cv::Mat& mono_input);

        [[nodiscard]] cv::Mat make_histogram_image(const std::vector<cv::Mat>& hist_rgb) const;
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

        std::unordered_map<connectivity, dip::ccl> label_makers{
            {four, {N4_connectivity, false, 0.5f}},
            {eight, {N8_connectivity, false, 0.5f}},
            {m, {N4_connectivity, true, 0.5f}},
        };

        command_interpreter interpreter{_state.mat1, _state.output};

        boost::signals2::connection ppm1Listener{
            _ui.ppm1Changed.connect([&](const std::string &path) {
                _state.mat1 = cv::imread(path, cv::IMREAD_COLOR);
                aChanged(_state.mat1);
            })
        };

        boost::signals2::connection ppm2Listener{
            _ui.ppm2Changed.connect([&](const std::string &path) {
                _state.mat2 = cv::imread(path, cv::IMREAD_COLOR);
                bChanged(_state.mat2);
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

        boost::signals2::connection eqListener{
            _ui.histogram_equalize.connect([this](histogram_op target) {
                this->handle_equalize(target);
            })
        };

        std::vector<boost::signals2::connection> _listeners;

        std::function<void(cv::Vec3b&, const cv::Mat&)> hist_match_gap_fills[1] {
            {

            }
        };

        void handle_op(operation op);
        void handle_cmd(const std::string &cmd);
        void compute_histograms();
        void handle_equalize(histogram_op target);
        void hist_match_a_b();

        static void equalize(cv::Mat &image, const std::vector<cv::Mat> &histograms);
        static cv::Mat eq_histogram(cv::Mat hist, float num_pixels);

        static cv::Mat eq_histogram_inv(cv::Mat hist, float num_pixels);
    };
}
