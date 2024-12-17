#include "dft.hpp"
#include "mat_tostring.hpp"



// F(u) = Σ (from x=0 to M-1) f(x) e^(-1 * j 2π u x / M )
// const cv::Mat &samples expected as cv::Mat_<float>(1, M)
cv::Mat dip::dft_fwd_1d(cv::Mat &samples) {
    const int M = samples.cols;
    cv::Mat output(1, M, CV_32FC2);

    output.forEach<cv::Vec2f>([&](cv::Vec2f& F_u, const int* pos_m) {
        const int u = pos_m[1];
        F_u[0] = 0;
        F_u[1] = 0;

        for (int x = 0; x < M; x++) {
            const double theta = -2.0 * CV_PI * u * x / M;
            const float f_x = samples.at<float>(x);

            F_u[0] += static_cast<float>(f_x * std::cos(theta));
            F_u[1] += static_cast<float>(f_x * std::sin(theta));
        }
    });

    return output;
}

cv::Mat dip::dft_fwd_1d_complex(cv::Mat &f) {
    const int M = f.cols;
    cv::Mat output(1, M, CV_32FC2);

    output.forEach<cv::Vec2f>([&](cv::Vec2f& F_u, const int* pos_m) {
        const int u = pos_m[1];
        F_u[0] = 0;
        F_u[1] = 0;

        for (int x = 0; x < M; x++) {
            const double theta = -2.0 * CV_PI * u * x / M;
            const cv::Vec2f f_x = f.at<cv::Vec2f>(x);
            const float a = f_x[0];
            const float b = f_x[1];
            const auto c = std::cos(theta);
            const auto d = std::sin(theta);
            F_u[0] += static_cast<float>(a * c - b * d);
            F_u[1] += static_cast<float>(a * d + b * c);
        }
    });

    return output;
}

cv::Mat dip::dft_bak_1d(cv::Mat &F, bool real_only) {
    const int M = F.cols;
    const float M_inv = 1.f / static_cast<float>(M);

    cv::Mat f(1, M, CV_32FC2);

    f.forEach<cv::Vec2f>([&](cv::Vec2f& f_x, const int* pos_m) {
        const int n = pos_m[1];
        f_x[0] = 0;
        f_x[1] = 0;

        for (int m = 0; m < M; m++) {
            const double theta = 2.0 * CV_PI * m * n / M;
            const cv::Vec2f F_m = F.at<cv::Vec2f>(m);
            const float a = F_m[0];
            const float b = F_m[1];
            const auto c = std::cos(theta);
            const auto d = std::sin(theta);
            f_x[0] += static_cast<float>(a * c - b * d);
            f_x[1] += static_cast<float>(a * d + b * c);
        }

        f_x[0] *= M_inv;
        f_x[1] *= M_inv;
    });

    if (real_only) {
        std::vector<cv::Mat> channels;
        cv::split(f, channels);
        return channels[0];
    }

    return f;
}

cv::Mat dip::dft_fwd_2d(cv::Mat &samples) {
    const int M = samples.cols;
    const int N = samples.rows;
    cv::Mat F(N, M, CV_32FC2);

    F.forEach<cv::Vec2f>([&](cv::Vec2f& F_uv, const int* pos_m) {
        const int u = pos_m[1];
        const int v = pos_m[0];

        F_uv[0] = 0;
        F_uv[1] = 0;

        for (int x = 0; x < M; x++) {
            for (int y = 0; y < N; y++) {
                const float f_xy = samples.at<float>(y,x);
                const double theta = -2.0 * CV_PI * (
                    static_cast<float>(u * x) / static_cast<float>(M) +
                    static_cast<float>(v * y) / static_cast<float>(N));

                F_uv[0] += static_cast<float>(f_xy * std::cos(theta));
                F_uv[1] += static_cast<float>(f_xy * std::sin(theta));
            }
        }
    });

    return F;
}

cv::Mat dip::dft_bak_2d(cv::Mat &F, const bool real_only) {
    const int M = F.cols;
    const int N = F.rows;
    const auto MN_inv = static_cast<float>(1.0f / static_cast<double>(M * N));
    cv::Mat f(F.rows, F.cols, CV_32FC2);

    f.forEach<cv::Vec2f>([&](cv::Vec2f& f_xy, const int* pos_m) {
        const int y = pos_m[0];
        const int x = pos_m[1];

        f_xy[0] = 0;
        f_xy[1] = 0;

        for (int u = 0; u < M; u++) {
            for (int v = 0; v < N; v++) {
                const cv::Vec2f& F_uv = F.at<cv::Vec2f>(v,u);
                const double theta = 2.0 * CV_PI * (
                    static_cast<float>(u * x) / static_cast<float>(M) +
                    static_cast<float>(v * y) / static_cast<float>(N));

                const float a = F_uv[0];
                const float b = F_uv[1];
                const auto c = std::cos(theta);
                const auto d = std::sin(theta);
                f_xy[0] += static_cast<float>(a * c - b * d);
                f_xy[1] += static_cast<float>(a * d + b * c);
            }
        }

        f_xy[0] *= MN_inv;
        f_xy[1] *= MN_inv;
    });

    if (real_only) {
        std::vector<cv::Mat> channels;
        cv::split(f, channels);
        return channels[0];
    }

    return f;
}

cv::Mat dip::dft_fwd_2d_separable(const cv::Mat& f) {
    const int M = f.rows;
    const int N = f.cols;

    cv::Mat F(M,N, CV_32FC2);
    cv::merge(std::vector<cv::Mat> {f, cv::Mat::zeros(M,N, CV_32F)}, F);

    cv::Mat row_buffer(1, M, CV_32FC2);

    for (int v = 0; v < F.rows; v++) {
        F.row(v).copyTo(row_buffer);
        dft_fwd_1d_complex(row_buffer).copyTo(F.row(v));
        std::cout << F << std::endl;
    }

    for (int u = 0; u < F.cols; u++) {
        cv::transpose(F.col(u), row_buffer);
        cv::transpose(dft_fwd_1d_complex(row_buffer), F.col(u));
        std::cout << F << std::endl;
    }

    cv::Mat spectrum(F.rows, F.cols, CV_32F);
    F.forEach<cv::Vec2f>([&](cv::Vec2f& f_xy, const int* pos_m) {
       spectrum.at<float>(pos_m[0], pos_m[1]) = cv::sqrt(f_xy[0] * f_xy[0] + f_xy[1] * f_xy[1]);
    });

    return F;
}
