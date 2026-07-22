#include "structure/fenwicktree.hpp"

using namespace std;
using namespace poe;

constexpr int maximum_int(int left, int right) {
    return left < right ? right : left;
}

constexpr int zero_int() {
    return 0;
}

int main() {
    mt19937 random(2662);
    for (int iteration = 0; iteration < 1000; ++iteration) {
        vector<pair<int, int>> points;
        for (int x = -4; x <= 4; ++x) {
            for (int y = -4; y <= 4; ++y) points.emplace_back(x, y);
        }
        fenwicktree2d<int, int, maximum_int, zero_int> data(points);
        map<pair<int, int>, int> values;
        for (int query = 0; query < 200; ++query) {
            const int x = static_cast<int>(random() % 9) - 4;
            const int y = static_cast<int>(random() % 9) - 4;
            const int value = static_cast<int>(random() % 1000);
            data.apply(x, y, value);
            values[{x, y}] = max(values[{x, y}], value);

            const int right_x = static_cast<int>(random() % 13) - 6;
            const int right_y = static_cast<int>(random() % 13) - 6;
            int expected = 0;
            for (const auto& [point, current] : values) {
                if (point.first <= right_x && point.second <= right_y) {
                    expected = max(expected, current);
                }
            }
            assert(data.prod(right_x, right_y) == expected);
        }
    }
}
