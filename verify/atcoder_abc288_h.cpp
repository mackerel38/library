#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/xorcount.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    unsigned long long maximum, target;
    cin >> n >> maximum >> target;
    using mint = modint998244353;
    const auto tuple = bounded_xor_tuple_counts<mint>(n, maximum, target);
    const auto distinct = distinct_xor_tuple_counts(tuple, maximum + 1);
    cout << xor_multiset_count(n, maximum + 1, distinct) << '\n';
}
