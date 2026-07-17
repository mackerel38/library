#include <bits/stdc++.h>
#include "graph/shortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr array<int, 4> dr{1, 0, -1, 0};
    constexpr array<int, 4> dc{0, 1, 0, -1};
    int tests;
    cin >> tests;
    while (tests--) {
        int height, width;
        cin >> height >> width;
        vector<string> grid(height);
        for (string& row : grid) cin >> row;
        const int source = 4 * height * width;
        const int target = source + 1;
        const int vertex_count = target + 1;
        const auto state = [&](int row, int column, int direction) {
            return 4 * (row * width + column) + direction;
        };
        const auto mirror = [&](char type) {
            if (type == 'A') return 0;
            if (type == 'B') return 1;
            return 3;
        };
        const auto result = zero_one_bfs(vertex_count, source, [&](int vertex, auto relax) {
            if (vertex == target) return;
            int row = 0, column = 0, incoming = 1;
            if (vertex != source) {
                const int cell = vertex / 4;
                incoming = vertex % 4;
                row = cell / width + dr[incoming];
                column = cell % width + dc[incoming];
                if (!(0 <= row && row < height && 0 <= column && column < width)) {
                    if (cell / width == height - 1 && cell % width == width - 1
                        && incoming == 1) {
                        relax(target, 0);
                    }
                    return;
                }
            }
            for (int outgoing = 0; outgoing < 4; ++outgoing) {
                if ((incoming ^ outgoing) == 2) continue;
                const int cost = (incoming ^ outgoing) == mirror(grid[row][column]) ? 0 : 1;
                relax(state(row, column, outgoing), cost);
            }
        });
        cout << result[target] << '\n';
    }
}
