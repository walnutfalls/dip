#include "command_interpreter.hpp"
#include <filesystem>
#include <algorithm>


bool is_ws(const std::string &str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
}

std::string dip::command_interpreter::qualify(const std::string &path) const {
    const std::filesystem::path fs_path(path);

    if (fs_path.is_absolute()) {
        return path;
    }

    const std::filesystem::path fs_pwd(_pwd);
    return (fs_pwd / path).generic_string();
}

std::string trim(const std::string &str) {
    const auto start = std::find_if_not(str.begin(), str.end(), [](const unsigned char ch) {
        return std::isspace(ch);
    });

    const auto end = std::find_if_not(str.rbegin(), str.rend(), [](const unsigned char ch) {
        return std::isspace(ch);
    }).base();

    if (start >= end) {
        return "";
    }

    return std::string(start, end);
}

void dip::command_interpreter::save(const std::string &path) const {
    const auto p = qualify(path);
    cv::imwrite(p, _output.get());
}

void dip::command_interpreter::load(const std::string &path) const {
    std::filesystem::path fs_path(path);

    if (fs_path.is_absolute()) {
        _a.get() = cv::imread(fs_path.generic_string(), cv::IMREAD_COLOR);
    } else {
        const std::filesystem::path fs_pwd(_pwd);
        const auto full = (fs_pwd / path).generic_string();
        _a.get() = cv::imread(full, cv::IMREAD_COLOR);
    }
}

std::string dip::command_interpreter::next_suggested(const std::string &input) {
    static std::filesystem::directory_iterator end;

    const std::filesystem::path fs_pwd(_pwd);

    const auto space_ind = input.find_last_of(' ');
    const auto candidate = trim(space_ind == -1 ? input : input.substr(space_ind));
    std::filesystem::path fs_candidate(candidate);

    const auto candidate_parent = fs_candidate.parent_path();

    if (candidate_parent.is_absolute() && !std::filesystem::exists(candidate_parent)) {
        return input;
    } else if (!candidate_parent.is_absolute()) {
        fs_candidate = fs_pwd / fs_candidate;
    }

    const auto iterable_dir = candidate_parent.is_absolute()
                                  ? candidate_parent
                                  : fs_pwd;

    if (std::filesystem::exists(iterable_dir)) {
        for (auto it = std::filesystem::directory_iterator(iterable_dir); it != end; ++it) {
            const auto s = it->path().string();
            if (s.rfind(fs_candidate.string(), 0) == 0) {
                std::string in = input;
                in.replace(space_ind + 1, candidate.size(), s);
                return in;
            }
        }
    }

    return input;
}


void dip::command_interpreter::load_operands(size_t loc) {
    const auto p1 = std::filesystem::path(_pwd) / last_command[loc++];
    const auto p2 = std::filesystem::path(_pwd) / last_command[loc];

    const cv::Mat m1 = cv::imread(p1.generic_string(), cv::IMREAD_COLOR);
    const cv::Mat m2 = cv::imread(p2.generic_string(), cv::IMREAD_COLOR);
    const auto max_width = std::max(m1.cols, m2.cols);
    const auto max_height = std::max(m1.rows, m2.rows);
    cv::copyMakeBorder(m1, a, 0, max_height - m1.rows, 0, max_width - m1.cols, cv::BORDER_CONSTANT, 0);
    cv::copyMakeBorder(m2, b, 0, max_height - m2.rows, 0, max_width - m2.cols, cv::BORDER_CONSTANT, 0);
}

void dip::command_interpreter::cd(const std::string &path) {
    const std::filesystem::path fs_path(path);
    const std::filesystem::path fs_pwd(_pwd);

    const auto abspath = fs_path.is_absolute() ? path : std::filesystem::absolute(fs_pwd / path).generic_string();

    if (std::filesystem::exists(_pwd)) {
        _pwd = abspath;
    }
}

void dip::command_interpreter::ls() {
    _out_text.clear();

    if (std::filesystem::exists(_pwd)) {
        for (const auto &file: std::filesystem::directory_iterator(_pwd)) {
            _out_text.append(file.path().string());
            _out_text.append("\n");
        }
    }
}

void dip::command_interpreter::add(const cv::Mat &a, const cv::Mat &b) const {
    _output.get() = a + b;
}

void dip::command_interpreter::sub(const cv::Mat &a, const cv::Mat &b) const {
    _output.get() = a - b;
}

void dip::command_interpreter::mul(const cv::Mat &a, const cv::Mat &b) const {
    _output.get() = a.mul(b);
}

void dip::command_interpreter::inv(const cv::Mat &a) const {
    bitwise_not(a, _output.get());
}

void dip::command_interpreter::log(const cv::Mat &a, float base, float c) const {
    cv::Mat m;
    a.convertTo(m, CV_32F);
    cv::Mat log_nat;
    cv::log(m + 1, log_nat);
    log_nat /= std::log(base);
    m = c * m;
    m.convertTo(_output.get(), CV_8U);
}

void dip::command_interpreter::pow(const cv::Mat &a, float gamma, float c) const {
    cv::Mat m;
    a.convertTo(m, CV_32F);
    cv::pow(m, gamma, m);
    m = c * m;
    m.convertTo(_output.get(), CV_8U);
}

std::optional<dip::operation> dip::command_interpreter::interpret_command(const std::string &command) {
    split(command);

    if (last_command.empty()) {
        return std::optional<dip::operation>();
    }

    std::transform(last_command[0].begin(), last_command[0].end(), last_command[0].begin(), [](const unsigned char c) {
        return std::tolower(c);
    });

    if (handlers.find(last_command[0]) != handlers.end()) {
        return handlers[last_command[0]]();
    }

    return std::optional<dip::operation>();
}

size_t dip::command_interpreter::find_param_index(const char *flag) const {
    size_t i = 1;
    while (last_command[i] != flag) {
        i++;
    }
    return i + 1;
}

std::vector<std::string> dip::command_interpreter::split(const std::string &str, const char delimiter) {
    last_command.clear();

    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        if (auto part = trim(str.substr(start, end - start)); !part.empty()) {
            last_command.push_back(part);
        }

        start = end + 1;
        end = str.find(delimiter, start);
    }
    if (const auto part = trim(str.substr(start)); !part.empty()) {
        last_command.push_back(part);
    }

    return last_command;
}
