#include <cassert>
#include <utility>
#include <vector>
#include "math/binomialsequence.hpp"

int main() {
    const std::vector<std::pair<int, long long>> runs{{2, 3}, {5, 2}, {1, 1}};
    const std::vector<std::pair<int, long long>> expected{{1, 1}, {4, 1}, {3, 1}, {2, 1}};
    assert(poe::adjacent_sum_reduce_rle<7>(runs, 2) == expected);
}
