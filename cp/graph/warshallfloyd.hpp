#pragma once
#include <bits/stdc++.h>
#include "graph/shortestpath.hpp"

namespace poe {

/// 全点対最短路結果: result(from, to), result.path(from, to)で取得する。
template<class Distance>
struct warshallfloydresult {
    std::vector<std::vector<Distance>> distance;
    std::vector<std::vector<int>> next;
    Distance inf = detail::distance_infinity<Distance>();

    /// O(1)。fromからtoへ到達可能か返す。
    bool reachable(int from, int to) const {
        assert_vertex(from);
        assert_vertex(to);
        return distance[from][to] != inf;
    }

    /// O(1)。fromからtoへの距離を返す。到達不能ならinf。
    Distance operator()(int from, int to) const {
        assert_vertex(from);
        assert_vertex(to);
        return distance[from][to];
    }

    /// O(n)。負閉路が存在するならtrueを返す。
    bool has_negative_cycle() const {
        for (int vertex = 0; vertex < size(); ++vertex) {
            if (distance[vertex][vertex] < Distance{}) return true;
        }
        return false;
    }

    /// O(n)。fromからtoの最短路が負閉路の影響を受けるならtrueを返す。
    bool affected_by_negative_cycle(int from, int to) const {
        assert_vertex(from);
        assert_vertex(to);
        for (int middle = 0; middle < size(); ++middle) {
            if (reachable(from, middle) && distance[middle][middle] < Distance{} &&
                reachable(middle, to)) {
                return true;
            }
        }
        return false;
    }

    /// O(経路長+n)。有限な最短路の頂点列を返す。到達不能または負閉路の影響下なら空列。
    std::vector<int> path(int from, int to) const {
        assert_vertex(from);
        assert_vertex(to);
        if (!reachable(from, to) || affected_by_negative_cycle(from, to)) return {};
        std::vector<int> result = {from};
        while (from != to) {
            from = next[from][to];
            assert(from != -1);
            result.push_back(from);
        }
        return result;
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(distance.size());
    }

private:
    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size());
    }
};

/// O(n^3)、空間O(n^2)。重み付きグラフの全点対最短路をWarshall-Floyd法で求める。
template<weighted_graph_type Graph>
auto warshall_floyd(const Graph& graph) {
    using distance_type = typename Graph::cost_type;
    warshallfloydresult<distance_type> result;
    result.distance.assign(graph.size(), std::vector<distance_type>(graph.size(), result.inf));
    result.next.assign(graph.size(), std::vector<int>(graph.size(), -1));
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
        result.distance[vertex][vertex] = distance_type{};
        result.next[vertex][vertex] = vertex;
    }
    for (int from = 0; from < graph.size(); ++from) {
        for (const auto& current : graph[from]) {
            if (current.cost < result.distance[from][current.to]) {
                result.distance[from][current.to] = current.cost;
                result.next[from][current.to] = current.to;
            }
        }
    }
    for (int middle = 0; middle < graph.size(); ++middle) {
        for (int from = 0; from < graph.size(); ++from) {
            if (!result.reachable(from, middle)) continue;
            for (int to = 0; to < graph.size(); ++to) {
                if (!result.reachable(middle, to)) continue;
                const distance_type candidate = detail::add_distance(
                    result.distance[from][middle], result.distance[middle][to], result.inf);
                if (candidate >= result.distance[from][to]) continue;
                result.distance[from][to] = candidate;
                result.next[from][to] = result.next[from][middle];
            }
        }
    }
    return result;
}

}
