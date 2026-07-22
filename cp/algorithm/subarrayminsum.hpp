#pragma once
#include <bits/stdc++.h>
#include "tree/cartesiantree.hpp"

namespace poe {

/// O(n log^2 n)。非負weightsについてmin(minimums[l:r])+sum(weights[l:r])<=limitとなる非空区間数。
template<class Minimum, class Weight, class Limit>
long long count_subarrays_min_plus_sum_at_most(
    const std::vector<Minimum>& minimums,
    const std::vector<Weight>& weights,
    Limit limit
) {
    assert(minimums.size() == weights.size());
    assert(std::ranges::all_of(weights, [](const Weight& value) { return value >= Weight{}; }));
    const int n = static_cast<int>(minimums.size());
    if (n == 0) return 0;

    using sum_type = std::common_type_t<Minimum, Weight, Limit, long long>;
    std::vector<sum_type> prefix(n + 1);
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + weights[i];

    const auto tree = cartesian_tree(minimums);
    std::vector<int> order;
    order.reserve(n);
    order.push_back(tree.root);
    for (int index = 0; index < static_cast<int>(order.size()); ++index) {
        const int vertex = order[index];
        if (tree.left[vertex] != -1) order.push_back(tree.left[vertex]);
        if (tree.right[vertex] != -1) order.push_back(tree.right[vertex]);
    }
    std::vector<int> subtree_left(n), subtree_right(n);
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        const int vertex = *it;
        subtree_left[vertex] = tree.left[vertex] == -1
            ? vertex : subtree_left[tree.left[vertex]];
        subtree_right[vertex] = tree.right[vertex] == -1
            ? vertex : subtree_right[tree.right[vertex]];
    }

    long long answer = 0;
    for (int minimum_index = 0; minimum_index < n; ++minimum_index) {
        const sum_type capacity = static_cast<sum_type>(limit)
            - static_cast<sum_type>(minimums[minimum_index]);
        if (capacity < sum_type{}) continue;
        const int left = subtree_left[minimum_index];
        const int right = subtree_right[minimum_index];
        const int left_choices = minimum_index - left + 1;
        const int right_choices = right - minimum_index + 1;
        if (left_choices <= right_choices) {
            for (int begin = left; begin <= minimum_index; ++begin) {
                const auto first = prefix.begin() + minimum_index + 1;
                const auto last = prefix.begin() + right + 2;
                answer += std::upper_bound(first, last, prefix[begin] + capacity) - first;
            }
        } else {
            for (int end = minimum_index; end <= right; ++end) {
                const auto first = prefix.begin() + left;
                const auto last = prefix.begin() + minimum_index + 1;
                answer += last - std::lower_bound(
                    first, last, prefix[end + 1] - capacity);
            }
        }
    }
    return answer;
}

}
