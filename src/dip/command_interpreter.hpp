#pragma once
#include <filesystem>
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

        void save(const std::string &path) const;

        void load(const std::string& path) const;

        [[nodiscard]] std::string next_suggested(const std::string& input);

        std::optional<operation> interpret_command(const std::string &command);

        [[nodiscard]] const std::vector<std::string> &get_command() const { return last_command; }
        [[nodiscard]] const std::string &out_text() const { return _out_text; }

    private:
        size_t find_param_index(const char* flag) const;
        std::vector<std::string> split(const std::string &str, char delimiter = ' ');
        void load_operands(size_t loc);

        [[nodiscard]] std::string qualify(const std::string &path) const;


        std::reference_wrapper<cv::Mat> _a;
        std::reference_wrapper<cv::Mat> _output;
        std::string _pwd{std::filesystem::current_path().string()};
        std::string _out_text;

        cv::Mat a;
        cv::Mat b;

        std::vector<std::string> last_command;

        std::unordered_map<std::string, std::function<std::optional<operation>()>> handlers = {
            { "cd", [&](){
                cd(last_command[1]);
                return std::optional<dip::operation>();
            } },

            { "ls", [&](){
                ls();
                return std::optional<dip::operation>();
            } },

            { "load", [&](){
                load(last_command[1]);
                return std::optional<dip::operation>();
            } },

            { "save", [&](){
                save(last_command[1]);
                return std::optional<dip::operation>();
            } },

            { "add", [&](){
                size_t i = find_param_index("-i");
                load_operands(i);
                add(a, b);
                i = find_param_index("-o");
                save(last_command[i]);
                return operation::add;
            } },

            { "sub", [&](){
                size_t i = find_param_index("-i");
                load_operands(i);
                sub(a, b);
                i = find_param_index("-o");
                save(last_command[i]);
                return operation::sub;
            } },

            { "mul", [&](){
                size_t i = find_param_index("-i");
                load_operands(i);
                mul(a, b);
                i = find_param_index("-o");
                save(last_command[i]);
                return operation::mul;
            } },

            { "inv", [&](){
                size_t i = find_param_index("-i");
                load(last_command[i]);
                inv(_a);
                i = find_param_index("-o");
                save(last_command[i]);
                return operation::negate;
            } },

            { "log", [&](){
                size_t i = find_param_index("-c");
                const float c = std::stof(last_command[i]);
                i = find_param_index("-b");
                const float base = std::stof(last_command[i]);
                i = find_param_index("-i");
                load(last_command[i]);
                log(_a, base, c);
                i = find_param_index("-o");
                save(last_command[i]);
                return operation::log;
            } },

            { "pow", [&](){
                size_t i = find_param_index("-c");
                const float c = std::stof(last_command[i]);
                i = find_param_index("-gamma");
                const float gamma = std::stof(last_command[i]);
                i = find_param_index("-i");
                load(last_command[i]);
                pow(_a, gamma, c);
                i = find_param_index("-o");
                save(last_command[i]);
                return operation::gamma;
            } },
        };

    };
}
