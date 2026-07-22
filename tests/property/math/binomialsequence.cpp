#include <cassert>
#include <random>
#include <utility>
#include <vector>
#include "math/binomialsequence.hpp"

int main() {
    std::mt19937 random(20260722);
    for (int trial = 0; trial < 5000; ++trial) {
        const int n = 1 + random() % 40;
        const int steps = random() % n;
        std::vector<int> values(n);
        std::vector<std::pair<int, long long>> runs;
        for (int& value : values) {
            value = random() % 7;
            if (runs.empty() || runs.back().first != value) runs.emplace_back(value, 1);
            else ++runs.back().second;
        }
        for (int turn = 0; turn < steps; ++turn) {
            for (int i = 0; i + 1 < static_cast<int>(values.size()); ++i)
                values[i] = (values[i] + values[i + 1]) % 7;
            values.pop_back();
        }
        const auto result = poe::adjacent_sum_reduce_rle<7>(runs, steps);
        std::vector<int> decoded;
        for (const auto& [value, count] : result) decoded.insert(decoded.end(), count, value);
        assert(decoded == values);
    }
}
