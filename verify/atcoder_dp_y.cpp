#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_y"
#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    int height, width, obstacles;
    cin >> height >> width >> obstacles;
    vector<pair<int, int>> point(obstacles + 1);
    for (int i = 0; i < obstacles; ++i) cin >> point[i].first >> point[i].second;
    point.back() = {height, width};
    ranges::sort(point);
    using mint = modint1000000007;
    combination<mint> comb(height + width);
    vector<mint> ways(point.size());
    for (int i = 0; i < static_cast<int>(point.size()); ++i) {
        const auto [row, column] = point[i];
        ways[i] = comb.choose(row + column - 2, row - 1);
        for (int j = 0; j < i; ++j) {
            const auto [previous_row, previous_column] = point[j];
            if (previous_column > column) continue;
            ways[i] -= ways[j] * comb.choose(row - previous_row + column - previous_column, row - previous_row);
        }
    }
    cout << ways.back().val() << '\n';
}
