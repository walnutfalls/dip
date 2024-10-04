#pragma once
#include <optional>
#include <string>
#include <opencv2/opencv.hpp>

#include "operation.hpp"

namespace dip {
    class command_interpreter {
    public:
        explicit command_interpreter(cv::Mat &output)
            : _output(output) {
        };

        void cd(const std::string &path);

        void add(const cv::Mat &a, const cv::Mat &b);

        void sub(const cv::Mat &a, const cv::Mat &b);

        void mul(const cv::Mat &a, const cv::Mat &b);

        void inv(const cv::Mat &a);

        void log(const cv::Mat &a, float base, float c);

        void pow(const cv::Mat &a, float gamma, float c);


        void save(const std::string &path);

        std::optional<operation> interpret_command(const std::string &command);

    private:
        std::reference_wrapper<cv::Mat> _output;
        std::string _pwd{"."};

        cv::Mat a;
        cv::Mat b;

        static std::vector<std::string> split(const std::string &str, const char delimiter = ' ') {
            std::vector<std::string> tokens;
            size_t start = 0;
            size_t end = str.find(delimiter);

            while (end != std::string::npos) {
                tokens.push_back(str.substr(start, end - start));
                start = end + 1;
                end = str.find(delimiter, start);
            }
            tokens.push_back(str.substr(start));
            return tokens;
        }

    };
}
