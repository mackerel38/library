#include <bits/stdc++.h>

#include "math/modint.hpp"
#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> edges(m);
    for (auto& [from, to] : edges) {
        cin >> from >> to;
        --from;
        --to;
    }
    ranges::sort(edges);
    vector<modint998244353> initial(n);
    initial[0] = 1;
    range_affine_range_sum<modint998244353> dp(initial);
    for (auto [from, to] : edges) {
        const auto extend = dp.sum(from, to + 1);
        if (to + 1 < n) dp.affine(to + 1, n, 2, 0);
        dp.set(to, dp[to] + extend);
    }
    cout << dp[n - 1].val() << '\n';
}
