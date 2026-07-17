#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// 有限手数の真偽ゲーム結果。winning[v]は開始頂点vで真側が勝つことを表す。
struct finitegameresult {
    std::vector<char> winning;

    /// O(1)。vertexから開始して真側が勝つならtrueを返す。
    bool operator[](int vertex) const {
        assert(0 <= vertex && vertex < static_cast<int>(winning.size()));
        return winning[vertex];
    }
};

/// O(turns*(n+m))。turn_maximizes(turn)で各手番のOR・ANDを指定する有限手数ゲームを解く。
template <graph_type Graph, class TurnMaximizes>
requires std::invocable<TurnMaximizes, int>
finitegameresult finite_horizon_game(
    const Graph& graph,
    const std::vector<bool>& terminal_winning,
    int turns,
    TurnMaximizes turn_maximizes
) {
    assert(static_cast<int>(terminal_winning.size()) == graph.size());
    assert(turns >= 0);
    std::vector<char> next(graph.size());
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        next[vertex] = terminal_winning[vertex];
    }
    std::vector<char> current(graph.size());
    for (int turn = turns - 1; turn >= 0; --turn) {
        const bool maximize = static_cast<bool>(std::invoke(turn_maximizes, turn));
        for (int vertex = 0; vertex < graph.size(); ++vertex) {
            bool outcome = !maximize;
            for (const auto& edge : graph[vertex]) {
                if (maximize) outcome = outcome || next[edge.to];
                else outcome = outcome && next[edge.to];
            }
            current[vertex] = outcome;
        }
        std::swap(current, next);
    }
    return {std::move(next)};
}

/// O(turns*(n+m))。first_maximizesから真最大化・最小化を交互に行うゲームを解く。
template <graph_type Graph>
finitegameresult finite_horizon_game(
    const Graph& graph,
    const std::vector<bool>& terminal_winning,
    int turns,
    bool first_maximizes = true
) {
    return finite_horizon_game(
        graph, terminal_winning, turns,
        [first_maximizes](int turn) {
            return first_maximizes ^ static_cast<bool>(turn & 1);
        }
    );
}

}
