#include <bits/stdc++.h>
#include "algorithm/intervalcover.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<pair<int, int>> intervals(m);
    for (auto& [left, right] : intervals) {
        cin >> left >> right;
        --left;
    }
    cout << expected_random_interval_cover_time(n, intervals).val() << '\n';
}
