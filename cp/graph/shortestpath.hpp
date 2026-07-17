#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

namespace detail {

template<class Distance>
Distance distance_infinity() {
    if constexpr (std::numeric_limits<Distance>::has_infinity) {
        return std::numeric_limits<Distance>::infinity();
    } else {
        return std::numeric_limits<Distance>::max() / 4;
    }
}

template<class Distance>
Distance add_distance(Distance left, Distance right, Distance infinity) {
    if constexpr (std::is_integral_v<Distance> && sizeof(Distance) <= 8) {
        if constexpr (std::is_signed_v<Distance>) {
            const __int128 value = static_cast<__int128>(left) + static_cast<__int128>(right);
            return static_cast<Distance>(std::clamp(value, -static_cast<__int128>(infinity),
                                                    static_cast<__int128>(infinity)));
        } else {
            const unsigned __int128 value = static_cast<unsigned __int128>(left) +
                                            static_cast<unsigned __int128>(right);
            return static_cast<Distance>(std::min(value,
                                                  static_cast<unsigned __int128>(infinity)));
        }
    } else {
        const Distance value = left + right;
        return std::clamp(value, -infinity, infinity);
    }
}

template<class Result>
void initialize_sources(Result& result, const std::vector<int>& starts) {
    const int n = static_cast<int>(result.distance.size());
    for (int start : starts) {
        assert(0 <= start && start < n);
        result.distance[start] = 0;
    }
}

}

/// 最短路の距離と復元情報: result[v], result.path(v), result.edge_path(v)。
template<class Distance>
struct shortestpathresult {
    std::vector<Distance> distance;
    std::vector<int> parent;
    std::vector<int> parent_edge;
    Distance inf = detail::distance_infinity<Distance>();

    /// O(1)。頂点vertexへ到達可能か返す。
    bool reachable(int vertex) const {
        assert_vertex(vertex);
        return distance[vertex] != inf;
    }

    /// O(1)。頂点vertexまでの距離を返す。到達不能ならinfを返す。
    Distance operator[](int vertex) const {
        assert_vertex(vertex);
        return distance[vertex];
    }

    /// O(経路長)。始点からvertexまでの頂点列を返す。到達不能なら空列を返す。
    std::vector<int> path(int vertex) const {
        assert_vertex(vertex);
        if (!reachable(vertex)) return {};
        std::vector<int> result;
        for (int current = vertex; current != -1; current = parent[current]) {
            result.push_back(current);
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    /// O(経路長)。始点からvertexまでの辺番号列を返す。到達不能なら空列を返す。
    std::vector<int> edge_path(int vertex) const {
        assert_vertex(vertex);
        if (!reachable(vertex)) return {};
        std::vector<int> result;
        for (int current = vertex; parent_edge[current] != -1; current = parent[current]) {
            result.push_back(parent_edge[current]);
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

private:
    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < static_cast<int>(distance.size()));
    }
};

/// ある頂点に近い始点の番号と距離。
template <class Distance>
struct nearestsource {
    int source = -1;
    Distance distance = detail::distance_infinity<Distance>();
};

/// 各頂点に近い異なる始点を近い順にCount個まで持つ。
template <class Distance, int Count>
struct nearestsourcesresult {
    static_assert(Count > 0);
    std::vector<std::array<nearestsource<Distance>, Count>> nearest;

    /// O(1)。vertexにindex番目の始点が存在するか返す。
    bool reachable(int vertex, int index) const {
        assert_vertex(vertex);
        assert(0 <= index && index < Count);
        return nearest[vertex][index].source != -1;
    }

    /// O(1)。vertexに近い始点列を返す。
    const std::array<nearestsource<Distance>, Count>& operator[](int vertex) const {
        assert_vertex(vertex);
        return nearest[vertex];
    }

private:
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < static_cast<int>(nearest.size()));
    }
};

/// O(n+m)。辺数を距離として、多点始点BFSを行う。重み付きグラフでは重みを無視する。
template<graph_type Graph>
shortestpathresult<int> bfs(const Graph& graph, const std::vector<int>& starts) {
    shortestpathresult<int> result;
    result.inf = std::numeric_limits<int>::max();
    result.distance.assign(graph.size(), result.inf);
    result.parent.assign(graph.size(), -1);
    result.parent_edge.assign(graph.size(), -1);
    detail::initialize_sources(result, starts);
    std::queue<int> queue;
    for (int start : starts) queue.push(start);
    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const auto& current : graph[vertex]) {
            if (result.reachable(current.to)) continue;
            result.distance[current.to] = result.distance[vertex] + 1;
            result.parent[current.to] = vertex;
            result.parent_edge[current.to] = current.id;
            queue.push(current.to);
        }
    }
    return result;
}

