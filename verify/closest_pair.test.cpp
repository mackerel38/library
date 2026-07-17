#define PROBLEM "https://judge.yosupo.jp/problem/closest_pair"
#include <bits/stdc++.h>
#include "geometry/closestpair.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n;
        cin >> n;
        vector<point<long long>> points(n);
        for (auto& current : points) cin >> current.x >> current.y;
        const auto result = closest_pair(points);
        cout << result.first << ' ' << result.second << '\n';
    }
}
