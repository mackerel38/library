#include <bits/stdc++.h>
#include "graph/gridseparator.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int height, width;
    cin >> height >> width;
    vector<string> grid(height);
    for (auto& row : grid) cin >> row;
    vector removable(height, vector<bool>(width));
    pair<int, int> start, goal;
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            if (grid[row][column] == '.') removable[row][column] = true;
            if (grid[row][column] == 'S') start = {row, column};
            if (grid[row][column] == 'G') goal = {row, column};
        }
    }
    const auto result = minimum_grid_vertex_separator(removable, start, goal);
    if (!result.possible()) {
        cout << "No\n";
        return 0;
    }
    cout << "Yes\n" << result.size << ' ' << result.count.val() << '\n';
}