/// O(n+m)。辺数を距離として、startからBFSを行う。重み付きグラフでは重みを無視する。
template<graph_type Graph>
shortestpathresult<int> bfs(const Graph& graph, int start) {
    return bfs(graph, std::vector<int>{start});
}

/// O(Count*(n+m))。各頂点について異なる始点の近い方からCount個をBFSで求める。
template <int Count, graph_type Graph>
nearestsourcesresult<int, Count> nearest_sources_bfs(
    const Graph& graph,
    const std::vector<int>& starts
) {
    static_assert(Count > 0);
    nearestsourcesresult<int, Count> result;
    result.nearest.resize(graph.size());
    struct state {
        int vertex;
        int source;
        int distance;
    };
    std::queue<state> queue;
    auto insert = [&](int vertex, int source, int distance) {
        auto& nearest = result.nearest[vertex];
        for (const auto& current : nearest) {
            if (current.source == source) return false;
        }
        for (auto& current : nearest) {
            if (current.source != -1) continue;
            current = {source, distance};
            return true;
        }
        return false;
    };
    for (const int start : starts) {
        assert(0 <= start && start < graph.size());
        if (insert(start, start, 0)) queue.push({start, start, 0});
    }
    while (!queue.empty()) {
        const auto [vertex, source, distance] = queue.front();
        queue.pop();
        for (const auto& current : graph[vertex]) {
            if (!insert(current.to, source, distance + 1)) continue;
            queue.push({current.to, source, distance + 1});
        }
    }
    return result;
}

/// O(n+m)。辺重みが0または1のグラフで、多点始点最短路を求める。
template<weighted_graph_type Graph>
auto zero_one_bfs(const Graph& graph, const std::vector<int>& starts) {
    using distance_type = typename Graph::cost_type;
    shortestpathresult<distance_type> result;
    result.distance.assign(graph.size(), result.inf);
    result.parent.assign(graph.size(), -1);
    result.parent_edge.assign(graph.size(), -1);
    detail::initialize_sources(result, starts);
    std::deque<int> queue;
    for (int start : starts) queue.push_back(start);
    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop_front();
        for (const auto& current : graph[vertex]) {
            assert(current.cost == distance_type{} || current.cost == distance_type{1});
            const distance_type candidate = result.distance[vertex] + current.cost;
            if (candidate >= result.distance[current.to]) continue;
            result.distance[current.to] = candidate;
            result.parent[current.to] = vertex;
            result.parent_edge[current.to] = current.id;
            if (current.cost == distance_type{}) {
                queue.push_front(current.to);
            } else {
                queue.push_back(current.to);
            }
        }
    }
    return result;
}

/// O(n+m)。辺重みが0または1のグラフで、startから最短路を求める。
template<weighted_graph_type Graph>
auto zero_one_bfs(const Graph& graph, int start) {
    return zero_one_bfs(graph, std::vector<int>{start});
}

/// O(n+m)。expand(v,relax)がrelax(to,cost)で重み0/1の出辺を列挙する暗黙graphの最短路。
template <class Expand>
shortestpathresult<int> zero_one_bfs(
    int vertex_count,
    const std::vector<int>& starts,
    Expand expand
) {
    assert(vertex_count >= 0);
    shortestpathresult<int> result;
    result.distance.assign(vertex_count, result.inf);
    result.parent.assign(vertex_count, -1);
    result.parent_edge.assign(vertex_count, -1);
    detail::initialize_sources(result, starts);
    std::deque<std::pair<int, int>> queue;
    for (const int start : starts) queue.emplace_back(0, start);
    while (!queue.empty()) {
        const auto [distance, vertex] = queue.front();
        queue.pop_front();
        if (distance != result.distance[vertex]) continue;
        expand(vertex, [&](int to, int cost) {
            assert(0 <= to && to < vertex_count);
            assert(cost == 0 || cost == 1);
            const int candidate = distance + cost;
            if (candidate >= result.distance[to]) return;
            result.distance[to] = candidate;
            result.parent[to] = vertex;
            if (cost == 0) queue.emplace_front(candidate, to);
            else queue.emplace_back(candidate, to);
        });
    }
    return result;
}

/// O(n+m)。暗黙graphでstartから0-1 BFSを行う。expandの呼び方は多点始点版と同じ。
template <class Expand>
shortestpathresult<int> zero_one_bfs(int vertex_count, int start, Expand expand) {
    return zero_one_bfs(vertex_count, std::vector<int>{start}, std::move(expand));
}

