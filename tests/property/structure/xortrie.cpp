#include <cassert>
#include <random>
#include <vector>

#include "structure/xortrie.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = random() % 20;
        std::vector<unsigned> values(n);
        for (auto& value : values) value = random() % 256;
        for (int query = 0; query < 20; ++query) {
            const unsigned limit = random() % 256;
            long long brute = 0;
            for (int i = 0; i < n; ++i)
                for (int j = i + 1; j < n; ++j) brute += (values[i] ^ values[j]) <= limit;
            assert((poe::count_pairs_xor_at_most<unsigned, 8>(values, limit) == brute));
        }
        if (!values.empty()) {
            for (unsigned left = 0; left <= 256; left += 17) {
                for (unsigned right = left; right <= 256; right += 19) {
                    unsigned long long brute = 0;
                    for (unsigned value = left; value < right; ++value) {
                        unsigned best = std::numeric_limits<unsigned>::max();
                        for (const unsigned candidate : values) best = std::min(best, value ^ candidate);
                        brute += best;
                    }
                    assert((poe::sum_min_xor<unsigned, 8>(values, left, right) == brute));
                }
            }
        }
    }
}
