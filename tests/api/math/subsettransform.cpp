#include <cassert>
#include <vector>

#include "math/modint.hpp"
#include "math/subsettransform.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    const std::vector<mint> left = {1, 2, 3, 4};
    const std::vector<mint> right = {5, 6, 7, 8};
    const auto product = poe::subset_convolution(left, right);
    assert((product == std::vector<mint>{5, 16, 22, 60}));

    const std::vector<mint> unit = {1, 2, 3, 4};
    const auto square = poe::subset_convolution_unit_power(unit, 2);
    assert((square == std::vector<mint>{1, 4, 6, 20}));

    std::vector<long long> all = {10, 6, 5, 2};
    assert((poe::exact_match_counts(all) == std::vector<long long>{1, 7, 2}));
    assert((poe::divisibility_match_counts(16, {4, 2, 3}) ==
            std::vector<long long>{5, 6, 4, 1}));
}
