#include "math/modint.hpp"
#include "math/randomwalk.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    assert(cyclic_hitting_expectation<mint>(100, 37, {1}) == 37);
    const mint half = mint{1} / 2;
    assert(cyclic_hitting_expectation<mint>(5, 1, {half, half}) == mint{34} / 11);
}
