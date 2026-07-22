#pragma once
#include <bits/stdc++.h>
#include "utility/presetsegtree.hpp"

namespace poe {

/// range sort一回を表す半開区間と向き。ascending=trueなら昇順、falseなら降順。
struct rangesortquery {
    int left;
    int right;
    bool ascending;
};

/// O((n+q)log n)。相異なる値の列へrange sortを順に行った後のtarget位置を返す。
template<class T>
int position_after_range_sorts(
    const std::vector<T>& values,
    const T& target,
    const std::vector<rangesortquery>& queries
) {
    const int n = static_cast<int>(values.size());
    std::vector<int> less(n);
    int position = -1;
    for (int index = 0; index < n; ++index) {
        less[index] = values[index] < target;
        if (values[index] == target) {
            assert(position == -1);
            position = index;
        }
    }
    assert(position != -1);
    range_set_range_sum<int> data(less);
    for (const auto& query : queries) {
        assert(0 <= query.left && query.left <= query.right && query.right <= n);
        const int count = data.sum(query.left, query.right);
        if (query.ascending) {
            data.set(query.left, query.left + count, 1);
            data.set(query.left + count, query.right, 0);
            if (query.left <= position && position < query.right) {
                position = query.left + count;
            }
        } else {
            data.set(query.left, query.right - count, 0);
            data.set(query.right - count, query.right, 1);
            if (query.left <= position && position < query.right) {
                position = query.right - count - 1;
            }
        }
    }
    return position;
}

}
