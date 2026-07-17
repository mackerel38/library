#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "structure/orderstatistic.hpp"

int main() {
    std::mt19937 random(20260716);
    std::vector<long long> universe(31);
    std::iota(universe.begin(), universe.end(), -15);
    poe::orderstatisticmultiset<long long> data(universe);
    std::vector<long long> values;
    for (int step = 0; step < 10000; ++step) {
        if (values.empty() || random() % 2 == 0) {
            const long long value = static_cast<int>(random() % 31) - 15;
            data.add(value);
            values.push_back(value);
        } else {
            const int index = random() % values.size();
            data.erase(values[index]);
            values.erase(values.begin() + index);
        }
        std::ranges::sort(values);
        assert(data.size() == static_cast<long long>(values.size()));
        assert(data.sum() == std::accumulate(values.begin(), values.end(), 0LL));
        const long long lower = static_cast<int>(random() % 31) - 15;
        const long long upper = static_cast<int>(random() % 31) - 15;
        long long clamped = 0;
        for (const long long value : values) clamped += std::max(lower, std::min(upper, value));
        assert(data.clamp_sum(lower, upper) == clamped);
        if (!values.empty()) {
            const int index = random() % values.size();
            assert(data.kth(index) == values[index]);
            const int count = random() % (values.size() + 1);
            assert(data.sum_smallest(count)
                   == std::accumulate(values.begin(), values.begin() + count, 0LL));
            assert(data.sum_largest(count)
                   == std::accumulate(values.end() - count, values.end(), 0LL));
        }
    }
}
