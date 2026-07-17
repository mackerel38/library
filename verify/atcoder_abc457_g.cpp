#include <bits/stdc++.h>
#include "dp/sequence.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<int, int>> points(n);
    for (auto& [u, v] : points) {
        int time, coordinate;
        cin >> time >> coordinate;
        u = time + coordinate;
        v = time - coordinate;
    }
    ranges::sort(points);
    vector<int> reversed_v(n);
    for (int i = 0; i < n; ++i) reversed_v[i] = -points[i].second;
    cout << lis(reversed_v).indices.size() << '\n';
}
