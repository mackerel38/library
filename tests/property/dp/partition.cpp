#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <vector>

#include "dp/partition.hpp"

int main() {
    std::mt19937 random(438);
    for (int size = 1; size <= 9; ++size) {
        for (int phases = 1; phases <= size; ++phases) {
            for (int trial = 0; trial < 200; ++trial) {
                std::vector<std::vector<long long>> score(phases, std::vector<long long>(size));
                for (auto& row : score) for (auto& value : row) value = int(random() % 21) - 10;
                long long expected = std::numeric_limits<long long>::lowest();
                for (int mask = 0; mask < (1 << (size - 1)); ++mask) {
                    if (__builtin_popcount(static_cast<unsigned>(mask)) != phases - 1) continue;
                    int phase = 0;
                    long long sum = 0;
                    for (int i = 0; i < size; ++i) {
                        sum += score[phase][i];
                        if (i + 1 < size && (mask >> i & 1)) ++phase;
                    }
                    expected = std::max(expected, sum);
                }
                assert(poe::max_phase_partition(score) == expected);
            }
        }
    }
}
