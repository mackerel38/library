#include <cassert>
#include <random>
#include "structure/laminarintervalset.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 2; n <= 100; ++n) {
        for (int test = 0; test < 300; ++test) {
            poe::laminarintervalset data(n);
            std::vector<std::pair<int, int>> accepted;
            std::vector<int> coordinates(n);
            std::iota(coordinates.begin(), coordinates.end(), 0);
            std::ranges::shuffle(coordinates, random);
            for (int query = 0; query + 1 < n; query += 2) {
                int left = coordinates[query];
                int right = coordinates[query + 1];
                if (left > right) std::swap(left, right);
                bool expected = true;
                for (const auto [first, second] : accepted) {
                    const bool crosses =
                        (first < left && left < second && second < right)
                        || (left < first && first < right && right < second);
                    expected &= !crosses;
                }
                assert(data.add(left, right) == expected);
                if (expected) accepted.emplace_back(left, right);
                assert(data.intervals() == accepted);
            }
        }
    }
}
