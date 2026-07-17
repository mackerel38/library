#define PROBLEM "https://atcoder.jp/contests/abc426/tasks/abc426_f"
#include <bits/stdc++.h>
#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> stock(n);
    for (auto& value : stock) cin >> value;
    range_subtract_clamp_zero_sum<long long> data(stock);

    int q;
    cin >> q;
    while (q--) {
        int left, right;
        long long count;
        cin >> left >> right >> count;
        cout << data.subtract(left - 1, right, count) << '\n';
    }
}
