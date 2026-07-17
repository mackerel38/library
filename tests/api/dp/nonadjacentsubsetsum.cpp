#include <algorithm>
#include <cassert>
#include <vector>

#include "dp/subsetsum.hpp"

int main() {
    const auto sums = poe::nonadjacent_subset_sums(std::vector<int>{1, 2, 3}, 5);
    auto all = sums.all;
    std::ranges::sort(all);
    assert(all == std::vector<int>({0, 1, 2, 3, 4}));
    assert(poe::count_nonadjacent_subsets_mod(std::vector<int>{1, 2, 3}, 5, 4) == 1);
}
