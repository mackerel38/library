#include <algorithm>
#include <cassert>
#include <random>
#include "string/concatenation.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 2; n <= 8; ++n) {
        for (int trial = 0; trial < 120; ++trial) {
            std::vector<std::string> strings(n);
            for (auto& value : strings) {
                const int length = 1 + random() % 5;
                while (static_cast<int>(value.size()) < length) value += char('a' + random() % 3);
            }
            std::vector<int> permutation(n);
            std::iota(permutation.begin(), permutation.end(), 0);
            std::vector<std::string> all;
            do {
                std::string value;
                for (int index : permutation) value += strings[index];
                all.push_back(std::move(value));
            } while (std::next_permutation(permutation.begin(), permutation.end()));
            std::sort(all.begin(), all.end());
            assert(poe::second_concatenation(strings) == all[1]);
            const auto order = poe::concatenation_order(strings);
            std::string minimum;
            for (int index : order) minimum += strings[index];
            assert(minimum == all[0]);
        }
    }
}
