#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "dp/permutation.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int size = 1; size <= 8; ++size) {
        for (int trial = 0; trial < 100; ++trial) {
            std::vector allowed(
                size, std::vector(size + 1, std::vector<char>(3))
            );
            for (auto& by_component : allowed) {
                for (auto& by_joined : by_component) {
                    for (char& value : by_joined) value = random() % 2;
                }
            }
            const auto fast = poe::insertion_component_counts<long long>(
                size,
                [&](int inserted, int components, int joined) {
                    return allowed[inserted][components][joined];
                }
            );

            std::vector<long long> brute(size + 1);
            std::vector<int> permutation(size);
            std::iota(permutation.begin(), permutation.end(), 0);
            do {
                std::vector<int> position(size);
                for (int index = 0; index < size; ++index) {
                    position[permutation[index]] = index;
                }
                std::vector<char> active(size);
                int components = 0;
                bool valid = true;
                for (int value = 0; value < size; ++value) {
                    const int index = position[value];
                    const int joined =
                        (index > 0 && active[index - 1])
                        + (index + 1 < size && active[index + 1]);
                    if (!allowed[value][components][joined]) {
                        valid = false;
                        break;
                    }
                    active[index] = true;
                    components += 1 - joined;
                }
                if (valid) ++brute[components];
            } while (std::next_permutation(permutation.begin(), permutation.end()));
            assert(fast == brute);
        }
    }
}
