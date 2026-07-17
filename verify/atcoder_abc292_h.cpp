#include <bits/stdc++.h>

#include "utility/presetsegtree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << fixed << setprecision(15);

    int n, q;
    long long border;
    cin >> n >> border >> q;
    vector<long long> values(n);
    for (auto& value : values) {
        cin >> value;
        value -= border;
    }
    point_set_prefix_sum_max data(values);
    while (q--) {
        int index;
        long long value;
        cin >> index >> value;
        data.set(index - 1, value - border);
        const int first = data.first_prefix_at_least(0);
        const int length = min(first + 1, n);
        cout << static_cast<long double>(data.sum(length) + border * length) / length
             << '\n';
    }
}
