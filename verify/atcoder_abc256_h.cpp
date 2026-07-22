#include <bits/stdc++.h>
#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> values(n);
    for (auto& value : values) cin >> value;
    range_divide_set_range_sum<long long> data(values);

    while (q--) {
        int type, left, right;
        cin >> type >> left >> right;
        --left;
        if (type == 1) {
            long long divisor;
            cin >> divisor;
            data.divide(left, right, divisor);
        } else if (type == 2) {
            long long value;
            cin >> value;
            data.set(left, right, value);
        } else {
            cout << data.sum(left, right) << '\n';
        }
    }
}
