#include <cassert>
#include <vector>

#include "dp/partition.hpp"

int main() {
    std::vector<std::vector<long long>> score{
        {1, 4, 2, 4, 3},
        {2, 3, 4, 2, 2},
        {3, 2, 4, 4, 3},
    };
    assert(poe::max_phase_partition(score) == 16);
    assert(poe::max_phase_partition(std::vector<std::vector<int>>{{-5, -1}}) == -6);
    const std::vector<std::vector<int>> xor_operation{{0, 1}, {1, 0}};
    assert(poe::parenthesized_results({1, 0, 1}, xor_operation) == 1ULL);
}
