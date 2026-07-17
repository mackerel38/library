#pragma once
#include <bits/stdc++.h>
#include "math/convolution.hpp"

namespace poe {

/// O((n+m) log(n+m))。wildcard_matches(text,pattern,wildcard)で一致する開始位置を返す。
/// textとpatternのどちらでもwildcardは任意の一文字に一致する。空patternは全境界に一致する。
inline std::vector<int> wildcard_matches(
    const std::string& text,
    const std::string& pattern,
    char wildcard = '_'
) {
    const int text_size = static_cast<int>(text.size());
    const int pattern_size = static_cast<int>(pattern.size());
    if (pattern_size == 0) {
        std::vector<int> result(static_cast<std::size_t>(text_size) + 1);
        std::iota(result.begin(), result.end(), 0);
        return result;
    }
    if (pattern_size > text_size) return {};

    std::array<int, 256> rank;
    rank.fill(-1);
    int alphabet_size = 0;
    for (const std::string* source : {&text, &pattern}) {
        for (const unsigned char symbol : *source) {
            if (symbol == static_cast<unsigned char>(wildcard)) continue;
            if (rank[symbol] == -1) rank[symbol] = alphabet_size++;
        }
    }
    std::vector<long long> text_value(text_size), text_square(text_size);
    std::vector<long long> text_mask(text_size);
    for (int index = 0; index < text_size; ++index) {
        if (text[index] == wildcard) continue;
        text_value[index] = rank[static_cast<unsigned char>(text[index])];
        text_square[index] = text_value[index] * text_value[index];
        text_mask[index] = 1;
    }
    std::vector<long long> pattern_value(pattern_size), pattern_square(pattern_size);
    std::vector<long long> pattern_mask(pattern_size);
    for (int index = 0; index < pattern_size; ++index) {
        if (pattern[index] == wildcard) continue;
        const int reversed = pattern_size - 1 - index;
        pattern_value[reversed] = rank[static_cast<unsigned char>(pattern[index])];
        pattern_square[reversed] = pattern_value[reversed] * pattern_value[reversed];
        pattern_mask[reversed] = 1;
    }

    std::vector<long long> first, second, cross;
    const long long maximum_difference = std::max(0, alphabet_size - 1);
    const __int128 score_bound =
        static_cast<__int128>(pattern_size) * maximum_difference * maximum_difference;
    if (score_bound < 998244353) {
        first = convolution<998244353>(text_square, pattern_mask);
        second = convolution<998244353>(text_mask, pattern_square);
        cross = convolution<998244353>(text_value, pattern_value);
    } else {
        first = convolution_ll(text_square, pattern_mask);
        second = convolution_ll(text_mask, pattern_square);
        cross = convolution_ll(text_value, pattern_value);
    }
    std::vector<int> result;
    for (int start = 0; start + pattern_size <= text_size; ++start) {
        const int coefficient = start + pattern_size - 1;
        const long long mismatch =
            first[coefficient] + second[coefficient] - 2 * cross[coefficient];
        if (mismatch == 0) result.push_back(start);
    }
    return result;
}

}
