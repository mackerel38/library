#include <cassert>

#include "fps/fps.hpp"

int main() {
    using fps = poe::fps998244353;
    using mint = poe::modint998244353;

    fps f = {1, 2, 3};
    assert(f[1] == 2);
    assert(f.coeff(10) == 0);
    assert((f * fps{1, 1}) == fps({1, 3, 5, 3}));
    assert(f.diff() == fps({2, 6}));
    assert(f.diff().integral() == fps({0, 2, 3}));

    fps inverse = f.inv(8);
    assert((f * inverse).prefix(8) == fps({1, 0, 0, 0, 0, 0, 0, 0}));

    fps logarithm = f.log(8);
    assert(logarithm.exp(8) == f.prefix(8));

    fps g = {0, 1, 2, 3};
    assert(g.exp(8).log(8) == g.prefix(8));
    assert(f.pow(3, 8) == (f * f * f).prefix(8));
    assert(f.eval(mint{2}) == 17);
}
