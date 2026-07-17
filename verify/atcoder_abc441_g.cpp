#include <bits/stdc++.h>
#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    range_active_add_range_clear_flip_max<long long> data(n);
    while (q--) {
        int type, left, right;
        cin >> type >> left >> right;
        --left;
        if (type == 1) {
            long long value;
            cin >> value;
            data.add(left, right, value);
        } else if (type == 2) {
            data.clear_flip(left, right);
        } else {
            cout << data.max(left, right) << '\n';
        }
    }
}
