#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"

namespace poe {

/// 始点追加だけを行う到達可能集合: monotonereachability reached(graph); reached.add_source(source);
struct monotonereachability {
    /// O(n+m)。以後変更しない有向graphに対する空の到達可能集合を作る。
    template <directed_graph_type Graph>
    explicit monotonereachability(const Graph& graph)
        : adjacency_(graph.size()), reached_(graph.size()) {
        for (int vertex = 0; vertex < graph.size(); ++vertex) {
            adjacency_[vertex].reserve(graph[vertex].size());
            for (const auto& edge : graph[vertex]) adjacency_[vertex].push_back(edge.to);
        }
    }

    /// 償却O(新しく到達する頂点とその出辺数)。sourceから到達できる頂点を集合へ加える。
    void add_source(int source) {
        assert(0 <= source && source < size());
        if (reached_[source]) return;
        std::vector<int> stack{source};
        reached_[source] = true;
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            ++reached_count_;
            for (int next : adjacency_[vertex]) {
                if (reached_[next]) continue;
                reached_[next] = true;
                stack.push_back(next);
            }
        }
    }

    /// O(1)。vertexが追加済み始点のいずれかから到達可能か返す。
    bool contains(int vertex) const {
        assert(0 <= vertex && vertex < size());
        return reached_[vertex];
    }

    /// O(1)。到達可能な頂点数を返す。
    int count() const noexcept {
        return reached_count_;
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return static_cast<int>(adjacency_.size());
    }

private:
    std::vector<std::vector<int>> adjacency_;
    std::vector<bool> reached_;
    int reached_count_ = 0;
};

}
