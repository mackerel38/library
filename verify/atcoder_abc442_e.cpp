#include <bits/stdc++.h>
#include "geometry/point.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, query_count;
    cin >> n >> query_count;
    vector<point<long long>> points(n);
    for (auto& [x, y] : points) cin >> x >> y;
    vector<int> order(n), position(n);
    iota(order.begin(), order.end(), 0);
    ranges::sort(order, [&](int left, int right) {
        return argument_less(points[right], points[left]);
    });
    for (int i = 0; i < n; ++i) position[order[i]] = i;
    vector<int> group_begin(n), group_end(n);
    for (int i = 0; i < n; ++i) {
        group_begin[i] = i > 0 && same_direction(points[order[i - 1]], points[order[i]])
            ? group_begin[i - 1] : i;
    }
    for (int i = n - 1; i >= 0; --i) {
        group_end[i] = i + 1 < n && same_direction(points[order[i]], points[order[i + 1]])
            ? group_end[i + 1] : i + 1;
    }
    while (query_count--) {
        int first, last;
        cin >> first >> last;
        const int left = group_begin[position[--first]];
        const int right = group_end[position[--last]];
        cout << (left < right ? right - left : n - left + right) << '\n';
    }
}
