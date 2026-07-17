#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include "math/gaussiannorm.hpp"
#include "math/modint.hpp"

using namespace poe;

int main() {
    for (int value = 1; value <= 150; ++value) {
        int remaining = value;
        std::vector<std::pair<int, unsigned long long>> factors;
        for (int prime = 2; prime * prime <= remaining; ++prime) {
            if (remaining % prime != 0) continue;
            unsigned long long exponent = 0;
            while (remaining % prime == 0) remaining /= prime, ++exponent;
            factors.emplace_back(prime, exponent);
        }
        if (remaining > 1) factors.emplace_back(remaining, 1);
        for (int modulus = 1; modulus <= 7; ++modulus) {
            auto actual = gaussian_norm_residue_distribution<modint998244353>(
                factors, modulus);
            std::vector<int> expected(modulus * modulus);
            const int limit = static_cast<int>(std::sqrt(value));
            for (int x = -limit; x <= limit; ++x) {
                for (int y = -limit; y <= limit; ++y) {
                    if (x * x + y * y != value) continue;
                    const int xr = (x % modulus + modulus) % modulus;
                    const int yr = (y % modulus + modulus) % modulus;
                    ++expected[xr * modulus + yr];
                }
            }
            for (int state = 0; state < modulus * modulus; ++state) {
                assert(actual[state].val() == static_cast<unsigned int>(expected[state]));
            }
        }
    }
}
