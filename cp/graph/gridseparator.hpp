#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// 最小頂点separatorの大きさと、その最適集合数を表す。
template<int mod>
struct gridseparatorresult {
    int size = -1;
    staticmodint<mod> count = 0;

    /// O(1)。指定可能マスだけで分離できるならtrueを返す。
    bool possible() const noexcept { return size != -1; }
};

/// O((h+w)(hw+(h+w)^2))時間・O(hw+(h+w)^2)領域。4近傍格子の最小頂点separatorと個数を返す。
template<int mod = 998244353>
gridseparatorresult<mod> minimum_grid_vertex_separator(
    const std::vector<std::vector<bool>>& removable,
    std::pair<int, int> start,
    std::pair<int, int> goal
) {
    using mint = staticmodint<mod>;
    static_assert(mod % 2 == 1);
    const int height = static_cast<int>(removable.size());
    assert(height >= 1);
    const int width = static_cast<int>(removable[0].size());
    assert(width >= 1);
    for (const auto& row : removable) assert(static_cast<int>(row.size()) == width);
    auto inside = [&](int row, int column) {
        return 0 <= row && row < height && 0 <= column && column < width;
    };
    assert(inside(start.first, start.second));
    assert(inside(goal.first, goal.second));
    assert(start != goal);
    assert(!removable[start.first][start.second]);
    assert(!removable[goal.first][goal.second]);

    if (start.first > goal.first) std::swap(start, goal);
    std::vector<std::vector<bool>> can_remove = removable;
    if (start.second > goal.second) {
        for (auto& row : can_remove) std::reverse(row.begin(), row.end());
        start.second = width - 1 - start.second;
        goal.second = width - 1 - goal.second;
    }

    const int padded_width = width + 2;
    auto padded_id = [&](int row, int column) {
        return row * padded_width + column;
    };
    std::vector<int> color((height + 2) * (width + 2));
    const int sr = start.first + 1, sc = start.second + 1;
    const int gr = goal.first + 1, gc = goal.second + 1;
    for (int column = sc; column <= gc; ++column) {
        color[padded_id(sr, column)] = 1;
    }
    for (int row = sr; row <= gr; ++row) color[padded_id(row, gc)] = 1;
    for (int column = sc; column <= gc + 1; ++column) {
        color[padded_id(sr - 1, column)] = 2;
    }
    for (int row = sr - 1; row <= gr; ++row) color[padded_id(row, gc + 1)] = 2;

    const int cells = height * width;
    std::vector<int> vertex(cells, -1);
    std::vector<std::pair<int, int>> position;
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            if (!can_remove[row][column]) continue;
            vertex[row * width + column] = static_cast<int>(position.size());
            position.push_back({row, column});
        }
    }
    const int vertices = static_cast<int>(position.size());
    std::vector<std::vector<std::pair<int, int>>> graph(vertices);
    auto is_boundary = [&](int row, int column) {
        return row == 0 || row + 1 == height || column == 0 || column + 1 == width;
    };
    auto boundary_tag = [&](int row, int column) {
        return color[padded_id(row + 1, column + 1)] == 1
            && (row == 0 || column + 1 == width);
    };
    auto add_edge = [&](int left, int right, int parity) {
        graph[left].push_back({right, parity});
        graph[right].push_back({left, parity});
    };

    constexpr int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
        {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };
    for (int left = 0; left < vertices; ++left) {
        const auto [row, column] = position[left];
        for (const auto& direction : directions) {
            const int next_row = row + direction[0];
            const int next_column = column + direction[1];
            if (!inside(next_row, next_column)) continue;
            const int right = vertex[next_row * width + next_column];
            if (right <= left) continue;
            const int parity =
                color[padded_id(row + 1, column + 1)]
              + color[padded_id(next_row + 1, next_column + 1)] == 3;
            if (
                is_boundary(row, column) && is_boundary(next_row, next_column)
                && parity == (boundary_tag(row, column)
                              ^ boundary_tag(next_row, next_column))
            ) {
                continue;
            }
            add_edge(left, right, parity);
        }
    }
    std::vector<int> boundary;
    for (int v = 0; v < vertices; ++v) {
        if (is_boundary(position[v].first, position[v].second)) boundary.push_back(v);
    }
    for (int i = 0; i < static_cast<int>(boundary.size()); ++i) {
        const auto [left_row, left_column] = position[boundary[i]];
        for (int j = i + 1; j < static_cast<int>(boundary.size()); ++j) {
            const auto [right_row, right_column] = position[boundary[j]];
            add_edge(boundary[i], boundary[j],
                     boundary_tag(left_row, left_column)
                     ^ boundary_tag(right_row, right_column));
        }
    }

    std::vector<int> roots;
    for (int v = 0; v < vertices; ++v) {
        const auto [row, column] = position[v];
        if (color[padded_id(row + 1, column + 1)] == 1) roots.push_back(v);
    }
    std::vector<char> active(vertices, true);
    const int infinity = std::numeric_limits<int>::max() / 4;
    int best = infinity;
    mint ways = 0;
    std::vector<int> distance(2 * vertices);
    std::vector<mint> path_count(2 * vertices);
    for (const int root : roots) {
        std::fill(distance.begin(), distance.end(), infinity);
        std::fill(path_count.begin(), path_count.end(), mint{0});
        std::queue<int> queue;
        distance[2 * root] = 0;
        path_count[2 * root] = 1;
        queue.push(2 * root);
        while (!queue.empty()) {
            const int state = queue.front();
            queue.pop();
            const int current = state / 2;
            const int parity = state % 2;
            for (const auto [next, edge_parity] : graph[current]) {
                if (!active[next]) continue;
                const int next_state = 2 * next + (parity ^ edge_parity);
                if (distance[next_state] == infinity) {
                    distance[next_state] = distance[state] + 1;
                    path_count[next_state] = path_count[state];
                    queue.push(next_state);
                } else if (distance[next_state] == distance[state] + 1) {
                    path_count[next_state] += path_count[state];
                }
            }
        }
        const int candidate = distance[2 * root + 1];
        if (candidate < best) {
            best = candidate;
            ways = path_count[2 * root + 1];
        } else if (candidate == best) {
            ways += path_count[2 * root + 1];
        }
        active[root] = false;
    }
    if (best == infinity) return {};
    return {best, ways / mint{2}};
}

}
