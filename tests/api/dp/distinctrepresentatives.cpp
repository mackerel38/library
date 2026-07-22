#include <bits/stdc++.h>
#include "dp/distinctrepresentatives.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    const vector<unsigned> sets = {0b0111, 0b1100, 0b1010};
    const auto count = count_distinct_representatives<mint>(3, [&](int mask) {
        unsigned intersection = 0b1111;
        for (int index = 0; index < 3; ++index) {
            if (mask >> index & 1) intersection &= sets[index];
        }
        return popcount(intersection);
    });
    assert(count == 5);
    assert(count_distinct_bounded_multiples<mint>(7, {2, 3, 4}) == 3);
}
