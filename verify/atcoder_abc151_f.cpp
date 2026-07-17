#define PROBLEM "https://atcoder.jp/contests/abc151/tasks/abc151_f"
#include <bits/stdc++.h>
#include "geometry/circle.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<point<long long>> points(n);
    for (auto& [x, y] : points) cin >> x >> y;
    cout << fixed << setprecision(15) << minimum_enclosing_circle(points).radius << '\n';
}
