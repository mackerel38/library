#include <cassert>
#include <vector>

#include "algorithm/rangefrequency.hpp"

int main() {
    const std::vector<int> values = {2, 7, 1, 8, 2, 8, 1, 8, 2, 8};
    const std::vector<std::pair<int, int>> ranges = {{0, 10}, {0, 9}, {1, 10}, {4, 5}};
    assert((poe::equal_tuple_range_counts(values, ranges, 3) ==
            std::vector<long long>{5, 2, 4, 0}));
    assert((poe::equal_tuple_range_counts(values, {{0, 4}}, 0) ==
            std::vector<long long>{1}));
    assert((poe::equal_tuple_range_counts(values, {{0, 4}}, 1) ==
            std::vector<long long>{4}));
    assert((poe::equal_tuple_range_counts(values, {{0, 4}}, 2) ==
            std::vector<long long>{0}));
}
