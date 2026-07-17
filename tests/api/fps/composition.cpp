#include <cassert>

#include "fps/composition.hpp"

int main() {
    using series = poe::fps998244353;
    const series f = {0, 1, 1};
    assert(poe::compose<998244353>(f, f, 5) == series({0, 1, 2, 2, 1}));
    assert(poe::functional_power<998244353>(f, 2, 5) == poe::compose<998244353>(f, f, 5));
}
