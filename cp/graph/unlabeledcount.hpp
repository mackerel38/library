#pragma once
#include <bits/stdc++.h>
#include "math/burnside.hpp"

namespace poe {

/// O(p(n)・n^2+n k)。全k色を使う頂点彩色付きn頂点無ラベル単純無向グラフ数を返す。
template<class mint>
mint count_surjectively_colored_unlabeled_graphs(int vertex_count, int color_count) {
    assert(vertex_count >= 0);
    assert(0 <= color_count && color_count <= vertex_count);

    std::vector<mint> surjections(vertex_count + 1);
    std::vector<mint> ways(color_count + 1);
    ways[0] = 1;
    surjections[0] = color_count == 0 ? mint{1} : mint{};
    for (int objects = 1; objects <= vertex_count; ++objects) {
        std::vector<mint> next(color_count + 1);
        for (int used = 0; used <= color_count; ++used) {
            next[used] += ways[used] * mint{used};
            if (used < color_count) {
                next[used + 1] += ways[used] * mint{color_count - used};
            }
        }
        ways = std::move(next);
        surjections[objects] = ways[color_count];
    }

    return symmetric_group_orbit_count<mint>(
        vertex_count,
        [&](const std::vector<int>& cycle_counts) {
            int vertex_cycles = 0;
            int edge_orbits = 0;
            for (int first = 1; first <= vertex_count; ++first) {
                const int first_count = cycle_counts[first];
                vertex_cycles += first_count;
                edge_orbits += first_count * (first / 2);
                edge_orbits += first_count * (first_count - 1) / 2 * first;
                for (int second = 1; second < first; ++second) {
                    edge_orbits += first_count * cycle_counts[second]
                        * std::gcd(first, second);
                }
            }
            return mint{2}.pow(edge_orbits) * surjections[vertex_cycles];
        });
}

}
