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
    const std::vector<edge> spectrum_edges{
        {0, 0, 100}, {0, 1, 10}, {1, 0, 1}
    };
    assert((poe::maximum_weight_bipartite_matching_values<long long>(
        2, 2, spectrum_edges
    ) == std::vector<long long>{0, 100, 11}));
    assert((poe::minimum_weight_bipartite_matching_values<long long>(
        2, 2, spectrum_edges
    ) == std::vector<long long>{0, 1, 11}));
}
