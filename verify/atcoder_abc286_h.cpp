#define PROBLEM "https://atcoder.jp/contests/abc286/tasks/abc286_h"
#include <bits/stdc++.h>
#include "geometry/convexpath.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<point<long long>> polygon(n);
    for (auto& [x, y] : polygon) cin >> x >> y;
    point<long long> start, goal;
    cin >> start.x >> start.y;
    cin >> goal.x >> goal.y;
    cout << fixed << setprecision(15)
         << shortest_path_avoiding_convex_polygon(polygon, start, goal) << '\n';
}
