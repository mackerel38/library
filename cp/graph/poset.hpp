#pragma once
#include <bits/stdc++.h>
#include "graph/matching.hpp"

namespace poe {

/// 半順序の最大反鎖と、それと同数の最小chain分割。
struct posetantichainresult {
    std::vector<int> antichain;
    std::vector<std::vector<int>> chains;

    /// O(1)。最大反鎖の大きさを返す。
    int size() const noexcept { return static_cast<int>(antichain.size()); }
};

/// O(n^2+n^2sqrt(n))。strict order less(i,j)で与えた半順序の最大反鎖と最小chain分割を返す。
template<class StrictOrder>
posetantichainresult maximum_poset_antichain(int size, StrictOrder less) {
    assert(size >= 0);
    bipartitematching matching(size, size);
    for (int left = 0; left < size; ++left) {
        assert(!less(left, left));
        for (int right = 0; right < size; ++right) {
            if (less(left, right)) matching.add_edge(left, right);
        }
    }
    const auto solved = matching.solve();
    std::vector<char> left_cover(size), right_cover(size);
    for (const int vertex : solved.left_cover) left_cover[vertex] = true;
    for (const int vertex : solved.right_cover) right_cover[vertex] = true;

    posetantichainresult result;
    for (int vertex = 0; vertex < size; ++vertex) {
        if (!left_cover[vertex] && !right_cover[vertex]) result.antichain.push_back(vertex);
    }
    for (int start = 0; start < size; ++start) {
        if (solved.right[start] != -1) continue;
        result.chains.push_back({});
        for (int vertex = start; vertex != -1; vertex = solved.left[vertex]) {
            result.chains.back().push_back(vertex);
        }
    }
    assert(result.antichain.size() == result.chains.size());
    return result;
}

}
