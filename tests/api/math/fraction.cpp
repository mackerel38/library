#include <cassert>

#include "math/fraction.hpp"

int main() {
    assert(poe::simplest_fraction_between(3, 2, 2, 1).denominator == 3);
    assert(poe::simplest_fraction_between(1, 2, 2, 1).denominator == 1);
    const auto result = poe::simplest_fraction_between(0, 1, 1, 2);
    assert(result.numerator == 1 && result.denominator == 3);
}
