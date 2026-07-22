#include <cassert>
#include <vector>
#include "algorithm/subarrayrange.hpp"

int main() {
    assert(poe::count_subarrays_range_excess_at_most(std::vector<int>{1, 4, 2, 3}, 1) == 9);
}
