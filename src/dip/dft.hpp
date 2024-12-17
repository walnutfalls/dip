#pragma once

#include <opencv2/core.hpp>


namespace dip {
    // DFT format is CV_32FC2 - the real and imaginary coefficients.

    // 1D DFT Formula
    // F(u) = Σ (from x=0 to M-1) f(x) e^(-1 * j 2π u x / M )
    cv::Mat dft_fwd_1d(cv::Mat &samples);
    cv::Mat dft_fwd_1d_complex(cv::Mat &samples);

    // f(x) = (1/M) Σ (from u=0 to M-1) 1d_dft_fwd(x) e^(j 2π u x / M )
    cv::Mat dft_bak_1d(cv::Mat &F, bool real_only = true);

    //2D DFT Formula
    //F(u, v) = Σ (from x=0 to M-1) Σ (from y=0 to N-1) f(x, y) * e^(-j * 2π * (ux/M + vy/N))
    cv::Mat dft_fwd_2d(cv::Mat &f);
    cv::Mat dft_fwd_2d_separable(const cv::Mat& f);

    // f(x, y) = (1 / (M*N)) * Σ (from u=0 to M-1) Σ (from v=0 to N-1) F(u, v) * e^(j * 2π * (ux/M + vy/N))
    cv::Mat dft_bak_2d(cv::Mat &F, bool real_only = true);
}
