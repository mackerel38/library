#include <cassert>
#include "algorithm/balancedsplit.hpp"

int main() {
    assert((poe::balanced_split_leaves(19, 9)
            == std::vector<std::pair<long long, long long>>{{4, 1}, {5, 3}}));
    const long double value = poe::kth_largest_after_max_halving(
        std::vector<int>{40, 20, 30}, 4, 5);
    assert(std::abs(value - 10) < 1e-12L);
}
