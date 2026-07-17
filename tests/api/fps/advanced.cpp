#include <cassert>

#include "fps/online.hpp"
#include "fps/rational.hpp"

int main() {
    using mint = poe::modint998244353;
    poe::rationalfps<998244353> fibonacci{{0, 1}, {1, -1, -1}};
    assert(fibonacci.coeff(10) == 55);
    assert(fibonacci.prefix(11)[10] == 55);

    const std::vector<mint> values = {2, 3, 5};
    const auto sums = poe::power_sums<998244353>(values, 4);
    assert(sums == poe::fps998244353({3, 10, 38, 160}));

    const std::vector<poe::weightedsumvariable> variables{{1, 1}, {1, 2}, {2}, {3}};
    assert(poe::count_weighted_sum<998244353>(5, variables) == 6);
    assert(poe::weighted_sum_rational<998244353>(variables).coeff(5) == 6);
    assert(poe::rational_product<998244353>({}).coeff(0) == 1);

    std::vector<mint> source(10), kernel(10);
    source[0] = 1;
    kernel[1] = kernel[2] = 1;
    const auto recurrence = poe::convolution_recurrence<998244353>(source, kernel);
    assert(recurrence[9] == 55);

    poe::relaxedconvolution<998244353> relaxed(4);
    assert(relaxed.append(1, 2) == 2);
    assert(relaxed.append(3, 5) == 11);
    assert(relaxed.append(4, 7) == 30);
    assert(relaxed.append(0, 11) == 52);
}
