#define PROBLEM "https://atcoder.jp/contests/abc314/tasks/abc314_h"
#include <bits/stdc++.h>
#include "algorithm/binarysearch.hpp"
#include "geometry/point.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<pair<point<long double>, point<long double>>> segments(n);
    for (auto& [first, second] : segments) {
        cin >> first.x >> first.y >> second.x >> second.y;
    }
    auto radius = [&](long double x, long double y) {
        long double result = 0;
        for (const auto& [first, second] : segments) {
            result = max(result, distance_to_segment(point<long double>{x, y}, first, second));
        }
        return result;
    };
    const auto answer = minimize_convex_real(
        0.0L, 1000.0L,
        [&](long double x) {
            return minimize_convex_real(
                0.0L, 1000.0L,
                [&](long double y) { return radius(x, y); }
            ).value;
        }
    );
    cout << fixed << setprecision(15) << answer.value << '\n';
}
