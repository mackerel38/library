#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/lowlink.hpp"
#include "structure/dsu.hpp"

namespace poe {

/// O(n^3/64+nq+n log n)。各queryのpath上最大頂点costの最小値を返す。到達不能ならnullopt。
template<graph_type Graph, class Cost>
std::vector<std::optional<Cost>> minimum_vertex_bottleneck_paths(
    const Graph& graph,
    const std::vector<Cost>& vertex_cost,
    const std::vector<std::pair<int, int>>& queries
) {
    const int size = graph.size();
    assert(static_cast<int>(vertex_cost.size()) == size);
    const int words = (size + 63) / 64;
    std::vector reachable(size, std::vector<std::uint64_t>(words));
    const auto set_reachable = [&](int from, int to) {
        reachable[from][to / 64] |= 1ULL << (to % 64);
    };
    const auto is_reachable = [&](int from, int to) {
        return (reachable[from][to / 64] >> (to % 64) & 1ULL) != 0;
    };
    for (int vertex = 0; vertex < size; ++vertex) {
        set_reachable(vertex, vertex);
        for (const auto& edge : graph[vertex]) set_reachable(vertex, edge.to);
    }
    for (const auto& [source, target] : queries) {
        assert(0 <= source && source < size && 0 <= target && target < size);
    }

    std::vector<int> order(size);
    std::iota(order.begin(), order.end(), 0);
    std::ranges::sort(order, [&](int left, int right) {
        if (vertex_cost[left] != vertex_cost[right]) {
            return vertex_cost[left] < vertex_cost[right];
        }
        return left < right;
    });
    std::vector<std::optional<Cost>> answer(queries.size());
    for (int left = 0; left < size;) {
        int right = left + 1;
        while (right < size && vertex_cost[order[right]] == vertex_cost[order[left]]) ++right;
        for (int index = left; index < right; ++index) {
            const int middle = order[index];
            for (int source = 0; source < size; ++source) {
                if (!is_reachable(source, middle)) continue;
                for (int word = 0; word < words; ++word) {
                    reachable[source][word] |= reachable[middle][word];
                }
            }
        }
        for (int query = 0; query < static_cast<int>(queries.size()); ++query) {
            if (answer[query].has_value()) continue;
            const auto [source, target] = queries[query];
            if (!is_reachable(source, target)) continue;
            answer[query] = std::max({vertex_cost[order[left]],
                                      vertex_cost[source], vertex_cost[target]});
        }
        left = right;
    }
    return answer;
}

/// minimax距離に指定辺が不可欠かを判定する: bottlenecksensitivity sensitivity(graph)。
template<weighted_graph_type Graph>
    requires (!Graph::is_directed)
struct bottlenecksensitivity {
    using cost_type = typename Graph::cost_type;

    /// O(m log m+n log n)。連結な重み付き無向グラフを前処理する。
    explicit bottlenecksensitivity(const Graph& graph)
        : graph_(&graph), critical_(graph.edge_count()), edge_child_(graph.edge_count(), -1),
          tree_(graph.size()) {
        assert(graph.size() > 0);
        std::vector<int> ids(graph.edge_count());
        std::iota(ids.begin(), ids.end(), 0);
        std::ranges::sort(ids, [&](int left, int right) {
            const auto& first = graph.edge_at(left);
            const auto& second = graph.edge_at(right);
            if (first.cost != second.cost) return first.cost < second.cost;
            return left < right;
        });

        dsu components(graph.size());
        std::vector<int> tree_original;
        for (int left = 0; left < graph.edge_count();) {
            int right = left + 1;
            while (right < graph.edge_count()
                   && graph.edge_at(ids[right]).cost == graph.edge_at(ids[left]).cost) {
                ++right;
            }
            std::vector<int> roots;
            for (int index = left; index < right; ++index) {
                const auto& edge = graph.edge_at(ids[index]);
                const int first = components.leader(edge.from);
                const int second = components.leader(edge.to);
                if (first == second) continue;
                roots.push_back(first);
                roots.push_back(second);
            }
            std::ranges::sort(roots);
            roots.erase(std::unique(roots.begin(), roots.end()), roots.end());
            undirected_graph temporary(static_cast<int>(roots.size()));
            std::vector<int> original;
            for (int index = left; index < right; ++index) {
                const int id = ids[index];
                const auto& edge = graph.edge_at(id);
                const int first = components.leader(edge.from);
                const int second = components.leader(edge.to);
                if (first == second) continue;
                const int a = std::ranges::lower_bound(roots, first) - roots.begin();
                const int b = std::ranges::lower_bound(roots, second) - roots.begin();
                temporary.add_edge(a, b);
                original.push_back(id);
            }
            const auto links = lowlink(temporary);
            for (int id = 0; id < temporary.edge_count(); ++id) {
                if (links.is_bridge(id)) critical_[original[id]] = true;
            }
            for (int index = left; index < right; ++index) {
                const int id = ids[index];
                const auto& edge = graph.edge_at(id);
                if (components.same(edge.from, edge.to)) continue;
                components.merge(edge.from, edge.to);
                tree_.add_edge(edge.from, edge.to, edge.cost);
                tree_original.push_back(id);
            }
            left = right;
        }
        assert(components.count() == 1);
        build_tree_data(tree_original);
    }

