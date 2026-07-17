#include <cassert>
#include <random>
#include <utility>
#include <vector>

#include "structure/nestedintervals.hpp"

int main() {
    std::mt19937 random(428);
    for (int trial = 0; trial < 200; ++trial) {
        const int size = 1 + random() % 30;
        std::vector<long long> lengths(size);
        for (int i = 0; i < size; ++i) lengths[i] = (i ? lengths[i - 1] : 0) + 1 + random() % 5;
        poe::nestedintervals<long long> intervals(lengths);
        std::vector<std::pair<long long, long long>> brute(size);
        for (int i = 0; i < size; ++i) brute[i] = {0, lengths[i]};
        for (int query = 0; query < 300; ++query) {
            const int last = random() % size;
            if (random() & 1) {
                const long long left = brute[last].first;
                for (int i = 0; i <= last; ++i) brute[i] = {left, left + lengths[i]};
                intervals.align_left(last);
            } else {
                const long long right = brute[last].second;
                for (int i = 0; i <= last; ++i) brute[i] = {right - lengths[i], right};
                intervals.align_right(last);
            }
            for (int i = 0; i < size; ++i) assert(intervals[i] == brute[i]);
            for (long long coordinate = -20; coordinate <= 50; ++coordinate) {
                int expected = 0;
                for (const auto [left, right] : brute) expected += left <= coordinate && coordinate < right;
                assert(intervals.count_covering(coordinate) == expected);
            }
        }
    }
}
