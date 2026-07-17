#include <bits/stdc++.h>
#include "flow/lowerbound.hpp"

using namespace std;
using namespace poe;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int height, width;
    cin >> height >> width;
    vector<string> cell(height);
    for (auto& row : cell) cin >> row;
    const int source = height * width;
    const int sink = source + 1;
    lowerboundflow<int> flow(sink + 1);
    auto bounds = [](char type) {
        if (type == '2') return pair{1, 1};
        if (type == '?') return pair{0, 1};
        return pair{0, 0};
    };
    constexpr int dr[] = {-1, 1, 0, 0};
    constexpr int dc[] = {0, 0, -1, 1};
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            const int vertex = row * width + column;
            const auto [lower, upper] = bounds(cell[row][column]);
            if ((row + column) % 2 == 0) {
                flow.add_edge(source, vertex, lower, upper);
                for (int direction = 0; direction < 4; ++direction) {
                    const int next_row = row + dr[direction];
                    const int next_column = column + dc[direction];
                    if (0 <= next_row && next_row < height &&
                        0 <= next_column && next_column < width) {
                        flow.add_edge(vertex, next_row * width + next_column, 0, 1);
                    }
                }
            } else {
                flow.add_edge(vertex, sink, lower, upper);
            }
        }
    }
    cout << (flow.maxflow(source, sink).has_value() ? "Yes" : "No") << '\n';
}