/// O((n+m)log n)。非負重みグラフで、多点始点Dijkstra法を行う。
template<weighted_graph_type Graph>
auto dijkstra(const Graph& graph, const std::vector<int>& starts) {
    using distance_type = typename Graph::cost_type;
    shortestpathresult<distance_type> result;
    result.distance.assign(graph.size(), result.inf);
    result.parent.assign(graph.size(), -1);
    result.parent_edge.assign(graph.size(), -1);
    detail::initialize_sources(result, starts);
    using state = std::pair<distance_type, int>;
    std::priority_queue<state, std::vector<state>, std::greater<state>> queue;
    for (int start : starts) queue.emplace(distance_type{}, start);
    while (!queue.empty()) {
        const auto [distance, vertex] = queue.top();
        queue.pop();
        if (distance != result.distance[vertex]) continue;
        for (const auto& current : graph[vertex]) {
            assert(current.cost >= distance_type{});
            const distance_type candidate =
                detail::add_distance(distance, current.cost, result.inf);
            if (candidate >= result.distance[current.to]) continue;
            result.distance[current.to] = candidate;
            result.parent[current.to] = vertex;
            result.parent_edge[current.to] = current.id;
            queue.emplace(candidate, current.to);
        }
    }
    return result;
}

/// O((n+m)log n)。非負重みグラフで、startからDijkstra法を行う。
template<weighted_graph_type Graph>
auto dijkstra(const Graph& graph, int start) {
    return dijkstra(graph, std::vector<int>{start});
}

/// Bellman-Ford法の結果。negative[v]は負閉路からvへ到達できることを表す。
template<class Distance>
struct bellmanfordresult : shortestpathresult<Distance> {
    std::vector<char> negative;

    /// O(1)。頂点vertexの最短距離が負閉路の影響を受けるか返す。
    bool affected_by_negative_cycle(int vertex) const {
        assert(0 <= vertex && vertex < static_cast<int>(negative.size()));
        return negative[vertex];
    }

    /// O(n)。始点から到達できる負閉路が存在するか返す。
    bool has_negative_cycle() const {
        return std::ranges::find(negative, true) != negative.end();
    }

    /// O(経路長)。有限な最短路の頂点列を返す。到達不能または負閉路の影響下なら空列を返す。
    std::vector<int> path(int vertex) const {
        if (affected_by_negative_cycle(vertex)) return {};
        return shortestpathresult<Distance>::path(vertex);
    }

    /// O(経路長)。有限な最短路の辺番号列を返す。到達不能または負閉路の影響下なら空列を返す。
    std::vector<int> edge_path(int vertex) const {
        if (affected_by_negative_cycle(vertex)) return {};
        return shortestpathresult<Distance>::edge_path(vertex);
    }
};

/// O(nm)。負辺を許すグラフで、多点始点Bellman-Ford法を行う。
template<weighted_graph_type Graph>
auto bellman_ford(const Graph& graph, const std::vector<int>& starts) {
    using distance_type = typename Graph::cost_type;
    bellmanfordresult<distance_type> result;
    result.distance.assign(graph.size(), result.inf);
    result.parent.assign(graph.size(), -1);
    result.parent_edge.assign(graph.size(), -1);
    result.negative.assign(graph.size(), false);
    detail::initialize_sources(result, starts);
    for (int iteration = 0; iteration + 1 < graph.size(); ++iteration) {
        bool updated = false;
        for (int vertex = 0; vertex < graph.size(); ++vertex) {
            if (!result.reachable(vertex)) continue;
            for (const auto& current : graph[vertex]) {
                const distance_type candidate = detail::add_distance(
                    result.distance[vertex], current.cost, result.inf);
                if (candidate >= result.distance[current.to]) continue;
                result.distance[current.to] = candidate;
                result.parent[current.to] = vertex;
                result.parent_edge[current.to] = current.id;
                updated = true;
            }
        }
        if (!updated) break;
    }
    std::queue<int> queue;
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        if (!result.reachable(vertex)) continue;
        for (const auto& current : graph[vertex]) {
            const distance_type candidate = detail::add_distance(
                result.distance[vertex], current.cost, result.inf);
            if (candidate < result.distance[current.to] && !result.negative[current.to]) {
                result.negative[current.to] = true;
                queue.push(current.to);
            }
        }
    }
    while (!queue.empty()) {
        const int vertex = queue.front();
        queue.pop();
        for (const auto& current : graph[vertex]) {
            if (result.negative[current.to]) continue;
            result.negative[current.to] = true;
            queue.push(current.to);
        }
    }
    return result;
}

/// O(nm)。負辺を許すグラフで、startからBellman-Ford法を行う。
template<weighted_graph_type Graph>
auto bellman_ford(const Graph& graph, int start) {
    return bellman_ford(graph, std::vector<int>{start});
}

}
