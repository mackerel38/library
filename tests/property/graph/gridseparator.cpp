#include <bits/stdc++.h>
#include "graph/gridseparator.hpp"

using namespace std;
using namespace poe;

pair<int, int> brute(
    const vector<vector<bool>>& removable,
    pair<int, int> start,
    pair<int, int> goal
) {
    const int height = removable.size(), width = removable[0].size();
    vector<pair<int, int>> choice;
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            if (removable[row][column]) choice.push_back({row, column});
        }
    }
    int best = numeric_limits<int>::max(), ways = 0;
    for (int mask = 0; mask < (1 << choice.size()); ++mask) {
        const int used = popcount(static_cast<unsigned>(mask));
        if (used > best) continue;
        vector blocked(height, vector<bool>(width));
        for (int i = 0; i < static_cast<int>(choice.size()); ++i) {
            if (mask >> i & 1) blocked[choice[i].first][choice[i].second] = true;
        }
        queue<pair<int, int>> queue;
        vector reached(height, vector<bool>(width));
        queue.push(start);
        reached[start.first][start.second] = true;
        constexpr int dr[4] = {-1, 0, 1, 0};
        constexpr int dc[4] = {0, 1, 0, -1};
        while (!queue.empty()) {
            const auto [row, column] = queue.front();
            queue.pop();
            for (int direction = 0; direction < 4; ++direction) {
                const int next_row = row + dr[direction];
                const int next_column = column + dc[direction];
                if (
                    next_row < 0 || height <= next_row || next_column < 0
                    || width <= next_column || blocked[next_row][next_column]
                    || reached[next_row][next_column]
                ) {
                    continue;
                }
                reached[next_row][next_column] = true;
                queue.push({next_row, next_column});
            }
        }
        if (reached[goal.first][goal.second]) continue;
        if (used < best) best = used, ways = 1;
        else ++ways;
    }
    if (best == numeric_limits<int>::max()) return {-1, 0};
    return {best, ways};
}

int main() {
    for (int height = 2; height <= 3; ++height) {
        for (int width = 2; width <= 3; ++width) {
            const int cells = height * width;
            for (int start_id = 0; start_id < cells; ++start_id) {
                for (int goal_id = start_id + 1; goal_id < cells; ++goal_id) {
                    const pair start{start_id / width, start_id % width};
                    const pair goal{goal_id / width, goal_id % width};
                    vector<pair<int, int>> choice;
                    for (int id = 0; id < cells; ++id) {
                        if (id != start_id && id != goal_id) {
                            choice.push_back({id / width, id % width});
                        }
                    }
                    for (int available = 0; available < (1 << choice.size()); ++available) {
                        vector removable(height, vector<bool>(width));
                        for (int i = 0; i < static_cast<int>(choice.size()); ++i) {
                            if (available >> i & 1) {
                                removable[choice[i].first][choice[i].second] = true;
                            }
                        }
                        const auto expected = brute(removable, start, goal);
                        const auto actual = minimum_grid_vertex_separator(removable, start, goal);
                        assert(actual.size == expected.first);
                        assert(actual.count.val() == static_cast<unsigned>(expected.second));
                    }
                }
            }
        }
    }
}
