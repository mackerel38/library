#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// alternating_path_gameの手番。minimizeは有限終了を優先して費用を最小化する。
enum class gameplayer {
    minimize,
    maximize,
};

/// alternating_path_gameの結果。値がnulloptの局面は最適プレイで終了しない。
template <class Cost>
struct alternatingpathgameresult {
    std::vector<std::array<std::optional<Cost>, 2>> values;

    /// O(1)。vertexでplayerの手番から始めたゲームが有限回で終了するか返す。
    bool finite(int vertex, gameplayer player) const {
        return value(vertex, player).has_value();
    }

    /// O(1)。有限なら最適プレイ時の総費用、終了しないならnulloptを返す。
    const std::optional<Cost>& value(int vertex, gameplayer player) const {
        assert(0 <= vertex && vertex < static_cast<int>(values.size()));
        return values[vertex][static_cast<int>(player)];
    }
};

/// O((n+m) log n)時間・O(n+m)領域。非負重み付き有向graphの交互min/max終了優先ゲームを解く。
template <directed_graph_type Graph>
requires weighted_graph_type<Graph>
      && std::totally_ordered<typename Graph::cost_type>
alternatingpathgameresult<typename Graph::cost_type> alternating_path_game(
    const Graph& graph
) {
    using cost_type = typename Graph::cost_type;
    struct reverseedge {
        int from;
        cost_type cost;
    };
    const int size = graph.size();
    std::vector<std::vector<reverseedge>> reverse(size);
    std::vector<int> outdegree(size);
    for (int from = 0; from < size; ++from) {
        outdegree[from] = static_cast<int>(graph[from].size());
        for (const auto& edge : graph[from]) {
            assert(!(edge.cost < cost_type{}));
            reverse[edge.to].push_back({from, edge.cost});
        }
    }

    const int states = 2 * size;
    const auto state = [](int vertex, gameplayer player) {
        return 2 * vertex + static_cast<int>(player);
    };
    using queueentry = std::pair<cost_type, int>;
    std::priority_queue<queueentry, std::vector<queueentry>, std::greater<>> queue;
    std::vector<char> settled(states);
    std::vector<int> finite_successors(size);
    std::vector<cost_type> maximum_candidate(size);
    for (int vertex = 0; vertex < size; ++vertex) {
        if (outdegree[vertex] == 0) {
            queue.emplace(cost_type{}, state(vertex, gameplayer::minimize));
            queue.emplace(cost_type{}, state(vertex, gameplayer::maximize));
        }
    }

    alternatingpathgameresult<cost_type> result;
    result.values.resize(size);
    while (!queue.empty()) {
        const auto [value, current] = queue.top();
        queue.pop();
        if (settled[current]) continue;
        settled[current] = true;
        const int vertex = current / 2;
        const auto player = static_cast<gameplayer>(current % 2);
        result.values[vertex][static_cast<int>(player)] = value;

        const auto predecessor_player = player == gameplayer::minimize
                                      ? gameplayer::maximize
                                      : gameplayer::minimize;
        for (const auto& edge : reverse[vertex]) {
            const cost_type candidate = edge.cost + value;
            const int predecessor = state(edge.from, predecessor_player);
            if (settled[predecessor]) continue;
            if (predecessor_player == gameplayer::minimize) {
                queue.emplace(candidate, predecessor);
            } else {
                if (finite_successors[edge.from]++ == 0
                    || maximum_candidate[edge.from] < candidate) {
                    maximum_candidate[edge.from] = candidate;
                }
                if (finite_successors[edge.from] == outdegree[edge.from]) {
                    queue.emplace(maximum_candidate[edge.from], predecessor);
                }
            }
        }
    }
    return result;
}

}
