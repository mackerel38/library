#include <cassert>
#include <random>
#include <vector>

#include "geometry/rectangleunion.hpp"

int main() {
    std::mt19937 random(20260715);
    using rectangle = poe::axisalignedrectangle<long long>;
    for (int trial = 0; trial < 1000; ++trial) {
        std::vector<rectangle> rectangles;
        bool covered[8][8]{};
        const int n = random() % 10;
        for (int i = 0; i < n; ++i) {
            int left = random() % 9, right = random() % 9;
            int bottom = random() % 9, top = random() % 9;
            if (left > right) std::swap(left, right);
            if (bottom > top) std::swap(bottom, top);
            rectangles.push_back({left, right, bottom, top});
            for (int x = left; x < right; ++x)
                for (int y = bottom; y < top; ++y) covered[x][y] = true;
        }
        long long brute = 0;
        for (auto& row : covered) for (bool cell : row) brute += cell;
        assert(poe::rectangle_union_area(rectangles) == brute);
    }

    for (int trial = 0; trial < 1000; ++trial) {
        long long left = random() % 8, right = left + 1 + random() % 8;
        long long bottom = random() % 8, top = bottom + 1 + random() % 8;
        const rectangle original{left, right, bottom, top};
        const long long x = static_cast<int>(random() % 24) - 4;
        const long long y = static_cast<int>(random() % 24) - 4;
        const auto xs = poe::split_rectangle_x(original, x);
        const auto ys = poe::split_rectangle_y(original, y);
        long long x_area = 0, y_area = 0;
        for (const auto& part : xs) x_area += poe::rectangle_area(part);
        for (const auto& part : ys) y_area += poe::rectangle_area(part);
        assert(x_area == poe::rectangle_area(original));
        assert(y_area == poe::rectangle_area(original));
    }
}
