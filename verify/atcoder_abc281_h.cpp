#define PROBLEM "https://atcoder.jp/contests/abc281/tasks/abc281_h"
#include <bits/stdc++.h>
#include "fps/online.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long kinds;
    cin >> n >> kinds;
    using mint = modint998244353;
    vector<mint> base(n + 1);
    base[0] = 1;
    for (int degree = 1; degree <= n; ++degree) {
        base[degree] = base[degree - 1] * mint{kinds - degree + 1} / mint{degree};
    }
    const auto answer = online_factor_product_coefficients<998244353>(
        std::move(base), min(2, n + 1));
    cout << answer[n] << '\n';
}
