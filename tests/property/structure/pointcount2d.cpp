#include <cassert>
#include <random>
#include <utility>
#include <vector>

#include "structure/pointcount2d.hpp"

using namespace poe;

int main() {
    std::mt19937 random(233233);
    for (int trial = 0; trial < 3000; ++trial) {
        const int size = random() % 30;
        std::vector<std::pair<int, int>> points(size);
        for (auto& [x, y] : points) {
            x = static_cast<int>(random() % 21) - 10;
            y = static_cast<int>(random() % 21) - 10;
        }
        pointcount2d<int> data(points);
        int x1 = static_cast<int>(random() % 25) - 12;
        int x2 = static_cast<int>(random() % 25) - 12;
        int y1 = static_cast<int>(random() % 25) - 12;
        int y2 = static_cast<int>(random() % 25) - 12;
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        int half_open = 0, closed = 0;
        for (const auto& [x, y] : points) {
            half_open += x1 <= x && x < x2 && y1 <= y && y < y2;
            closed += x1 <= x && x <= x2 && y1 <= y && y <= y2;
        }
        assert(data.count(x1, y1, x2, y2) == half_open);
        assert(data.count_closed(x1, y1, x2, y2) == closed);
    }
}
