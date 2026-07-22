#include <bits/stdc++.h>
#include "geometry/distancepairs.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long radius;
    cin >> n >> radius;
    vector<point<long long>> points(n);
    for (auto& point : points) cin >> point.x >> point.y;
    const auto pairs = pairs_within_distance(points, radius);
    cout << pairs.size() << '\n';
    for (const auto& [left, right] : pairs) cout << left + 1 << ' ' << right + 1 << '\n';
}
