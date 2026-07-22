#pragma once
#include <bits/stdc++.h>

namespace poe {

/// Stern--Brocot木上のL/R移動を同方向runごとに持つ。
struct sternbrocotpath {
    std::vector<std::pair<bool, long long>> runs;
    long long depth = -1;
};

/// O(log max(p,q))。既約なp/qへのStern--Brocot pathをrun-lengthで返す。
inline sternbrocotpath stern_brocot_path(long long numerator, long long denominator) {
    assert(numerator >= 0 && denominator >= 0);
    assert(std::gcd(numerator, denominator) == 1);
    if (numerator == 0 || denominator == 0) return {};
    sternbrocotpath result;
    result.depth = 0;
    while (numerator != denominator) {
        if (numerator < denominator) {
            const long long count = (denominator - 1) / numerator;
            result.runs.emplace_back(false, count);
            result.depth += count;
            denominator -= count * numerator;
        } else {
            const long long count = (numerator - 1) / denominator;
            result.runs.emplace_back(true, count);
            result.depth += count;
            numerator -= count * denominator;
        }
    }
    return result;
}

/// 指定した既約分数だけを含む圧縮Stern--Brocot木。leaf iはpoints[i]に対応する。
struct sternbrocotvirtualtree {
    int root = -1;
    int leaves = 0;
    std::vector<int> parent;
    std::vector<std::vector<int>> children;
    std::vector<long long> depth;
};

/// O(n log n+n log V)。既約分数pointsを含むStern--Brocot仮想木を返す。
inline sternbrocotvirtualtree stern_brocot_virtual_tree(
    const std::vector<std::pair<long long, long long>>& points
) {
    const int n = static_cast<int>(points.size());
    sternbrocotvirtualtree result;
    result.leaves = n;
    if (n == 0) return result;

    std::vector<sternbrocotpath> paths(n);
    for (int index = 0; index < n; ++index) {
        paths[index] = stern_brocot_path(points[index].first, points[index].second);
    }
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::ranges::sort(order, [&](int left, int right) {
        const auto [a, b] = points[left];
        const auto [c, d] = points[right];
        if (b == 0 || d == 0) return b != 0;
        return static_cast<__int128>(a) * d < static_cast<__int128>(c) * b;
    });

    const auto lcp = [&](int first, int second) {
        if (paths[first].depth < 0 || paths[second].depth < 0) return -1LL;
        const auto& left = paths[first].runs;
        const auto& right = paths[second].runs;
        std::size_t i = 0;
        std::size_t j = 0;
        long long left_used = 0;
        long long right_used = 0;
        long long common = 0;
        while (i < left.size() && j < right.size() &&
               left[i].first == right[j].first) {
            const long long take = std::min(
                left[i].second - left_used, right[j].second - right_used);
            common += take;
            left_used += take;
            right_used += take;
            if (left_used == left[i].second) {
                ++i;
                left_used = 0;
            }
            if (right_used == right[j].second) {
                ++j;
                right_used = 0;
            }
        }
        return common;
    };

    struct edge {
        long long depth;
        int position;
    };
    std::vector<edge> edges;
    for (int position = 0; position + 1 < n; ++position) {
        edges.push_back({lcp(order[position], order[position + 1]), position});
    }
    std::ranges::sort(edges, std::greater{}, &edge::depth);

    result.parent.assign(n, -1);
    result.children.resize(n);
    result.depth.resize(n);
    for (int index = 0; index < n; ++index) result.depth[index] = paths[index].depth;
    std::vector<int> dsu_parent(n);
    std::iota(dsu_parent.begin(), dsu_parent.end(), 0);
    std::vector<int> component_node(n);
    for (int position = 0; position < n; ++position) {
        component_node[position] = order[position];
    }
    const auto find = [&](auto&& self, int vertex) -> int {
        if (dsu_parent[vertex] == vertex) return vertex;
        return dsu_parent[vertex] = self(self, dsu_parent[vertex]);
    };
    for (const auto [depth, position] : edges) {
        int left = find(find, position);
        int right = find(find, position + 1);
        assert(left != right);
        const int node = static_cast<int>(result.parent.size());
        result.parent.push_back(-1);
        result.children.push_back({component_node[left], component_node[right]});
        result.depth.push_back(depth);
        result.parent[component_node[left]] = node;
        result.parent[component_node[right]] = node;
        dsu_parent[right] = left;
        component_node[left] = node;
    }
    const int component = find(find, 0);
    const int root = static_cast<int>(result.parent.size());
    result.root = root;
    result.parent.push_back(-1);
    result.children.push_back({component_node[component]});
    result.depth.push_back(-1);
    result.parent[component_node[component]] = root;
    return result;
}

}
