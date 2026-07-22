#include "graph/spanningforest.hpp"
#include "math/modint.hpp"

using namespace poe;

int main() {
    using mint = modint998244353;
    const matrix<mint> path({{0, 2, 0}, {2, 0, 3}, {0, 3, 0}});
    const auto counts = spanning_forest_counts(path);
    assert(counts.size() == 3);
    assert(counts[0] == 1);
    assert(counts[1] == 5);
    assert(counts[2] == 6);

    const matrix<mint> empty(0, 0);
    assert(spanning_forest_counts(empty) == std::vector<mint>{1});
}
