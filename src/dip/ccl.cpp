#include "ccl.hpp"
#include "connectivity.hpp"


cv::Mat dip::ccl::build_labels_map(const cv::Mat &image) {
    cv::Mat labels = cv::Mat::zeros(image.rows, image.cols, CV_8UC3);

    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            const auto &pixel = image.at<cv::Vec3b>(row, col);
            const auto &label = labels.at<cv::Vec3b>(row, col);

            const bool is_unset = label[0] == 0 && label[1] == 0 && label[2] == 0;

            if (is_unset && is_foreground(pixel)) {
                traverse_region(image, labels, {row, col});
                _label = rand_color();
            }
        }
    }

    return labels;
}

void dip::ccl::set_sensitivity(const float sensitivity) {
    _sensitivity = sensitivity;
}

bool dip::ccl::is_foreground(const cv::Vec3b &pixel) const {
    const float percent_of_max = static_cast<float>(pixel[0]) / 255;
    return percent_of_max > _sensitivity;
}

void dip::ccl::traverse_region(
    const cv::Mat &image,
    cv::Mat &labels,
    std::pair<int, int> coord) const {
    std::queue<std::pair<int, int> > q;
    q.emplace(coord);

    auto out_of_bounds = [&](const std::pair<int, int> &x) {
        return x.first < 0 || x.first >= image.rows ||
               x.second < 0 || x.second >= image.cols;
    };

    // this will actually set the label
    auto check_neighbor = [&](const std::pair<int, int> &x) -> bool {
        if (out_of_bounds(x)) {
            return false;
        }

        auto &pixel = image.at<cv::Vec3b>(x.first, x.second);
        auto &label = labels.at<cv::Vec3b>(x.first, x.second);

        const bool is_unset = label[0] == 0 && label[1] == 0 && label[2] == 0;

        if (is_unset && is_foreground(pixel)) {
            label = _label; // NOTE - sets the label
            return true;
        }

        return false;
    };

    while (!q.empty()) {
        coord = q.front();
        q.pop();

        for (const auto &p: _connectivity.get()) {
            std::pair neighbor{p.first + coord.first, p.second + coord.second};

            if (check_neighbor(neighbor)) {
                q.emplace(neighbor);
            }
        }

        if (_m_adjacency_check) {
            for (const auto &p: Nd_connectivity) {
                std::pair neighbor{coord.first, p.second + coord.second};

                if (!out_of_bounds(neighbor)) {
                    if (is_foreground(image.at<cv::Vec3b>(neighbor.first, neighbor.second))) {
                        continue;
                    }
                }

                neighbor = {p.first + coord.first, coord.second};
                if (!out_of_bounds(neighbor)) {
                    if (is_foreground(image.at<cv::Vec3b>(neighbor.first, neighbor.second))) {
                        continue;
                    }
                }

                neighbor = {p.first + coord.first, p.second + coord.second};

                if (check_neighbor(neighbor)) {
                    q.emplace(neighbor);
                }
            }
        }
    }
}
