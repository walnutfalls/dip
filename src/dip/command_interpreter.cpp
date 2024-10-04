#include "command_interpreter.hpp"
#include <filesystem>
#include <algorithm>



std::string trim(const std::string &str) {
    const auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });

    const auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();

    if (start >= end) {
        return ""; // Return an empty string if only whitespace is present
    }

    return std::string(start, end);
}

void dip::command_interpreter::save(const std::string &path) {
    auto p = std::filesystem::path(_pwd) / path;
    cv::imwrite(p.generic_string(), _output.get());
}

void dip::command_interpreter::cd(const std::string &path) {
    _pwd = path;
}

void dip::command_interpreter::add(const cv::Mat &a, const cv::Mat &b) {
    _output.get() = a + b;
}

void dip::command_interpreter::sub(const cv::Mat &a, const cv::Mat &b) {
    _output.get() = a - b;
}

void dip::command_interpreter::mul(const cv::Mat &a, const cv::Mat &b) {
    _output.get() = a.mul(b);
}

void dip::command_interpreter::inv(const cv::Mat &a) {
    bitwise_not(a, _output.get());
}

void dip::command_interpreter::log(const cv::Mat &a, float base, float c) {
    cv::Mat m;
    a.convertTo(m, CV_32F);
    cv::Mat log_nat;
    cv::log(m + 1, log_nat);
    log_nat /= std::log(base);
    m = c * m;
    m.convertTo(_output.get(), CV_8U);
}

void dip::command_interpreter::pow(const cv::Mat &a, float gamma, float c) {
    cv::Mat m;
    a.convertTo(m, CV_32F);
    cv::pow(m, gamma, m);
    m = c * m;
    m.convertTo(_output.get(), CV_8U);
}

std::optional<dip::operation> dip::command_interpreter::interpret_command(const std::string &command) {
    auto tokens = split(command, ' ');

    std::transform(tokens.begin(), tokens.end(), tokens.begin(), [](const std::string &s) {
        return trim(s);
    });

    std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), [](const unsigned char c) {
        return std::tolower(c);
    });

    if (tokens.size() < 2) {
        return std::optional<dip::operation>();
    }

    auto load_operands = [&](size_t tokens_i) {
        auto p1 = std::filesystem::path(_pwd) / tokens[tokens_i++];
        auto p2 = std::filesystem::path(_pwd) / tokens[tokens_i];

        cv::Mat m1 = cv::imread(p1.generic_string(), cv::IMREAD_COLOR);
        cv::Mat m2 = cv::imread(p2.generic_string(), cv::IMREAD_COLOR);
        const auto max_width = std::max(m1.cols, m2.cols);
        const auto max_height = std::max(m1.rows, m2.rows);
        cv::copyMakeBorder(m1, a, 0, max_height - m1.rows, 0, max_width - m1.cols, cv::BORDER_CONSTANT, 0);
        cv::copyMakeBorder(m2, b, 0, max_height - m2.rows, 0, max_width - m2.cols, cv::BORDER_CONSTANT, 0);
    };


    if (tokens[0] == "cd") {
        cd(tokens[1]);
    } else if (tokens[0] == "save") {
        save(tokens[1]);
    } else if (tokens[0] == "add") {
        size_t i = 1;
        while (tokens[i] != "-i") {
            i++;
        }

        load_operands(i + 1);
        add(a, b);

        i = 0;
        while (tokens[i] != "-o") {
            i++;
        }

        save(tokens[i + 1]);
        return operation::add;
    } else if (tokens[0] == "sub") {
        size_t i = 1;
        while (tokens[i] != "-i") {
            i++;
        }

        load_operands(i + 1);
        sub(a, b);

        i = 0;
        while (tokens[i] != "-o") {
            i++;
        }

        save(tokens[i + 1]);
        return operation::sub;
    } else if (tokens[0] == "mul") {
        size_t i = 1;
        while (tokens[i] != "-i") {
            i++;
        }

        load_operands(i + 1);
        mul(a, b);

        i = 0;
        while (tokens[i] != "-o") {
            i++;
        }

        save(tokens[i + 1]);
        return operation::mul;
    } else if (tokens[0] == "inv") {
        size_t i = 1;
        while (tokens[i] != "-i") {
            i++;
        }

        a = cv::imread(tokens[i + 1], cv::IMREAD_COLOR);
        inv(a);

        i = 0;
        while (tokens[i] != "-o") {
            i++;
        }

        save(tokens[i + 1]);
        return operation::negate;
    } else if (tokens[0] == "log") {
        size_t i = 1;
        while (tokens[i] != "-c") {
            i++;
        }

        const float c = std::stof(tokens[i + 1]);

        i = 1;
        while (tokens[i] != "-b") {
            i++;
        }

        const float base = std::stof(tokens[i + 1]);

        i = 1;
        while (tokens[i] != "-i") {
            i++;
        }

        a = cv::imread(tokens[i + 1], cv::IMREAD_COLOR);
        log(a, base, c);

        i = 1;
        while (tokens[i] != "-o") {
            i++;
        }

        save(tokens[i + 1]);
        return operation::log;
    } else if (tokens[0] == "pow") {
        size_t i = 1;
        while (tokens[i] != "-c") {
            i++;
        }

        const float c = std::stof(tokens[i + 1]);

        i = 1;
        while (tokens[i] != "-gamma") {
            i++;
        }

        const float gamma = std::stof(tokens[i + 1]);

        i = 1;
        while (tokens[i] != "-i") {
            i++;
        }

        a = cv::imread(tokens[i + 1], cv::IMREAD_COLOR);
        pow(a, gamma, c);

        i = 1;
        while (tokens[i] != "-o") {
            i++;
        }

        save(tokens[i + 1]);
        return operation::gamma;
    }
    return std::optional<dip::operation>();
}
