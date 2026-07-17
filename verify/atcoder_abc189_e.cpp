#define PROBLEM "https://atcoder.jp/contests/abc189/tasks/abc189_e"
#include <bits/stdc++.h>
#include "geometry/affine.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<point<long long>> points(n);
    for (auto& [x, y] : points) cin >> x >> y;
    int operations;
    cin >> operations;
    vector<affine<long long>> prefix(operations + 1);
    for (int i = 1; i <= operations; ++i) {
        int type;
        cin >> type;
        affine<long long> current;
        if (type == 1) current = affine<long long>::rotate_clockwise();
        if (type == 2) current = affine<long long>::rotate_counterclockwise();
        if (type == 3) { long long p; cin >> p; current = affine<long long>::reflect_x(p); }
        if (type == 4) { long long p; cin >> p; current = affine<long long>::reflect_y(p); }
        prefix[i] = current * prefix[i - 1];
    }
    int queries;
    cin >> queries;
    while (queries--) {
        int count, index;
        cin >> count >> index;
        const auto answer = prefix[count](points[index - 1]);
        cout << answer.x << ' ' << answer.y << '\n';
    }
}
