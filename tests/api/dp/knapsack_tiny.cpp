#include <cassert>
#include <vector>
#include "dp/knapsack.hpp"

using namespace poe;

int main() {
    const std::vector<int> weight{3, 1, 2, 2};
    const std::vector<long long> value{5, 2, 7, 1};
    const std::vector<long long> count{5, 4, 1, 2};
    assert(bounded_knapsack_tiny_weights(weight, value, count, 7) == 16);
}
