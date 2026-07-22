#define PROBLEM "https://atcoder.jp/contests/abc278/tasks/abc278_h"
#include <bits/stdc++.h>
#include "fps/stirling.hpp"
#include "math/qanalog.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, bit_count;
    cin >> n >> bit_count;
    using mint = modint998244353;

    const auto repeated =
        matrix_rowspace_avoiding_vector_counts<998244353>(n, bit_count, 2);
    const auto stirling = signed_stirling_first_row<998244353>(n - 1);
    mint previous = 0;
    mint current = 0;
    for (int used = 0; used <= n; ++used) {
        if (used < n) previous += stirling[used] * repeated[used];
        const mint coefficient = (used > 0 ? stirling[used - 1] : mint{}) -
                                 (used < n ? stirling[used] * (n - 1) : mint{});
        current += coefficient * repeated[used];
    }
    const mint remaining = mint{2}.pow(bit_count) - n + 1;
    cout << previous * remaining - current << '\n';
}
