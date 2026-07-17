#pragma once
#include <bits/stdc++.h>
#include "tree/centroidtree.hpp"

namespace poe {

/// 最寄りの有効頂点query結果。距離が同じなら頂点番号が小さいものを返す。
template <class Distance>
struct nearestactiveresult {
    int vertex;
    Distance distance;
};

/// 有効頂点を更新し最寄りを尋ねる: nearestactive active(graph); 更新O(log^2 n)、query O(log n)。
template <undirected_graph_type Graph>
struct nearestactive {
    using distance_type = detail::tree_distance_type_t<Graph>;

    /// O(n log n)。全頂点が無効な状態で、非負辺重みの木を前処理する。
    explicit nearestactive(const Graph& graph)
        : decomposition_(graph), active_(graph.size()), nearest_(graph.size()) {}

    /// O(log^2 n)。vertexを有効化する。状態が変わったならtrue。
    bool add(int vertex) {
        assert_vertex(vertex);
        if (active_[vertex]) return false;
        active_[vertex] = true;
        for (const auto& entry : decomposition_.path(vertex)) {
            nearest_[entry.centroid].emplace(entry.distance, vertex);
        }
        return true;
    }

    /// O(log^2 n)。vertexを無効化する。状態が変わったならtrue。
    bool remove(int vertex) {
        assert_vertex(vertex);
        if (!active_[vertex]) return false;
        active_[vertex] = false;
        for (const auto& entry : decomposition_.path(vertex)) {
            auto& values = nearest_[entry.centroid];
            const auto iterator = values.find({entry.distance, vertex});
            assert(iterator != values.end());
            values.erase(iterator);
        }
        return true;
    }

    /// O(log^2 n)。vertexの有効・無効を反転し、変更後の状態を返す。
    bool toggle(int vertex) {
        assert_vertex(vertex);
        if (active_[vertex]) {
            remove(vertex);
            return false;
        }
        add(vertex);
        return true;
    }

    /// O(log^2 n)。vertexをenabledで指定した状態にし、状態が変わったならtrue。
    bool set(int vertex, bool enabled) {
        return enabled ? add(vertex) : remove(vertex);
    }

    /// O(log n)。vertexから最寄りの有効頂点と距離。存在しなければnullopt。
    std::optional<nearestactiveresult<distance_type>> nearest(int vertex) const {
        assert_vertex(vertex);
        std::optional<nearestactiveresult<distance_type>> result;
        for (const auto& entry : decomposition_.path(vertex)) {
            const auto& values = nearest_[entry.centroid];
            if (values.empty()) continue;
            const auto& [stored_distance, candidate_vertex] = *values.begin();
            const nearestactiveresult<distance_type> candidate = {
                candidate_vertex, entry.distance + stored_distance};
            if (!result || candidate.distance < result->distance ||
                (candidate.distance == result->distance &&
                 candidate.vertex < result->vertex)) {
                result = candidate;
            }
        }
        return result;
    }

    /// O(log n)。vertexから最寄りの有効頂点までの距離。存在しなければnullopt。
    std::optional<distance_type> distance(int vertex) const {
        const auto result = nearest(vertex);
        return result ? std::optional<distance_type>(result->distance) : std::nullopt;
    }

    /// O(1)。vertexが有効ならtrue。
    bool operator[](int vertex) const {
        assert_vertex(vertex);
        return active_[vertex];
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept { return static_cast<int>(active_.size()); }

private:
    void assert_vertex(int vertex) const { assert(0 <= vertex && vertex < size()); }

    centroidtree<Graph> decomposition_;
    std::vector<char> active_;
    std::vector<std::multiset<std::pair<distance_type, int>>> nearest_;
};

}
