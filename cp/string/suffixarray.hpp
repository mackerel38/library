#pragma once
#include <bits/stdc++.h>

namespace poe {

namespace detail {

/// O(n^2 log n)。小さい列のsuffix arrayを返す。
template <class T>
std::vector<int> suffix_array_naive(const std::vector<T>& sequence) {
    const int n = static_cast<int>(sequence.size());
    std::vector<int> result(n);
    std::iota(result.begin(), result.end(), 0);
    std::sort(result.begin(), result.end(), [&](int left, int right) {
        if (left == right) {
            return false;
        }
        while (left < n && right < n) {
            if (sequence[left] != sequence[right]) {
                return sequence[left] < sequence[right];
            }
            ++left;
            ++right;
        }
        return left == n;
    });
    return result;
}

/// O(n log^2 n)。doubling法でsuffix arrayを返す。
template <class T>
std::vector<int> suffix_array_doubling(const std::vector<T>& sequence) {
    const int n = static_cast<int>(sequence.size());
    std::vector<int> result(n), rank(sequence.begin(), sequence.end()), next_rank(n);
    std::iota(result.begin(), result.end(), 0);
    for (int length = 1; length < n; length <<= 1) {
        auto less = [&](int left, int right) {
            if (rank[left] != rank[right]) {
                return rank[left] < rank[right];
            }
            const int left_next = left + length < n ? rank[left + length] : -1;
            const int right_next = right + length < n ? rank[right + length] : -1;
            return left_next < right_next;
        };
        std::sort(result.begin(), result.end(), less);
        next_rank[result[0]] = 0;
        for (int i = 1; i < n; ++i) {
            next_rank[result[i]] = next_rank[result[i - 1]] +
                                   static_cast<int>(less(result[i - 1], result[i]));
        }
        rank.swap(next_rank);
    }
    return result;
}

/// O(n+upper)。SA-ISで0以上upper以下の整数列のsuffix arrayを返す。
template <class T>
std::vector<int> suffix_array_is(const std::vector<T>& sequence, int upper) {
    const int n = static_cast<int>(sequence.size());
    if (n == 0) {
        return {};
    }
    if (n == 1) {
        return {0};
    }
    if (n == 2) {
        return sequence[0] < sequence[1] ? std::vector<int>{0, 1}
                                         : std::vector<int>{1, 0};
    }
    if (n < 10) {
        return suffix_array_naive(sequence);
    }
    if (n < 40) {
        return suffix_array_doubling(sequence);
    }
    std::vector<int> suffix_array(n);
    std::vector<bool> is_s(n);
    for (int i = n - 2; i >= 0; --i) {
        is_s[i] = sequence[i] == sequence[i + 1] ? is_s[i + 1]
                                                  : sequence[i] < sequence[i + 1];
    }
    std::vector<int> sum_l(upper + 1), sum_s(upper + 1);
    for (int i = 0; i < n; ++i) {
        if (is_s[i]) {
            ++sum_l[sequence[i] + 1];
        } else {
            ++sum_s[sequence[i]];
        }
    }
    for (int value = 0; value <= upper; ++value) {
        sum_s[value] += sum_l[value];
        if (value < upper) {
            sum_l[value + 1] += sum_s[value];
        }
    }
    auto induce = [&](const std::vector<int>& lms) {
        std::fill(suffix_array.begin(), suffix_array.end(), -1);
        std::vector<int> buffer = sum_s;
        for (int position : lms) {
            if (position != n) {
                suffix_array[buffer[sequence[position]]++] = position;
            }
        }
        buffer = sum_l;
        suffix_array[buffer[sequence[n - 1]]++] = n - 1;
        for (int i = 0; i < n; ++i) {
            const int position = suffix_array[i];
            if (position >= 1 && !is_s[position - 1]) {
                suffix_array[buffer[sequence[position - 1]]++] = position - 1;
            }
        }
        buffer = sum_l;
        for (int i = n - 1; i >= 0; --i) {
            const int position = suffix_array[i];
            if (position >= 1 && is_s[position - 1]) {
                suffix_array[--buffer[sequence[position - 1] + 1]] = position - 1;
            }
        }
    };
    std::vector<int> lms_map(n + 1, -1);
    int lms_count = 0;
    for (int i = 1; i < n; ++i) {
        if (!is_s[i - 1] && is_s[i]) {
            lms_map[i] = lms_count++;
        }
    }
    std::vector<int> lms;
    lms.reserve(lms_count);
    for (int i = 1; i < n; ++i) {
        if (!is_s[i - 1] && is_s[i]) {
            lms.push_back(i);
        }
    }
    induce(lms);
    if (lms_count > 0) {
        std::vector<int> sorted_lms;
        sorted_lms.reserve(lms_count);
        for (int position : suffix_array) {
            if (lms_map[position] != -1) {
                sorted_lms.push_back(position);
            }
        }
        std::vector<int> recursive_sequence(lms_count);
        int recursive_upper = 0;
        recursive_sequence[lms_map[sorted_lms[0]]] = 0;
        for (int i = 1; i < lms_count; ++i) {
            int left = sorted_lms[i - 1];
            int right = sorted_lms[i];
            const int end_left = lms_map[left] + 1 < lms_count
                                     ? lms[lms_map[left] + 1]
                                     : n;
            const int end_right = lms_map[right] + 1 < lms_count
                                      ? lms[lms_map[right] + 1]
                                      : n;
            bool same = end_left - left == end_right - right;
            if (same) {
                while (left < end_left && sequence[left] == sequence[right]) {
                    ++left;
                    ++right;
                }
                if (left == n || sequence[left] != sequence[right]) {
                    same = false;
                }
            }
            if (!same) {
                ++recursive_upper;
            }
            recursive_sequence[lms_map[sorted_lms[i]]] = recursive_upper;
        }
        const auto recursive_suffix_array =
            suffix_array_is(recursive_sequence, recursive_upper);
        for (int i = 0; i < lms_count; ++i) {
            sorted_lms[i] = lms[recursive_suffix_array[i]];
        }
        induce(sorted_lms);
    }
    return suffix_array;
}

}

/// O(n+upper)。0以上upper以下の整数列sequenceのsuffix arrayを返す。
inline std::vector<int> suffix_array(const std::vector<int>& sequence, int upper) {
    assert(upper >= 0);
    for (int value : sequence) {
        assert(0 <= value && value <= upper);
    }
    return detail::suffix_array_is(sequence, upper);
}

/// O(n log n)。比較可能な列sequenceのsuffix arrayを返す。追加領域O(n)。
template <class T>
std::vector<int> suffix_array(const std::vector<T>& sequence) {
    const int n = static_cast<int>(sequence.size());
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(),
              [&](int left, int right) { return sequence[left] < sequence[right]; });
    std::vector<int> compressed(n);
    int upper = 0;
    for (int i = 0; i < n; ++i) {
        if (i > 0 && sequence[order[i - 1]] != sequence[order[i]]) {
            ++upper;
        }
        compressed[order[i]] = upper;
    }
    return detail::suffix_array_is(compressed, upper);
}

/// O(n)。文字列textのsuffix arrayを返す。追加領域O(n)。
inline std::vector<int> suffix_array(const std::string& text) {
    std::vector<int> sequence(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        sequence[i] = static_cast<unsigned char>(text[i]);
    }
    return detail::suffix_array_is(sequence, 255);
}

}
