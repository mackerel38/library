#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(nk)。互いに交わらない高々k個の非空部分配列を選んだ総和の最大値を返す。
template <class T>
T maximum_k_disjoint_subarray_sum(const std::vector<T>& values, int k) {
    assert(k >= 0);
    const T negative_infinity = std::numeric_limits<T>::lowest() / T{4};
    std::vector<T> outside(k + 1, negative_infinity);
    std::vector<T> inside(k + 1, negative_infinity);
    outside[0] = T{};
    for (const T& value : values) {
        auto next_outside = outside;
        auto next_inside = inside;
        for (int count = 1; count <= k; ++count) {
            next_outside[count] = std::max(outside[count], inside[count]);
            next_inside[count] = std::max(inside[count], outside[count - 1]) + value;
        }
        outside = std::move(next_outside);
        inside = std::move(next_inside);
    }
    T answer{};
    for (int count = 0; count <= k; ++count) {
        answer = std::max(answer, outside[count]);
        answer = std::max(answer, inside[count]);
    }
    return answer;
}

}
