#pragma once
#include <optional>
#include <string>
#include <opencv2/opencv.hpp>

#include "operation.hpp"

namespace dip {
    class command_interpreter {
    public:
        explicit command_interpreter(cv::Mat &a, cv::Mat &output)
            : _a(a)
            , _output(output) {}

        void cd(const std::string &path);
        void ls();

        void add(const cv::Mat &a, const cv::Mat &b) const;

        void sub(const cv::Mat &a, const cv::Mat &b) const;

        void mul(const cv::Mat &a, const cv::Mat &b) const;

        void inv(const cv::Mat &a) const;

        void log(const cv::Mat &a, float base, float c) const;

        void pow(const cv::Mat &a, float gamma, float c) const;


        std::string qualify(const std::string &path) const;

        void save(const std::string &path) const;

        void load(const std::string& path) const;

        std::optional<operation> interpret_command(const std::string &command);

        [[nodiscard]] const std::vector<std::string> &get_command() const { return last_command; }
        [[nodiscard]] const std::string &out_text() const { return _out_text; }

    private:
        std::reference_wrapper<cv::Mat> _a;
        std::reference_wrapper<cv::Mat> _output;
        std::string _pwd{"."};
        std::string _out_text;

        cv::Mat a;
        cv::Mat b;

        std::vector<std::string> last_command;

        size_t find_param_index(const char* flag) const;

        std::vector<std::string> split(const std::string &str, char delimiter = ' ');

    };
}
