#pragma once
#include <bits/stdc++.h>
#include "string/lcp.hpp"
#include "string/suffixarray.hpp"

namespace poe {

/// O(n log n)。forbiddenを含まない相異なる空でない部分文字列の個数を返す。
template <class T>
long long count_distinct_substrings_without(
    const std::vector<T>& sequence,
    const T& forbidden
) {
    const int n = static_cast<int>(sequence.size());
    const auto suffix = suffix_array(sequence);
    const auto adjacent = lcp_array(sequence, suffix);
    std::vector<int> valid_length(n);
    int next_forbidden = n;
    for (int index = n - 1; index >= 0; --index) {
        if (sequence[index] == forbidden) next_forbidden = index;
        valid_length[index] = next_forbidden - index;
    }
    long long answer = 0;
    for (int order = 0; order < n; ++order) {
        const int duplicate = order == 0 ? 0 : adjacent[order - 1];
        answer += std::max(0, valid_length[suffix[order]] - duplicate);
    }
    return answer;
}

/// O(n log n)。forbiddenを含まない相異なる空でない部分文字列の個数を返す。
inline long long count_distinct_substrings_without(
    const std::string& text,
    char forbidden
) {
    return count_distinct_substrings_without(
        std::vector<char>(text.begin(), text.end()), forbidden
    );
}

}
