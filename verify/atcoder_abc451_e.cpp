#include <bits/stdc++.h>

#include "tree/treemetric.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector distance(n, vector<long long>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            cin >> distance[i][j];
            distance[j][i] = distance[i][j];
        }
    }
    cout << (is_tree_metric(distance) ? "Yes" : "No") << '\n';
}
