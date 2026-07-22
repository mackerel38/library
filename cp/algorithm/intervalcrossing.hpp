#pragma once
#include <bits/stdc++.h>
#include "structure/fenwicktree.hpp"

namespace poe {

/// O(n log n)時間・O(n)領域。端点がl_i<l_j<r_i<r_jと厳密に交互に現れる区間対数を返す。
template <class T>
long long count_alternating_interval_pairs(
    const std::vector<std::pair<T, T>>& intervals
) {
    std::vector<T> coordinates;
    coordinates.reserve(intervals.size() * 2);
    std::vector<std::pair<T, T>> normalized = intervals;
    for (auto& [left, right] : normalized) {
        if (right < left) std::swap(left, right);
        assert(left < right);
        coordinates.push_back(left);
        coordinates.push_back(right);
    }
    std::ranges::sort(coordinates);
    coordinates.erase(std::unique(coordinates.begin(), coordinates.end()), coordinates.end());

    std::vector<std::vector<int>> opening(coordinates.size()), closing(coordinates.size());
    std::vector<int> left_rank(intervals.size());
    for (int index = 0; index < static_cast<int>(normalized.size()); ++index) {
        const auto [left, right] = normalized[index];
        const int l = static_cast<int>(std::lower_bound(coordinates.begin(), coordinates.end(), left)
                                       - coordinates.begin());
        const int r = static_cast<int>(std::lower_bound(coordinates.begin(), coordinates.end(), right)
                                       - coordinates.begin());
        left_rank[index] = l;
        opening[l].push_back(index);
        closing[r].push_back(index);
    }

    fenwicktree<int> active(static_cast<int>(coordinates.size()));
    long long result = 0;
    for (int rank = 0; rank < static_cast<int>(coordinates.size()); ++rank) {
        for (const int interval : closing[rank]) active.add(left_rank[interval], -1);
        for (const int interval : closing[rank]) {
            result += active.sum(left_rank[interval] + 1, rank);
        }
        for (const int interval : opening[rank]) active.add(rank, 1);
    }
    return result;
}

}
