#include <cassert>
#include <numeric>

#include "math/fraction.hpp"

int main() {
    for (long long b = 1; b <= 20; ++b) {
        for (long long a = 0; a <= 2 * b; ++a) {
            for (long long d = 1; d <= 20; ++d) {
                for (long long c = 1; c <= 2 * d + 1; ++c) {
                    if (a * d >= c * b) continue;
                    const auto result = poe::simplest_fraction_between(a, b, c, d);
                    assert(a * result.denominator < result.numerator * b);
                    assert(result.numerator * d < c * result.denominator);
                    assert(std::gcd(result.numerator, result.denominator) == 1);
                    bool found = false;
                    for (long long q = 1; q < result.denominator; ++q) {
                        const long long first = a * q / b + 1;
                        if (a * q < first * b && first * d < c * q) found = true;
                    }
                    assert(!found);
                }
            }
        }
    }
}
