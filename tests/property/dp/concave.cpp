#include <algorithm>
#include <cassert>
#include <optional>
#include <random>
#include <vector>

#include "dp/concave.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 500; ++trial) {
        const int maximum = 1 + random() % 20;
        poe::concavesequence sequence(maximum, 0);
        std::vector<std::optional<long long>> brute(maximum + 1);
        brute[maximum] = 0;
        for (int operation = 0; operation < 30; ++operation) {
            const long long affine_slope = -static_cast<int>(random() % 6);
            const long long constant = random() % 31;
            sequence.add_affine(affine_slope, constant);
            for (int x = 0; x <= maximum; ++x) {
                if (brute[x]) *brute[x] += affine_slope * x + constant;
                if (brute[x] && *brute[x] < 0) brute[x].reset();
            }
            sequence.trim_below(0);
            if (!sequence.empty()) {
                const long long slope = 1 + random() % 8;
                sequence.suffix_max(slope, 0);
                std::vector<std::optional<long long>> next(maximum + 1);
                for (int x = 0; x <= maximum; ++x) {
                    for (int y = x; y <= maximum; ++y) {
                        if (!brute[y]) continue;
                        const long long value = *brute[y] - slope * (y - x);
                        if (!next[x] || *next[x] < value) next[x] = value;
                    }
                    if (next[x] && *next[x] < 0) next[x].reset();
                }
                brute = std::move(next);
                sequence.trim_below(0);
            }
            for (int x = 0; x <= maximum; ++x) assert(sequence.get(x) == brute[x]);
            if (sequence.empty()) break;
        }
    }
}
