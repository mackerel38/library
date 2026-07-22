#include <bits/stdc++.h>
#include "dp/gridpath.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<vector<gridpathpoint>> groups(n * n + 1);
    for (int row = 0; row < n; ++row) {
        for (int column = 0, label; column < n; ++column) {
            cin >> label;
            groups[label].push_back({row, column});
        }
    }
    cout << count_grouped_monotone_grid_paths<998244353>(n, n, groups) << '\n';
}
