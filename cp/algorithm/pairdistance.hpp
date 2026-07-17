#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 二列から一つずつ選ぶ全組の絶対差和を返す: sum_cross_absolute(a, b); O((n+m)log n)。
template <class T>
T sum_cross_absolute(std::vector<T> left, const std::vector<T>& right) {
    std::ranges::sort(left);
    std::vector<T> prefix(left.size() + 1);
    for (int i = 0; i < static_cast<int>(left.size()); ++i) prefix[i + 1] = prefix[i] + left[i];
    T answer{};
    for (const T& value : right) {
        const int middle = std::ranges::lower_bound(left, value) - left.begin();
        answer += value * middle - prefix[middle];
        answer += prefix.back() - prefix[middle] - value * (static_cast<int>(left.size()) - middle);
    }
    return answer;
}

}
