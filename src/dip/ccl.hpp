#pragma once

#include <opencv2/opencv.hpp>
#include <random>


namespace dip {
    class ccl {
    public:
        ccl(
            const std::vector<std::pair<int, int> > &connectivity,
            const bool
            adjacency_check,
            const float sensitivity)
            : _connectivity(connectivity)
              , _sensitivity(sensitivity)
              , _rng(std::random_device{}())
              , _adjacency_check(adjacency_check) {
        }

        cv::Mat build_labels_map(const cv::Mat &image);

        void set_sensitivity(float ccl_sensitivity);

    private:
        std::reference_wrapper<const std::vector<std::pair<int, int> >> _connectivity;
        float _sensitivity;
        std::mt19937 _rng;
        cv::Vec3b _label = rand_color();
        bool _adjacency_check{false};

        [[nodiscard]] bool is_foreground(
            const cv::Vec3b &pixel) const;

        void traverse_region(
            const cv::Mat &image,
            cv::Mat &labels,
            std::pair<int, int> coord) const;


        cv::Vec3b rand_color();
    };
}
