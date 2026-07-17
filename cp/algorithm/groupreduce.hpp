#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 隣接する同値要素がgroup_size個揃うたび消す: reduce_adjacent_groups(values, k); O(n)。
template <class T, class Equal = std::equal_to<T>>
std::vector<T> reduce_adjacent_groups(const std::vector<T>& values, int group_size,
                                      Equal equal = Equal{}) {
    assert(group_size >= 1);
    if (group_size == 1) return {};
    std::vector<std::pair<T, int>> stack;
    stack.reserve(values.size());
    for (const T& value : values) {
        if (stack.empty() || !std::invoke(equal, stack.back().first, value)) {
            stack.emplace_back(value, 1);
        } else if (++stack.back().second == group_size) {
            stack.pop_back();
        }
    }
    std::vector<T> result;
    result.reserve(values.size());
    for (const auto& [value, count] : stack) {
        result.insert(result.end(), count, value);
    }
    return result;
}

}
