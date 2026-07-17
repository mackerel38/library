#define PROBLEM "https://atcoder.jp/contests/abc413/tasks/abc413_g"
#include <bits/stdc++.h>
#include "graph/grid.hpp"

using namespace std;
using namespace poe;

int main() {
    int height, width, count;
    cin >> height >> width >> count;
    vector<gridpoint> blocked(count);
    for (auto& [row, column] : blocked) {
        cin >> row >> column;
        --row;
        --column;
    }
    cout << (sparse_grid_path_exists(height, width, blocked) ? "Yes" : "No") << '\n';
}
