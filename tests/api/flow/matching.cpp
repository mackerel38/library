#include <cassert>
#include <vector>

#include "flow/matching.hpp"

int main() {
    using edge = poe::weightedbipartiteedge<long long>;
    assert(poe::minimum_weight_bipartite_matching<long long>(
        2, 2, std::vector<edge>{{0, 0, -5}, {0, 1, -4}, {1, 0, -3}, {1, 1, 2}}
    ) == -7);
    assert(poe::minimum_weight_bipartite_matching<long long>(
        1, 1, std::vector<edge>{{0, 0, 4}}
    ) == 0);
}
