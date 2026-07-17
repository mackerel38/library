#define PROBLEM "https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum"
#include <bits/stdc++.h>
#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, query_count;
    cin >> n >> query_count;
    vector<long long> values(n);
    for (long long& value : values) cin >> value;
    range_chmin_chmax_add_range_sum<long long> data(values);
    while (query_count--) {
        int type, left, right;
        cin >> type >> left >> right;
        if (type == 0) {
            long long value;
            cin >> value;
            data.chmin(left, right, value);
        } else if (type == 1) {
            long long value;
            cin >> value;
            data.chmax(left, right, value);
        } else if (type == 2) {
            long long value;
            cin >> value;
            data.add(left, right, value);
        } else {
            cout << data.sum(left, right) << '\n';
        }
    }
}
