#include <bits/stdc++.h>
#include "graph/matching.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, a, b;
    cin >> n >> a >> b;
    vector<string> board(n);
    for (string& row : board) cin >> row;

    const int divisor = gcd(a, b);
    const int reduced_a = a / divisor;
    const int reduced_b = b / divisor;
    auto color = [&](int row, int column) {
        if ((reduced_a & 1) && (reduced_b & 1)) return (row / divisor) & 1;
        return ((row / divisor) + (column / divisor)) & 1;
    };

    vector<vector<int>> id(n, vector<int>(n, -1));
    array<int, 2> size{};
    for (int row = 0; row < n; ++row) {
        for (int column = 0; column < n; ++column) {
            if (board[row][column] == '.') {
                const int side = color(row, column);
                id[row][column] = size[side]++;
            }
        }
    }

    bipartitematching graph(size[0], size[1]);
    const array<int, 8> dr{a, b, b, a, -a, -b, -b, -a};
    const array<int, 8> dc{b, a, -a, -b, -b, -a, a, b};
    for (int row = 0; row < n; ++row) {
        for (int column = 0; column < n; ++column) {
            if (board[row][column] != '.' || color(row, column) != 0) continue;
            for (int direction = 0; direction < 8; ++direction) {
                const int next_row = row + dr[direction];
                const int next_column = column + dc[direction];
                if (0 <= next_row && next_row < n && 0 <= next_column && next_column < n &&
                    board[next_row][next_column] == '.') {
                    graph.add_edge(id[row][column], id[next_row][next_column]);
                }
            }
        }
    }

    const auto result = graph.solve();
    vector<bool> take_left(size[0]), take_right(size[1]);
    for (int vertex : result.left_independent()) take_left[vertex] = true;
    for (int vertex : result.right_independent()) take_right[vertex] = true;
    for (int row = 0; row < n; ++row) {
        for (int column = 0; column < n; ++column) {
            if (board[row][column] != '.') continue;
            const int side = color(row, column);
            if ((side == 0 ? take_left : take_right)[id[row][column]]) {
                board[row][column] = 'o';
            }
        }
    }
    for (const string& row : board) cout << row << '\n';
}
