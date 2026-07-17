#define PROBLEM "https://atcoder.jp/contests/abc425/tasks/abc425_d"
#include <bits/stdc++.h>
#include "graph/activation.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int height, width;
    cin >> height >> width;
    vector<string> grid(height);
    for (auto& row : grid) cin >> row;

    const auto result = grid_synchronous_activation(
        height, width,
        [&](int row, int column) { return grid[row][column] == '#'; },
        [](int, int, int active_neighbors) { return active_neighbors == 1; });
    cout << result.active_count << '\n';
}
