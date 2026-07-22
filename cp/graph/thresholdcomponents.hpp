#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "structure/dsu.hpp"

namespace poe {

namespace detail {

template<class T>
std::vector<T> truncated_product(
    const std::vector<T>& left,
    const std::vector<T>& right,
    int degree
) {
    std::vector<T> result(std::min<int>(degree + 1, left.size() + right.size() - 1));
    for (int i = 0; i < static_cast<int>(left.size()); ++i) {
        for (int j = 0; j < static_cast<int>(right.size()) && i + j <= degree; ++j) {
            result[i + j] += left[i] * right[j];
        }
    }
    return result;
}

}

/// O(m log m+nK)時間・O(nK+m)領域。重み閾値連結成分のunionを作る最少選択数別の集合数。
template<class T, weighted_graph_type Graph>
    requires (!Graph::is_directed)
std::vector<T> threshold_component_union_counts(
    const Graph& graph,
    int maximum_selections
) {
    assert(maximum_selections >= 0);
    const int n = graph.size();
    std::vector<int> edge_ids(graph.edge_count());
    std::iota(edge_ids.begin(), edge_ids.end(), 0);
    std::ranges::sort(edge_ids, [&](int left, int right) {
        return graph.edge_at(left).cost < graph.edge_at(right).cost;
    });

    dsu components(n);
    std::vector<std::vector<T>> polynomial(n, std::vector<T>{T{1}});
    if (maximum_selections >= 1) {
        for (auto& current : polynomial) current.push_back(T{1});
    }
    for (int begin = 0; begin < static_cast<int>(edge_ids.size());) {
        int end = begin + 1;
        while (end < static_cast<int>(edge_ids.size())
               && graph.edge_at(edge_ids[begin]).cost == graph.edge_at(edge_ids[end]).cost) {
            ++end;
        }
        std::vector<std::pair<int, int>> pairs;
        std::vector<int> roots;
        for (int index = begin; index < end; ++index) {
            const auto& edge = graph.edge_at(edge_ids[index]);
            const int left = components.leader(edge.from);
            const int right = components.leader(edge.to);
            if (left == right) continue;
            pairs.emplace_back(left, right);
            roots.push_back(left);
            roots.push_back(right);
        }
        std::ranges::sort(roots);
        roots.erase(std::unique(roots.begin(), roots.end()), roots.end());
        dsu temporary(roots.size());
        for (const auto& [left, right] : pairs) {
            const int left_index = std::lower_bound(roots.begin(), roots.end(), left) - roots.begin();
            const int right_index = std::lower_bound(roots.begin(), roots.end(), right) - roots.begin();
            temporary.merge(left_index, right_index);
        }
        std::vector<std::vector<int>> groups(roots.size());
        for (int index = 0; index < static_cast<int>(roots.size()); ++index) {
            groups[temporary.leader(index)].push_back(roots[index]);
        }
        for (const auto& group : groups) {
            if (group.size() < 2) continue;
            std::vector<T> merged{T{1}};
            for (const int root : group) {
                merged = detail::truncated_product(
                    merged, polynomial[root], maximum_selections);
            }
            if (static_cast<int>(group.size()) <= maximum_selections) {
                merged[group.size()] -= T{1};
            }
            if (maximum_selections >= 1) merged[1] += T{1};
            int root = group[0];
            for (int index = 1; index < static_cast<int>(group.size()); ++index) {
                root = components.merge(root, group[index]);
            }
            polynomial[root] = std::move(merged);
        }
        begin = end;
    }

    std::vector<T> result{T{1}};
    for (int vertex = 0; vertex < n; ++vertex) {
        if (components.leader(vertex) == vertex) {
            result = detail::truncated_product(
                result, polynomial[vertex], maximum_selections);
        }
    }
    result.resize(maximum_selections + 1);
    return result;
}

}
