#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(nm)。maxplus_convolution(a,b)[k]=max_{i+j=k}(a[i]+b[j])を返す。
template <class T>
std::vector<T> maxplus_convolution(const std::vector<T>& left, const std::vector<T>& right) {
    if (left.empty() || right.empty()) return {};
    std::vector<T> result(left.size() + right.size() - 1, std::numeric_limits<T>::lowest());
    for (std::size_t i = 0; i < left.size(); ++i) {
        for (std::size_t j = 0; j < right.size(); ++j) result[i + j] = std::max(result[i + j], left[i] + right[j]);
    }
    return result;
}

/// O(n+m)。両列が離散凹ならmax-plus畳み込みを傾きのmergeで返す。
template <class T>
std::vector<T> maxplus_convolution_concave(const std::vector<T>& left, const std::vector<T>& right) {
    if (left.empty() || right.empty()) return {};
    std::vector<T> left_difference, right_difference;
    for (std::size_t i = 1; i < left.size(); ++i) left_difference.push_back(left[i] - left[i - 1]);
    for (std::size_t i = 1; i < right.size(); ++i) right_difference.push_back(right[i] - right[i - 1]);
    assert(std::ranges::is_sorted(left_difference, std::greater<T>()));
    assert(std::ranges::is_sorted(right_difference, std::greater<T>()));
    std::vector<T> result{left[0] + right[0]};
    std::size_t i = 0, j = 0;
    while (i < left_difference.size() || j < right_difference.size()) {
        if (j == right_difference.size() ||
            (i < left_difference.size() && right_difference[j] < left_difference[i])) {
            result.push_back(result.back() + left_difference[i++]);
        } else {
            result.push_back(result.back() + right_difference[j++]);
        }
    }
    return result;
}

/// O(nm)。minplus_convolution(a,b)[k]=min_{i+j=k}(a[i]+b[j])を返す。
template <class T>
std::vector<T> minplus_convolution(const std::vector<T>& left, const std::vector<T>& right) {
    if (left.empty() || right.empty()) return {};
    std::vector<T> result(left.size() + right.size() - 1, std::numeric_limits<T>::max());
    for (std::size_t i = 0; i < left.size(); ++i) {
        for (std::size_t j = 0; j < right.size(); ++j) result[i + j] = std::min(result[i + j], left[i] + right[j]);
    }
    return result;
}

/// O(n+m)。両列が離散凸ならmin-plus畳み込みを傾きのmergeで返す。
template <class T>
std::vector<T> minplus_convolution_convex(const std::vector<T>& left, const std::vector<T>& right) {
    if (left.empty() || right.empty()) return {};
    std::vector<T> left_difference, right_difference;
    for (std::size_t i = 1; i < left.size(); ++i) left_difference.push_back(left[i] - left[i - 1]);
    for (std::size_t i = 1; i < right.size(); ++i) right_difference.push_back(right[i] - right[i - 1]);
    assert(std::ranges::is_sorted(left_difference));
    assert(std::ranges::is_sorted(right_difference));
    std::vector<T> result{left[0] + right[0]};
    std::size_t i = 0, j = 0;
    while (i < left_difference.size() || j < right_difference.size()) {
        if (j == right_difference.size() ||
            (i < left_difference.size() && left_difference[i] < right_difference[j])) {
            result.push_back(result.back() + left_difference[i++]);
        } else {
            result.push_back(result.back() + right_difference[j++]);
        }
    }
    return result;
}

}
