#include <cassert>
#include <cmath>
#include "algorithm/binarysearch.hpp"

using namespace poe;

int main() {
    assert(first_true(0, 20, [](int value) { return value * value >= 50; }) == 8);
    assert(last_true(0, 20, [](int value) { return value * value <= 50; }) == 7);
    assert(!first_true(0, 10, [](int) { return false; }));
    assert(!last_true(0, 10, [](int) { return false; }));
    const double root = binary_search_real(
        0.0, 10.0, [](double value) { return value * value >= 2.0; });
    assert(std::abs(root - std::sqrt(2.0)) < 1e-12);
    const auto minimum = minimize_convex_real(
        -10.0, 10.0, [](double value) { return (value - 3) * (value - 3) + 5; });
    assert(std::abs(minimum.argument - 3.0) < 1e-7);
    assert(std::abs(minimum.value - 5.0) < 1e-10);
}
