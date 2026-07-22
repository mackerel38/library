#include "dp/groupassignment.hpp"

#include <array>
#include <cassert>
#include <limits>
#include <random>
#include <vector>

int main() {
    std::mt19937 random(290);
    constexpr long long infinity = std::numeric_limits<long long>::max() / 4;
    for (int size = 0; size <= 10; ++size) {
        for (int trial = 0; trial < 300; ++trial) {
            std::vector<int> category(size);
            std::array<int, 3> total{};
            for (int& value : category) {
                value = random() % 3;
                ++total[value];
            }
            std::array<int, 3> target{};
            for (int kind = 0; kind < 3; ++kind) target[kind] = random() % (total[kind] + 1);
            std::vector<std::array<long long, 2>> base(size);
            std::vector<std::array<std::array<long long, 3>, 2>> coefficient(size);
            for (int index = 0; index < size; ++index) {
                for (int group = 0; group < 2; ++group) {
                    base[index][group] = static_cast<int>(random() % 21) - 10;
                    for (int kind = 0; kind < 3; ++kind) {
                        coefficient[index][group][kind] = static_cast<int>(random() % 9) - 4;
                    }
                }
            }
            auto cost = [&](int index, int group, const auto& first, const auto& second) {
                long long result = base[index][group];
                for (int kind = 0; kind < 3; ++kind) {
                    result += coefficient[index][group][kind] * (first[kind] + 2LL * second[kind]);
                }
                return result;
            };

            long long expected = infinity;
            for (int mask = 0; mask < (1 << size); ++mask) {
                std::array<int, 3> first{}, second{};
                long long sum = 0;
                for (int index = 0; index < size; ++index) {
                    const int group = mask >> index & 1;
                    sum += cost(index, group, first, second);
                    ++(group == 0 ? first : second)[category[index]];
                }
                if (first == target) expected = std::min(expected, sum);
            }
            const long long actual = poe::minimum_two_group_assignment<long long>(
                category, target, cost, infinity
            );
            assert(actual == expected);
        }
    }
}
