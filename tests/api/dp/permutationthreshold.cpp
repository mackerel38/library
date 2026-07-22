#include "dp/permutationthreshold.hpp"

int main() {
    using mint = poe::modint998244353;
    assert((poe::permutation_threshold_counts<998244353>({0, 2}) ==
            std::vector<mint>{0, 2, 0}));
    assert((poe::permutation_threshold_counts<998244353>({}) ==
            std::vector<mint>{1}));
}