    /// O(log n)。辺edge_idの重み増加でs-t minimax距離が増えるならtrueを返す。
    bool increases(int edge_id, int s, int t) const {
        assert(0 <= edge_id && edge_id < graph_->edge_count());
        assert(0 <= s && s < graph_->size());
        assert(0 <= t && t < graph_->size());
        assert(s != t);
        if (!critical_[edge_id]) return false;
        const int child = edge_child_[edge_id];
        assert(child != -1);
        if (in_subtree(child, s) == in_subtree(child, t)) return false;
        return maximum_on_path(s, t) == graph_->edge_at(edge_id).cost;
    }

    /// O(1)。辺edge_idがその重み以下の部分グラフで不可欠な橋ならtrueを返す。
    bool critical(int edge_id) const {
        assert(0 <= edge_id && edge_id < graph_->edge_count());
        return critical_[edge_id];
    }

private:
    /// O(n log n)。最小全域木の祖先・部分木区間・path最大値表を作る。
    void build_tree_data(const std::vector<int>& tree_original) {
        const int n = tree_.size();
        assert(tree_.edge_count() == n - 1);
        parent_.assign(n, -1);
        depth_.assign(n, 0);
        in_.assign(n, 0);
        subtree_size_.assign(n, 1);
        std::vector<int> parent_edge(n, -1);
        std::vector<int> order;
        order.reserve(n);
        std::vector<int> stack{0};
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            in_[vertex] = order.size();
            order.push_back(vertex);
            for (auto iterator = tree_[vertex].rbegin(); iterator != tree_[vertex].rend(); ++iterator) {
                if (iterator->id == parent_edge[vertex]) continue;
                parent_[iterator->to] = vertex;
                parent_edge[iterator->to] = iterator->id;
                depth_[iterator->to] = depth_[vertex] + 1;
                stack.push_back(iterator->to);
            }
        }
        assert(static_cast<int>(order.size()) == n);
        for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
            const int vertex = *iterator;
            if (parent_[vertex] != -1) subtree_size_[parent_[vertex]] += subtree_size_[vertex];
        }
        const int levels = std::max(1, std::bit_width(static_cast<unsigned>(n)));
        ancestor_.assign(levels, std::vector<int>(n));
        maximum_.assign(levels, std::vector<cost_type>(n));
        for (int vertex = 0; vertex < n; ++vertex) {
            ancestor_[0][vertex] = parent_[vertex] == -1 ? vertex : parent_[vertex];
            if (parent_edge[vertex] != -1) {
                maximum_[0][vertex] = tree_.edge_at(parent_edge[vertex]).cost;
                edge_child_[tree_original[parent_edge[vertex]]] = vertex;
            }
        }
        for (int level = 1; level < levels; ++level) {
            for (int vertex = 0; vertex < n; ++vertex) {
                const int middle = ancestor_[level - 1][vertex];
                ancestor_[level][vertex] = ancestor_[level - 1][middle];
                maximum_[level][vertex] = std::max(
                    maximum_[level - 1][vertex],
                    maximum_[level - 1][middle]
                );
            }
        }
    }

    /// O(1)。vertexがrootの部分木内ならtrueを返す。
    bool in_subtree(int root, int vertex) const {
        return in_[root] <= in_[vertex] && in_[vertex] < in_[root] + subtree_size_[root];
    }

    /// O(log n)。最小全域木path上の最大辺重みを返す。
    cost_type maximum_on_path(int first, int second) const {
        bool has_value = false;
        cost_type answer{};
        const auto take = [&](const cost_type& value) {
            if (!has_value || answer < value) answer = value;
            has_value = true;
        };
        if (depth_[first] < depth_[second]) std::swap(first, second);
        int difference = depth_[first] - depth_[second];
        for (int level = 0; difference > 0; ++level, difference >>= 1) {
            if (!(difference & 1)) continue;
            take(maximum_[level][first]);
            first = ancestor_[level][first];
        }
        if (first == second) {
            assert(has_value);
            return answer;
        }
        for (int level = static_cast<int>(ancestor_.size()) - 1; level >= 0; --level) {
            if (ancestor_[level][first] == ancestor_[level][second]) continue;
            take(maximum_[level][first]);
            take(maximum_[level][second]);
            first = ancestor_[level][first];
            second = ancestor_[level][second];
        }
        take(maximum_[0][first]);
        take(maximum_[0][second]);
        return answer;
    }

    const Graph* graph_;
    std::vector<char> critical_;
    std::vector<int> edge_child_;
    undirected_graph<cost_type> tree_;
    std::vector<int> parent_;
    std::vector<int> depth_;
    std::vector<int> in_;
    std::vector<int> subtree_size_;
    std::vector<std::vector<int>> ancestor_;
    std::vector<std::vector<cost_type>> maximum_;
};

}
