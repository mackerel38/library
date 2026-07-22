#include "algorithm/dominance.hpp"

using namespace poe;

int main() {
    std::vector<weightedpoint3d<int, long long>> points{
        {1, 1, 3, 4},
        {2, 2, 2, 10},
        {3, 3, 3, 5},
        {4, 4, 4, 1}
    };
    const auto result = maximum_weight_dominance_chain_3d(points);
    assert(result.weight == 16);
    assert((result.indices == std::vector<int>{1, 2, 3}));
    assert(result.ending[0] == 4 && result.ending[2] == 15);
}
