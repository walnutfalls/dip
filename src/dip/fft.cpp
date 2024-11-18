#include "fft.hpp"

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

#include "mat_tostring.hpp"



int dip::next_pow_2(int n) {
    if (n == 0) return 1;
    if ((n & (n - 1)) == 0) return n;

    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    //n |= n >> 32;

    return n + 1;
}

// Adapted from Numerical Recipes
void dip::four1(std::vector<std::complex<double> > &data, int sign) {
    const auto n = data.size();
    const auto K = n / 2;
    if (n <= 1) return;

    size_t j = 0;
    for (size_t i = 0; i < n; ++i) {
        if (j > i) {
            std::swap(data[j], data[i]);
        }
        auto m = K;
        while (m >= 1 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    unsigned int fft_K = 1;
    while (n > fft_K) {
        const unsigned int fft_N = fft_K * 2;
        const double theta = -sign * (CV_2PI / fft_N);
        const double wtemp = std::sin(0.5 * theta);

        std::complex<double> wp{ -2.0 * wtemp * wtemp, std::sin(theta) };
        std::complex<double> w{1.0, 0};

        for (unsigned int k = 0; k < fft_K; k++) {
            for (unsigned int i = k; i < n; i += fft_N) {
                j = i + fft_K;
                std::complex<double> temp = w * data[j];

                data[j] = data[i] - temp;
                data[i] += temp;
            }

            w = w + w * wp;
        }
        fft_K = fft_N;
    }
}


void fft2D(std::vector<std::vector<std::complex<double>>> &data, int sign) {
    const auto N = data.size();
    const dip::twiddle_lookup W(N);

    for (int i = 0; i < N; ++i) {
        W.four1(data[i]);
    }

    for (int j = 0; j < N; ++j) {
        std::vector<std::complex<double> > column(N);

        for (int i = 0; i < N; ++i) {
            column[i] = data[i][j];
        }

        W.four1(column);

        for (int i = 0; i < N; ++i) {
            data[i][j] = column[i];
        }
    }
}

cv::Mat dip::fft_2d(const cv::Mat &image) {
    const int M = next_pow_2(std::max(image.rows, image.cols));

    const int top = (M - image.rows) / 2;
    const int bottom = M - image.rows - top;
    const int left = (M - image.cols) / 2;
    const int right = M - image.cols - left;

    cv::Mat padded_f;
    cv::copyMakeBorder(image, padded_f, top, bottom, left, right, cv::BORDER_WRAP, cv::Scalar(0));

    std::vector<std::vector<std::complex<double>>> rows(M, std::vector<std::complex<double> >(M, 0));
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < M; ++col) {
            rows[row][col] = std::complex<double>(padded_f.at<float>(row, col) ,0);
        }
    }

    fft2D(rows, 1);

    cv::Mat F_out(M, M, CV_32FC2);
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < M; ++col) {
            F_out.at<cv::Vec2f>(row, col)[0] = static_cast<float>(rows[row][col].real());
            F_out.at<cv::Vec2f>(row, col)[1] = static_cast<float>(rows[row][col].imag());
        }
    }

    return F_out;
}

cv::Mat dip::ifft_2d(cv::Mat F) {
    const int M = F.rows;

    std::vector<std::vector<std::complex<double>>> rows(M, std::vector<std::complex<double> >(M, 0));
    for (int row = 0; row < M; ++row) {
        for (int col = 0; col < M; ++col) {
            rows[row][col] = std::complex<double>(
                F.at<cv::Vec2f>(row, col)[0],
                -F.at<cv::Vec2f>(row, col)[1]);
        }
    }

    fft2D(rows, -1);

    cv::Mat f_out(M, M, CV_32F);
    f_out.forEach<float>([&](float& val, const int* pos) {
        val = static_cast<float>(rows[pos[0]][pos[1]].real());
    });

    return f_out;
}

void dip::shiftDFT(cv::Mat &mat)  {
    // Rearrange the quadrants of the Fourier image so that the origin is at the center
    int cx = mat.cols / 2;
    int cy = mat.rows / 2;

    cv::Mat q0(mat, cv::Rect(0, 0, cx, cy));    // Top-Left
    cv::Mat q1(mat, cv::Rect(cx, 0, cx, cy));   // Top-Right
    cv::Mat q2(mat, cv::Rect(0, cy, cx, cy));   // Bottom-Left
    cv::Mat q3(mat, cv::Rect(cx, cy, cx, cy));  // Bottom-Right

    // Swap quadrants (Top-Left with Bottom-Right)
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // Swap quadrants (Top-Right with Bottom-Left)
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void dip::twiddle_lookup::ensure(size_t maxN, size_t maxK) {
    static constexpr size_t padding = 16;
    maxN += padding;
    maxK += padding;

    if (maxN >= W.size()) {
        W.resize(maxN);
    }

    for (unsigned int n = 0; n < maxN; ++n) {
        W[n] = std::vector<std::complex<double>>(maxK);

        for (unsigned int k = 0; k < maxK; ++k) {
            W[n][k] = std::polar<double>(1, CV_2PI * static_cast<double>(k) / static_cast<double>(n));
        }
    }
}

void dip::twiddle_lookup::four1(std::vector<std::complex<double>> &data) const {
    const size_t n = data.size();
    const size_t K = n / 2;
    if (n <= 1) return;

    size_t j = 0;
    for (size_t i = 0; i < n; ++i) {
        if (j > i) {
            std::swap(data[j], data[i]);
        }
        size_t m = K;
        while (m >= 1 && j >= m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    size_t fft_K = 1;
    while (n > fft_K) {
        const size_t fft_N = fft_K * 2;

        for (size_t k = 0; k < fft_K; k++) {
            for (size_t i = k; i < n; i += fft_N) {
                j = i + fft_K;
                std::complex<double> temp = W[fft_N][k] * data[j];

                data[j] = data[i] - temp;
                data[i] += temp;
            }
        }
        fft_K = fft_N;
    }
}


// Numerical recipes, original
// void dip::four1(std::vector<float> &data, unsigned long nn, int isign) {
//     unsigned long n, mmax, m, j, istep, i;
//     double wtemp, wr, wpr, wpi, wi, theta;
//     float tempr, tempi;
//
//     n = nn << 1;
//     j = 1;
//
//
//     // bit reversal
//     for (i = 1; i < n; i += 2) {
//         if (j > i) {
//             std::swap(data[j], data[i]);
//             std::swap(data[j + 1], data[i + 1]);
//         }
//         m = nn;
//         while (m >= 2 && j > m) {
//             j -= m;
//             m >>= 1;
//         }
//         j += m;
//     }
//
//     // Danielson-Lanczos section
//     mmax = 2;
//     while (n > mmax) {
//         istep = mmax << 1;
//         theta = -isign * (CV_2PI / mmax);
//         wtemp = std::sin(0.5 * theta);
//         wpr = -2.0 * wtemp * wtemp;
//         wpi = std::sin(theta);
//         wr = 1.0;
//         wi = 0.0;
//         for (m = 1; m < mmax; m += 2) {
//             for (i = m; i <= n; i += istep) {
//                 j = i + mmax;
//                 tempr = wr * data[j] - wi * data[j + 1];
//                 tempi = wr * data[j + 1] + wi * data[j];
//
//                 data[j] = data[i] - tempr;
//                 data[j + 1] = data[i + 1] - tempi;
//                 data[i] += tempr;
//                 data[i + 1] += tempi;
//             }
//             wr = (wtemp = wr) * wpr - wi * wpi + wr;
//             wi = wi * wpr + wtemp * wpi + wi;
//         }
//         mmax = istep;
//     }
// }
