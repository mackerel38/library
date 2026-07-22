#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n^3)時間・O(n^2)領域。子をlabel昇順に辿るDFSの行きがけ順がlabelsとなる根付き木数を返す。
template<class T, class Label>
T count_sorted_child_preorder_trees(const std::vector<Label>& labels) {
    const int size = static_cast<int>(labels.size());
    if (size == 0) return T{};
    std::vector<std::vector<T>> trees(size, std::vector<T>(size));
    std::vector<std::vector<T>> attachable(size, std::vector<T>(size));
    for (int right = 0; right < size; ++right) {
        trees[right][right] = T{1};
        attachable[right][right] = T{1};
        for (int left = right - 1; left >= 0; --left) {
            for (int split = left; split < right; ++split) {
                const T ways = attachable[left][split] * trees[split + 1][right];
                trees[left][right] += ways;
                if (right + 1 < size && labels[split + 1] < labels[right + 1]) {
                    attachable[left][right] += ways;
                }
            }
        }
    }
    return trees[0][size - 1];
}

}
