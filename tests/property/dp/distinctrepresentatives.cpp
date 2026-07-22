#include <bits/stdc++.h>
#include "dp/distinctrepresentatives.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;
using mint = modint998244353;

int main() {
    mt19937 random(236);
    for (int trial = 0; trial < 5000; ++trial) {
        const int size = random() % 8;
        const int universe = 1 + random() % 8;
        vector<unsigned> sets(size);
        for (auto& set : sets) set = random() & ((1U << universe) - 1);
        long long expected = 0;
        auto brute = [&](auto&& self, int index, unsigned used) -> void {
            if (index == size) {
                ++expected;
                return;
            }
            unsigned candidates = sets[index] & ~used;
            while (candidates) {
                const unsigned bit = candidates & -candidates;
                candidates ^= bit;
                self(self, index + 1, used | bit);
            }
        };
        brute(brute, 0, 0);
        const auto actual = count_distinct_representatives<mint>(size, [&](int mask) {
            unsigned intersection = (1U << universe) - 1;
            for (int index = 0; index < size; ++index) {
                if (mask >> index & 1) intersection &= sets[index];
            }
            return popcount(intersection);
        });
        assert(actual == expected);

        if (size <= 6) {
            const int limit = 1 + random() % 20;
            vector<long long> divisors(size);
            for (auto& divisor : divisors) divisor = 1 + random() % limit;
            expected = 0;
            auto multiples_brute = [&](auto&& self, int index, unsigned long long used) -> void {
                if (index == size) {
                    ++expected;
                    return;
                }
                for (int value = divisors[index]; value <= limit; value += divisors[index]) {
                    if (!(used >> value & 1)) self(self, index + 1, used | (1ULL << value));
                }
            };
            multiples_brute(multiples_brute, 0, 0);
            assert(count_distinct_bounded_multiples<mint>(limit, divisors) == expected);
        }
    }
}
