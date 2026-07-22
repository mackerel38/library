#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// O(n r^2 log_r n)時間・O(n r log_r n)領域。
/// parent順に頂点を追加した各prefixで、根0の完全r分木となる誘導部分木数を返す。
template <int mod>
std::vector<staticmodint<mod>> perfect_ary_subtree_prefix_counts(
    const std::vector<int>& parent,
    int arity = 2
) {
    using mint = staticmodint<mod>;
    const int n = static_cast<int>(parent.size());
    assert(arity >= 2);
    if (n == 0) return {};
    assert(parent[0] == -1);
    for (int vertex = 1; vertex < n; ++vertex) {
        assert(0 <= parent[vertex] && parent[vertex] < vertex);
    }

    int levels = 0;
    long long vertices = 1;
    while (vertices <= n) {
        ++levels;
        if (vertices > (n - 1LL) / arity) break;
        vertices = vertices * arity + 1;
    }
    const auto dp_index = [levels](int vertex, int height) {
        return static_cast<std::size_t>(vertex) * levels + height;
    };
    const auto symmetric_index = [levels, arity](int vertex, int height, int degree) {
        return (static_cast<std::size_t>(vertex) * levels + height) * (arity + 1) + degree;
    };

    std::vector<mint> dp(static_cast<std::size_t>(n) * levels);
    std::vector<mint> symmetric(
        static_cast<std::size_t>(n) * levels * (arity + 1)
    );
    for (int vertex = 0; vertex < n; ++vertex) {
        for (int height = 0; height < levels; ++height) {
            symmetric[symmetric_index(vertex, height, 0)] = 1;
        }
    }

    std::vector<mint> answer(n);
    mint total = 0;
    std::vector<mint> quotient(arity + 1), updated(arity + 1);
    for (int added = 0; added < n; ++added) {
        int vertex = added;
        int height = 0;
        mint old_value = 0;
        mint new_value = 1;
        dp[dp_index(vertex, 0)] = 1;
        if (vertex == 0) total += 1;

        while (parent[vertex] != -1 && height + 1 < levels) {
            const int ancestor = parent[vertex];
            quotient[0] = 1;
            updated[0] = 1;
            for (int degree = 1; degree <= arity; ++degree) {
                const mint current = symmetric[
                    symmetric_index(ancestor, height, degree)
                ];
                quotient[degree] = current - old_value * quotient[degree - 1];
                updated[degree] = quotient[degree] + new_value * quotient[degree - 1];
            }
            const mint delta = updated[arity] - symmetric[
                symmetric_index(ancestor, height, arity)
            ];
            for (int degree = 1; degree <= arity; ++degree) {
                symmetric[symmetric_index(ancestor, height, degree)] = updated[degree];
            }
            if (delta == mint{}) break;

            const mint ancestor_old = dp[dp_index(ancestor, height + 1)];
            const mint ancestor_new = ancestor_old + delta;
            dp[dp_index(ancestor, height + 1)] = ancestor_new;
            if (ancestor == 0) total += delta;
            vertex = ancestor;
            ++height;
            old_value = ancestor_old;
            new_value = ancestor_new;
        }
        answer[added] = total;
    }
    return answer;
}

}
