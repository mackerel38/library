#include <bits/stdc++.h>
#include "math/rangeproductpower.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> values(n);
    for (int& value : values) cin >> value;
    vector<pair<int, int>> queries(q);
    for (auto& [left, right] : queries) {
        cin >> left >> right;
        --left;
    }
    for (const bool answer : range_product_power_queries(values, queries, 3)) {
        cout << (answer ? "Yes" : "No") << '\n';
    }
}
