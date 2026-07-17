#include <cassert>
#include <vector>

#include "algorithm/rangemin.hpp"

int main() {
    using query = poe::rangeminsumquery<long long>;
    const std::vector<long long> values{5, 1, 8, 3};
    const auto answer = poe::offline_range_min_sum(
        values, std::vector<query>{{0, 4, 4}, {1, 3, 10}, {2, 2, -5}});
    assert(answer == std::vector<long long>({12, 9, 0}));
}
