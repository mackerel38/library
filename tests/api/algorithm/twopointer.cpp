#include <cassert>
#include <vector>

#include "algorithm/twopointer.hpp"

int main() {
    assert(poe::count_subarrays_inversions(std::vector<int>{3, 1, 2}, 0) == 4);
    assert(poe::count_subarrays_inversions(std::vector<int>{3, 1, 2}, 1) == 1);
    assert(poe::count_subarrays_inversions(std::vector<int>{3, 1, 2}, 2) == 1);
    assert(poe::count_subarrays_inversions(std::vector<int>{2, 2, 1}, 1) == 1);
    assert(poe::count_subarrays_sum_nonnegative(std::vector<int>{0, 0}, 0) == 3);
    assert(poe::count_subarrays_sum_nonnegative(std::vector<int>{1, 2, 1, 1}, 3) == 2);
    assert(poe::count_subrectangles_sum_nonnegative(
        std::vector<std::vector<int>>{{1, 0}, {0, 1}}, 1
    ) == 6);
    assert(poe::count_subarrays_frequency_constraints(
        std::string("abbaaabaaba"), 4, 2,
        [](char c) { return c == 'a'; }, [](char c) { return c == 'b'; }
    ) == 3);
    const auto windows = poe::circular_threshold_windows(std::vector<int>{3, 1, 1}, 2);
    assert(windows[0].length == 1 && windows[0].sum == 3);
    assert(windows[1].length == 2 && windows[1].sum == 2);
    assert(windows[2].length == 2 && windows[2].sum == 4);
}
