#include <cassert>

#include "fps/sparse.hpp"
#include "fps/transform.hpp"

int main() {
    using fps = poe::fps998244353;
    using mint = poe::modint998244353;
    const fps f = {0, 1, 0, 1};
    assert(poe::sparse_power<998244353>(f, 3, 10) == f.pow(3, 10));

    fps series = {1, 2, 3, 4, 5};
    const fps divided = poe::divide_binomial(series, 2, mint{-1}, 5);
    assert(poe::multiply_binomial(divided, 2, mint{-1}, 5) == series);

    assert(poe::binomial_transform<998244353>(fps{1, 2, 3}, mint{1}) == fps({1, 3, 8}));
    assert(
        poe::upper_binomial_transform<998244353>(fps{1, 2, 3}, mint{1})
        == fps({6, 8, 3})
    );
}
