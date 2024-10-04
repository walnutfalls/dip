#pragma once
#include <vector>
#include <utility>

namespace dip {
    const std::vector<std::pair<int, int> > N4_connectivity = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1},
    };

    const std::vector<std::pair<int, int> > Nd_connectivity = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
    };

    const std::vector<std::pair<int, int> > N8_connectivity = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1},
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
    };
}
