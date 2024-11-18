#pragma once

#include <map>
#include <vector>
#include <opencv2/core.hpp>


namespace dip {
    // Adapted from the C-ish version to C++ vector of complex
    void four1(std::vector<std::complex<double>>& data, int sign);

    // From Numerical Recipes, 2nd edition
    //void four1(std::vector<float>& data, unsigned long nn, int sign);

    cv::Mat fft_2d(const cv::Mat &image);
    cv::Mat ifft_2d(cv::Mat F);

    void shiftDFT(cv::Mat& mat);
    int next_pow_2(int n);

    class twiddle_lookup {
    public:
        explicit twiddle_lookup(const size_t N) {
            ensure(N, N);
        }

        void ensure(size_t maxN, size_t maxK);

        void four1(std::vector<std::complex<double>>& data) const;

    private:
        std::vector<std::vector<std::complex<double>>> W;
    };
}
