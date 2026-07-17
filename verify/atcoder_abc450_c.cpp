#include <bits/stdc++.h>

#include "graph/grid.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int height, width;
    cin >> height >> width;
    vector<string> grid(height);
    for (auto& row : grid) cin >> row;
    auto components = grid_components(height, width, [&](int row, int column) {
        return grid[row][column] == '.';
    });
    cout << ranges::count(components.touches_border, false) << '\n';
}
