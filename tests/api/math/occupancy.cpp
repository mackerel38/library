#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/occupancy.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    assert(count_nonempty_subset_occupancies<mint>(2, {2, 1, 1}) == 21);
    assert(count_nonempty_subset_occupancies<mint>(2, {0, 0, 0}) == 0);
    assert(count_nonempty_subset_occupancies<mint>(0, {}) == 1);
}
