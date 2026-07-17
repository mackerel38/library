#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/grid.hpp"

namespace poe {

/// 同期単調activationの結果。roundは初期頂点が0、未到達が-1。
struct activationresult {
    std::vector<int> round;
    int active_count = 0;

    /// O(1)。vertexが最終的に有効ならtrueを返す。
    bool contains(int vertex) const {
        assert(0 <= vertex && vertex < size());
        return round[vertex] >= 0;
    }

    /// O(1)。vertexが有効になったラウンドを返す。未到達は-1。
    int operator[](int vertex) const {
        assert(0 <= vertex && vertex < size());
        return round[vertex];
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(round.size());
    }
};

/// grid同期単調activationの結果。round[row][column]は初期マスが0、未到達が-1。
struct gridactivationresult {
    std::vector<std::vector<int>> round;
    int active_count = 0;

    /// O(1)。(row,column)が最終的に有効ならtrueを返す。
    bool contains(int row, int column) const {
        assert(0 <= row && row < height() && 0 <= column && column < width());
        return round[row][column] >= 0;
    }

    /// O(1)。行数を返す。
    int height() const noexcept {
        return static_cast<int>(round.size());
    }

    /// O(1)。列数を返す。
    int width() const noexcept {
        return round.empty() ? 0 : static_cast<int>(round.front().size());
    }
};

namespace detail {

template <class ForEachNeighbor, class CanActivate>
activationresult synchronous_activation_core(
    int size,
    const std::vector<bool>& initially_active,
    ForEachNeighbor for_each_neighbor,
    CanActivate can_activate
) {
    assert(size >= 0 && static_cast<int>(initially_active.size()) == size);
    activationresult result{std::vector<int>(size, -1), 0};
    std::vector<int> active_neighbor_count(size);
    for (int vertex = 0; vertex < size; ++vertex) {
        if (!initially_active[vertex]) continue;
        result.round[vertex] = 0;
        ++result.active_count;
        for_each_neighbor(vertex, [&](int next) {
            assert(0 <= next && next < size);
            ++active_neighbor_count[next];
        });
    }

    std::vector<int> candidates;
    candidates.reserve(size);
    for (int vertex = 0; vertex < size; ++vertex) {
        if (result.round[vertex] == -1) candidates.push_back(vertex);
    }
    std::vector<int> stamp(size, -1);
    int current_round = 1;
    while (!candidates.empty()) {
        std::vector<int> activated;
        for (const int vertex : candidates) {
            if (result.round[vertex] == -1
                && can_activate(vertex, active_neighbor_count[vertex])) {
                activated.push_back(vertex);
            }
        }
        if (activated.empty()) break;
        for (const int vertex : activated) {
            result.round[vertex] = current_round;
            ++result.active_count;
        }

        std::vector<int> next_candidates;
        for (const int vertex : activated) {
            for_each_neighbor(vertex, [&](int next) {
                ++active_neighbor_count[next];
                if (result.round[next] == -1 && stamp[next] != current_round) {
                    stamp[next] = current_round;
                    next_candidates.push_back(next);
                }
            });
        }
        candidates.swap(next_candidates);
        ++current_round;
    }
    return result;
}

}

/// O(n+m)。各round開始時にcan_activate(vertex,active近傍数)を満たす頂点を一斉に有効化する。
template <graph_type Graph, class CanActivate>
activationresult synchronous_activation(
    const Graph& graph,
    const std::vector<bool>& initially_active,
    CanActivate can_activate
) {
    return detail::synchronous_activation_core(
        graph.size(), initially_active,
        [&](int vertex, auto visit) {
            for (const auto& edge : graph[vertex]) visit(edge.to);
        },
        std::move(can_activate));
}

/// O(hwK)。gridで各round開始時に条件を満たすマスをdirections近傍により一斉有効化する。
template <class InitiallyActive, class CanActivate, std::size_t K>
gridactivationresult grid_synchronous_activation(
    int height,
    int width,
    InitiallyActive initially_active,
    CanActivate can_activate,
    const std::array<gridpoint, K>& directions
) {
    assert(height >= 0 && width >= 0);
    const int size = height * width;
    std::vector<bool> initial(size);
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            initial[row * width + column] = initially_active(row, column);
        }
    }
    auto flat = detail::synchronous_activation_core(
        size, initial,
        [&](int vertex, auto visit) {
            const int row = vertex / width;
            const int column = vertex % width;
            for (const auto direction : directions) {
                const int next_row = row + direction.row;
                const int next_column = column + direction.column;
                if (inside_grid(next_row, next_column, height, width)) {
                    visit(next_row * width + next_column);
                }
            }
        },
        [&](int vertex, int active_neighbor_count) {
            return can_activate(vertex / width, vertex % width, active_neighbor_count);
        });

    gridactivationresult result{
        std::vector(height, std::vector<int>(width, -1)), flat.active_count};
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            result.round[row][column] = flat.round[row * width + column];
        }
    }
    return result;
}

/// O(hw)。上下左右4近傍で同期単調activationを行う。
template <class InitiallyActive, class CanActivate>
gridactivationresult grid_synchronous_activation(
    int height,
    int width,
    InitiallyActive initially_active,
    CanActivate can_activate
) {
    return grid_synchronous_activation(
        height, width, std::move(initially_active),
        std::move(can_activate), grid4);
}

}
