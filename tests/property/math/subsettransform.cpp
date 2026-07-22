#include <cassert>
#include <random>
#include <vector>

#include "math/modint.hpp"
#include "math/subsettransform.hpp"

int main() {
    std::mt19937 random(20260716);
    using mint = poe::staticmodint<998244353>;
    for (int bits = 0; bits <= 6; ++bits) {
        const int count = 1 << bits;
        for (int trial = 0; trial < 10; ++trial) {
            std::vector<mint> left(count), right(count);
            left[0] = 1;
            for (int mask = 1; mask < count; ++mask) left[mask] = random() % 20;
            for (auto& value : right) value = random() % 20;
            std::vector<mint> expected(count);
            for (int mask = 0; mask < count; ++mask) {
                for (int sub = mask;; sub = (sub - 1) & mask) {
                    expected[mask] += left[sub] * right[mask ^ sub];
                    if (sub == 0) break;
                }
            }
            assert(poe::subset_convolution(left, right) == expected);

            std::fill(expected.begin(), expected.end(), mint{});
            for (int first = 0; first < count; ++first) {
                for (int second = 0; second < count; ++second) {
                    expected[first ^ second] += left[first] * right[second];
                }
            }
            assert(poe::xor_convolution(left, right) == expected);

            const int exponent = random() % 7;
            std::vector<mint> power(count);
            power[0] = 1;
            for (int iteration = 0; iteration < exponent; ++iteration) {
                power = poe::subset_convolution(power, left);
            }
            assert(poe::subset_convolution_unit_power(left, exponent) == power);
        }
    }
    for (int trial = 0; trial < 3000; ++trial) {
        const int conditions = random() % 8;
        const int limit = random() % 101;
        std::vector<long long> divisors(conditions);
        for (auto& divisor : divisors) divisor = 1 + random() % 30;

        std::vector<long long> expected(conditions + 1);
        for (int value = 1; value <= limit; ++value) {
            int matches = 0;
            for (const long long divisor : divisors) matches += value % divisor == 0;
            ++expected[matches];
        }
        assert(poe::divisibility_match_counts(limit, divisors) == expected);
    }
}
