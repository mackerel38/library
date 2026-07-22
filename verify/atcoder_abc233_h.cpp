#define PROBLEM "https://atcoder.jp/contests/abc233/tasks/abc233_h"

#include <bits/stdc++.h>

#include "geometry/manhattan.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<pair<long long, long long>> points(n);
    for (auto& [x, y] : points) cin >> x >> y;
    manhattannearest near(points);
    int q;
    cin >> q;
    while (q--) {
        long long x, y;
        int k;
        cin >> x >> y >> k;
        cout << near.kth(x, y, k) << '\n';
    }
}
