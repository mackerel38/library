#pragma once
#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "math/modint.hpp"
#include "math/subsettransform.hpp"

namespace poe {

namespace detail {

using coloringgraph = std::vector<std::uint32_t>;

inline coloringgraph erase_coloring_vertex(const coloringgraph& graph, int removed) {
    const int size = static_cast<int>(graph.size());
    std::vector<int> index(size, -1);
    for (int vertex = 0, next = 0; vertex < size; ++vertex) {
        if (vertex != removed) index[vertex] = next++;
    }
    coloringgraph result(size - 1);
    for (int left = 0; left < size; ++left) {
        if (left == removed) continue;
        for (int right = left + 1; right < size; ++right) {
            if (right == removed || !(graph[left] >> right & 1U)) continue;
            result[index[left]] |= std::uint32_t{1} << index[right];
            result[index[right]] |= std::uint32_t{1} << index[left];
        }
    }
    return result;
}

inline coloringgraph contract_coloring_vertices(
    const coloringgraph& graph, int kept, int removed) {
    assert(kept != removed);
    assert(!(graph[kept] >> removed & 1U));
    const int size = static_cast<int>(graph.size());
    std::vector<int> index(size, -1);
    for (int vertex = 0, next = 0; vertex < size; ++vertex) {
        if (vertex == removed) continue;
        index[vertex] = next++;
    }
    index[removed] = index[kept];
    coloringgraph result(size - 1);
    for (int left = 0; left < size; ++left) {
        for (int right = left + 1; right < size; ++right) {
            if (!(graph[left] >> right & 1U)) continue;
            const int new_left = index[left];
            const int new_right = index[right];
            if (new_left == new_right) continue;
            result[new_left] |= std::uint32_t{1} << new_right;
            result[new_right] |= std::uint32_t{1} << new_left;
        }
    }
    return result;
}

template <int modulus>
staticmodint<modulus> count_graph_colorings_recursive(
    const coloringgraph& graph, long long colors) {
    using mint = staticmodint<modulus>;
    const int size = static_cast<int>(graph.size());
    if (size == 0) return 1;

    int vertex = 0;
    for (int current = 1; current < size; ++current) {
        if (std::popcount(graph[current]) < std::popcount(graph[vertex])) vertex = current;
    }
    const int degree = std::popcount(graph[vertex]);
    if (degree == 0) {
        return mint{colors} *
               count_graph_colorings_recursive<modulus>(
                   erase_coloring_vertex(graph, vertex), colors);
    }
    if (degree == 1) {
        return mint{colors - 1} *
               count_graph_colorings_recursive<modulus>(
                   erase_coloring_vertex(graph, vertex), colors);
    }
    if (degree == 2) {
        const int first = std::countr_zero(graph[vertex]);
        const int second = std::countr_zero(
            graph[vertex] & (graph[vertex] - 1));
        auto erased = erase_coloring_vertex(graph, vertex);
        const int mapped_first = first - (first > vertex);
        const int mapped_second = second - (second > vertex);
        const mint different =
            count_graph_colorings_recursive<modulus>(erased, colors);
        mint same = 0;
        if (!(erased[mapped_first] >> mapped_second & 1U)) {
            same = count_graph_colorings_recursive<modulus>(
                contract_coloring_vertices(erased, mapped_first, mapped_second), colors);
        }
        return mint{colors - 2} * different + same;
    }

    assert(size <= 24);
    const int count = 1 << size;
    std::vector<mint> independent(count);
    independent[0] = 1;
    for (int mask = 1; mask < count; ++mask) {
        const int current = std::countr_zero(static_cast<unsigned int>(mask));
        const int rest = mask & (mask - 1);
        independent[mask] =
            independent[rest].val() != 0 && !(graph[current] & rest) ? mint{1} : mint{0};
    }
    return subset_convolution_unit_power(independent, colors).back();
}

}

/// O(m^2 1.59^m)程度。30辺以下の単純無向graphのK彩色数を返す。
template <int modulus, undirected_graph_type Graph>
staticmodint<modulus> count_graph_colorings(const Graph& graph, long long colors) {
    assert(0 <= graph.size() && graph.size() <= 30);
    assert(0 <= graph.edge_count() && graph.edge_count() <= 30);
    assert(colors >= 0);
    detail::coloringgraph adjacency(graph.size());
    for (const auto& current : graph.edges()) {
        assert(current.from != current.to);
        assert(!(adjacency[current.from] >> current.to & 1U));
        adjacency[current.from] |= std::uint32_t{1} << current.to;
        adjacency[current.to] |= std::uint32_t{1} << current.from;
    }
    return detail::count_graph_colorings_recursive<modulus>(adjacency, colors);
}

}
