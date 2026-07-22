#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "algorithm/groupxor.hpp"

int main() {
    std::mt19937 random(20260722);
    for (int trial = 0; trial < 3000; ++trial) {
        const int count = random() % 7;
        std::vector<std::vector<unsigned>> groups(count);
        std::vector<unsigned> brute{0};
        for (auto& group : groups) {
            group.resize(1 + random() % 4);
            for (auto& value : group) value = random() % 256;
            std::vector<unsigned> next;
            for (const unsigned prefix : brute)
                for (const unsigned value : group) next.push_back(prefix ^ value);
            brute = std::move(next);
        }
        std::ranges::sort(brute, std::greater{});
        for (int query = 0; query < 10; ++query) {
            const unsigned long long k = 1 + random() % brute.size();
            assert((poe::kth_largest_group_xor<unsigned, 8>(groups, k) == brute[k - 1]));
        }
    }
}
