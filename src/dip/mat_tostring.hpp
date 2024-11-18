#pragma once
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>

template <typename Tp, unsigned int Tcn>
std::string matToString(const cv::Mat& mat) {
    std::stringstream ss;
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            for (int k = 0; k < mat.channels(); ++k) {
                ss << mat.at<cv::Vec<Tp, Tcn>>(i, j)[k];
                ss << " ";
            }
            ss << ", ";
        }
        ss << "\n";
    }
    return ss.str();
}