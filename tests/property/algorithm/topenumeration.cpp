#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "algorithm/topenumeration.hpp"

void enumerate(
    const std::vector<long long>& values,
    int remaining,
    int index,
    long long sum,
    std::vector<long long>& result
) {
    if (index + 1 == static_cast<int>(values.size())) {
        result.push_back(sum + values[index] * remaining);
        return;
    }
    for (int count = 0; count <= remaining; ++count) {
        enumerate(values, remaining - count, index + 1,
                  sum + values[index] * count, result);
    }
}

int main() {
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 6;
        const int picks = random() % 8;
        std::vector<long long> values(n);
        for (auto& value : values) value = static_cast<int>(random() % 15) - 7;
        std::vector<long long> expected;
        enumerate(values, picks, 0, 0, expected);
        std::ranges::sort(expected, std::greater{});
        const int count = random() % (expected.size() + 5);
        if (static_cast<int>(expected.size()) > count) expected.resize(count);
        assert(poe::top_multiset_sums(values, picks, count) == expected);
    }
}
