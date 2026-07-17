#include <cassert>

#include "math/floorproduct.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    using monoid = powersummonoid<mint>;
    const auto result = floor_product(4ULL, 5ULL, 2ULL, 1ULL, monoid::x(mint{2}), monoid::y(mint{3}));
    mint expected = 0;
    for (int i = 0; i < 4; ++i) expected += mint{2}.pow(i) * mint{3}.pow((2 * i + 1) / 5);
    assert(result.answer() == expected);
}
