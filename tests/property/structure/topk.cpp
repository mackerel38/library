#include <algorithm>
#include <cassert>
#include <map>
#include <random>
#include <vector>

#include "structure/topk.hpp"

int main() {
    std::mt19937 random(20260716);
    using state = poe::topkdistinct<int, int, 5>;
    for (int repetition = 0; repetition < 1000; ++repetition) {
        state left, right;
        std::map<int, int> brute;
        for (int i = 0; i < 30; ++i) {
            const int value = random() % 100;
            const int key = random() % 10;
            (random() % 2 ? left : right).add(value, key);
            brute[key] = std::max(brute[key], value);
        }
        const state result = poe::merge_topkdistinct(left, right);
        std::vector<std::pair<int, int>> expected;
        for (const auto [key, value] : brute) expected.push_back({value, key});
        std::ranges::sort(expected, std::greater<>());
        expected.resize(std::min<std::size_t>(5, expected.size()));
        assert(result.size() == static_cast<int>(expected.size()));
        for (int i = 0; i < result.size(); ++i) assert(result[i] == expected[i]);
    }
}
