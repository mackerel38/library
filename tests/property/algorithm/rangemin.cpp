#include <cassert>
#include <random>
#include <vector>

#include "algorithm/rangemin.hpp"

int main() {
    std::mt19937 random(438);
    using query = poe::rangeminsumquery<long long>;
    for (int size = 0; size <= 50; ++size) {
        for (int trial = 0; trial < 100; ++trial) {
            std::vector<long long> values(size);
            for (auto& value : values) value = int(random() % 41) - 20;
            std::vector<query> queries;
            std::vector<long long> expected;
            for (int q = 0; q < 100; ++q) {
                int left = random() % (size + 1);
                int right = random() % (size + 1);
                if (left > right) std::swap(left, right);
                const long long cap = int(random() % 51) - 25;
                queries.push_back({left, right, cap});
                long long sum = 0;
                for (int i = left; i < right; ++i) sum += std::min(values[i], cap);
                expected.push_back(sum);
            }
            assert(poe::offline_range_min_sum(values, queries) == expected);
        }
    }
}
