#include <bits/stdc++.h>
#include "algorithm/rangesort.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q, target;
    cin >> n >> q >> target;
    vector<int> values(n);
    for (int& value : values) cin >> value;
    vector<rangesortquery> queries(q);
    for (auto& query : queries) {
        int type;
        cin >> type >> query.left >> query.right;
        --query.left;
        query.ascending = type == 1;
    }
    cout << position_after_range_sorts(values, target, queries) + 1 << '\n';
}
