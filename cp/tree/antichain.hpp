#pragma once
#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "tree/statictoptree.hpp"

namespace poe {

namespace detail {

template <int modulus>
struct antichainpath {
    fps<modulus> scale;
    fps<modulus> shift;
};

}

/// O(M(n) log n)。根付き木で祖先・子孫を同時に含まないk頂点集合の個数をk=0..nで返す。
template <int modulus = 998244353, undirected_graph_type Graph>
std::vector<staticmodint<modulus>> tree_antichain_counts(
    const Graph& graph,
    int root = 0
) {
    using series = fps<modulus>;
    using path = detail::antichainpath<modulus>;
    const int n = graph.size();
    if (n == 0) return {1};
    statictoptree tree(graph, root);
    const series one{1};
    const series variable{0, 1};
    auto dp = make_statictoptreedp<path, series>(
        tree,
        [&](int) { return path{one, variable}; },
        [](const path& parent, const path& child) {
            return path{
                parent.scale * child.scale,
                parent.scale * child.shift + parent.shift,
            };
        },
        [](const series& left, const series& right) { return left * right; },
        [](const path& child) { return child.scale + child.shift; },
        [&](const series& children, int) { return path{children, variable}; }
    );
    series answer = dp.get().scale + dp.get().shift;
    answer.resize(static_cast<std::size_t>(n + 1));
    return answer;
}

}
