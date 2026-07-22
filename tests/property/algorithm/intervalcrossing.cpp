#include "algorithm/intervalcrossing.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(2631);
    for (int iteration = 0; iteration < 10000; ++iteration) {
        const int n = random() % 20;
        vector<pair<int, int>> intervals(n);
        for (auto& [left, right] : intervals) {
            left = static_cast<int>(random() % 15) - 7;
            do right = static_cast<int>(random() % 15) - 7; while (right == left);
            if (right < left) swap(left, right);
        }
        long long expected = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                const auto [a, b] = intervals[i];
                const auto [c, d] = intervals[j];
                expected += (a < c && c < b && b < d) || (c < a && a < d && d < b);
            }
        }
        assert(count_alternating_interval_pairs(intervals) == expected);
    }
}
