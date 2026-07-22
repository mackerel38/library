#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O((n+q)log^2 q)。各部分列が選ぶ根path和集合の頂点重み総和を全て足す。
template <class T>
T sum_subarray_root_path_unions(
    const std::vector<int>& parent,
    const std::vector<int>& vertices,
    const std::vector<T>& weight
) {
    const int n = static_cast<int>(parent.size());
    assert(static_cast<int>(weight.size()) == n);
    if (n == 0) {
        assert(vertices.empty());
        return T{};
    }
    std::vector<std::vector<int>> children(n);
    int root = -1;
    for (int vertex = 0; vertex < n; ++vertex) {
        if (parent[vertex] == -1) {
            assert(root == -1);
            root = vertex;
        } else {
            assert(0 <= parent[vertex] && parent[vertex] < n);
            children[parent[vertex]].push_back(vertex);
        }
    }
    assert(root != -1);
    std::vector<std::vector<int>> positions(n);
    for (int position = 0; position < static_cast<int>(vertices.size()); ++position) {
        assert(0 <= vertices[position] && vertices[position] < n);
        positions[vertices[position]].push_back(position);
    }
    std::vector<int> order{root};
    for (int index = 0; index < static_cast<int>(order.size()); ++index) {
        for (int child : children[order[index]]) order.push_back(child);
    }
    const long long length = static_cast<long long>(vertices.size());
    const long long all_subarrays = length * (length + 1) / 2;
    struct position_set {
        std::set<int> values;
        long long avoiding = 0;
    };
    std::vector<position_set> subtree_positions(n);
    for (auto& positions_set : subtree_positions) {
        positions_set.avoiding = all_subarrays;
    }
    const auto triangle = [](long long value) {
        return value * (value + 1) / 2;
    };
    const auto insert = [&](position_set& positions_set, int position) {
        const auto next_iterator = positions_set.values.lower_bound(position);
        if (next_iterator != positions_set.values.end() &&
            *next_iterator == position) return;
        const int next = next_iterator == positions_set.values.end()
            ? static_cast<int>(length) : *next_iterator;
        const int previous = next_iterator == positions_set.values.begin()
            ? -1 : *std::prev(next_iterator);
        positions_set.avoiding -= triangle(next - previous - 1LL);
        positions_set.avoiding += triangle(position - previous - 1LL);
        positions_set.avoiding += triangle(next - position - 1LL);
        positions_set.values.insert(next_iterator, position);
    };
    T answer{};
    for (auto iterator = order.rbegin(); iterator != order.rend(); ++iterator) {
        const int vertex = *iterator;
        for (int child : children[vertex]) {
            if (subtree_positions[vertex].values.size() <
                subtree_positions[child].values.size()) {
                std::swap(subtree_positions[vertex], subtree_positions[child]);
            }
            for (int position : subtree_positions[child].values) {
                insert(subtree_positions[vertex], position);
            }
            subtree_positions[child].values.clear();
            subtree_positions[child].avoiding = all_subarrays;
        }
        for (int position : positions[vertex]) insert(subtree_positions[vertex], position);
        answer += weight[vertex] *
                  T(all_subarrays - subtree_positions[vertex].avoiding);
    }
    return answer;
}

}
