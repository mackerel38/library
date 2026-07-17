#include "fps/bostanmori.hpp"
#include "fps/rational.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    const vector<mint> numerator{1};
    const vector<mint> denominator{1, -1, -1};
    assert(bostan_mori_submask_sum<998244353>(numerator, denominator, 6) == 21);

    rationalfps<998244353> fibonacci{numerator, denominator};
    assert(fibonacci.submask_sum(8) == 35);
    assert(fibonacci.submask_sum(0) == 1);

    rationalfps<998244353> polynomial{{3, 4, 5}, {1}};
    assert(polynomial.submask_sum(2) == 8);
}
