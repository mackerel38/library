#define PROBLEM "https://atcoder.jp/contests/abc460/tasks/abc460_d"
#include <bits/stdc++.h>
#include "graph/grid.hpp"

using namespace std;
using namespace poe;

int main() {
    int height, width;
    cin >> height >> width;
    vector<string> grid(height);
    for (auto& row : grid) cin >> row;

    vector<string> after_one(height, string(width, '.'));
    vector<gridpoint> sources;
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            if (grid[row][column] == '#') continue;
            for (const auto direction : grid8) {
                const int next_row = row + direction.row;
                const int next_column = column + direction.column;
                if (inside_grid(next_row, next_column, height, width) &&
                    grid[next_row][next_column] == '#') {
                    after_one[row][column] = '#';
                    break;
                }
            }
            if (after_one[row][column] == '#') sources.push_back({row, column});
        }
    }
    const auto distance = gridbfs(
        height, width, sources, [](int, int) { return true; }, grid8
    );
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            cout << (distance[row][column] >= 0 && distance[row][column] % 2 == 1 ? '#' : '.');
        }
        cout << '\n';
    }
}
