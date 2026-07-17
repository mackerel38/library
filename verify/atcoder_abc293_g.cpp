#include <bits/stdc++.h>

#include "algorithm/rangefrequency.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> values(n);
    for (auto& value : values) cin >> value;
    vector<pair<int, int>> ranges(q);
    for (auto& [left, right] : ranges) {
        cin >> left >> right;
        --left;
    }
    for (const auto answer : equal_tuple_range_counts(values, ranges, 3)) {
        cout << answer << '\n';
    }
}
