#include <cassert>
#include <random>
#include <vector>

#include "dp/subsetsum.hpp"

int main() {
    std::mt19937 random(427);
    for (int trial = 0; trial < 500; ++trial) {
        const int size = random() % 17;
        const int modulo = 1 + random() % 20;
        std::vector<int> values(size);
        for (int& value : values) value = random() % modulo;
        std::vector<unsigned long long> expected(modulo);
        const std::uint64_t count = std::uint64_t{1} << size;
        for (std::uint64_t mask = 0; mask < count; ++mask) {
            if (mask & (mask << 1)) continue;
            int sum = 0;
            for (int bit = 0; bit < size; ++bit) {
                if (mask >> bit & 1) sum = (sum + values[bit]) % modulo;
            }
            ++expected[sum];
        }
        for (int target = 0; target < modulo; ++target) {
            assert(poe::count_nonadjacent_subsets_mod(values, modulo, target)
                   == expected[target]);
        }
        const auto listed = poe::nonadjacent_subset_sums(values, modulo);
        unsigned long long total = 0;
        for (const auto count_for_sum : expected) total += count_for_sum;
        assert(listed.all.size() == total);
    }
}
