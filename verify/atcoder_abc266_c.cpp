#define PROBLEM "https://atcoder.jp/contests/abc266/tasks/abc266_c"
#include <bits/stdc++.h>
#include "geometry/polygon.hpp"

using namespace std;
using namespace poe;

int main() {
    vector<point<long long>> polygon(4);
    for (auto& [x, y] : polygon) cin >> x >> y;
    cout << (is_convex(polygon) ? "Yes" : "No") << '\n';
}
