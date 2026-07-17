#include <cassert>
#include <optional>
#include "algorithm/binarysearch.hpp"

using namespace poe;

int main() {
    for (int left = -100; left <= 100; ++left) {
        for (int right = left; right <= 100; ++right) {
            for (int boundary = left - 1; boundary <= right; ++boundary) {
                std::optional<int> expected_first;
                if (left < right && boundary + 1 < right) {
                    expected_first = std::max(left, boundary + 1);
                }
                assert(first_true(left, right, [&](int value) { return boundary < value; }) ==
                       expected_first);

                std::optional<int> expected_last;
                if (left < right && left <= boundary) expected_last = std::min(right - 1, boundary);
                assert(last_true(left, right, [&](int value) { return value <= boundary; }) ==
                       expected_last);
            }
        }
    }
    for (int center = -20; center <= 20; ++center) {
        const auto result = minimize_convex_real(
            -100.0L, 100.0L,
            [center](long double value) {
                return std::abs(value - center) + std::abs(value - center - 3);
            }
        );
        assert(center - 1e-12L <= result.argument);
        assert(result.argument <= center + 3 + 1e-12L);
        assert(std::abs(result.value - 3) < 1e-10L);
    }
}
