#pragma once
#include <bits/stdc++.h>
#include "math/modint.hpp"

namespace poe {

/// O(m b n^3)。同長wildcard列を辞書順狭義増加へ補完する方法数を返す。
template <class Value>
Value count_strictly_increasing_wildcard_sequences(
    const std::vector<std::vector<int>>& patterns,
    int alphabet_size,
    int wildcard = -1) {
    assert(alphabet_size >= 1);
    if (patterns.empty()) return Value{1};
    const int count = static_cast<int>(patterns.size());
    const int length = static_cast<int>(patterns[0].size());
    for (const auto& pattern : patterns) {
        assert(static_cast<int>(pattern.size()) == length);
        for (const int symbol : pattern) {
            assert(symbol == wildcard || (0 <= symbol && symbol < alphabet_size));
        }
    }

    std::vector<std::vector<Value>> next(count + 1, std::vector<Value>(count + 1));
    for (int left = 0; left <= count; ++left) {
        next[left][left] = 1;
        if (left < count) next[left][left + 1] = 1;
    }
    for (int position = length - 1; position >= 0; --position) {
        std::vector<std::vector<Value>> current(count + 1, std::vector<Value>(count + 1));
        for (int left = 0; left <= count; ++left) {
            current[left][left] = 1;
            std::vector<Value> ways(count + 1);
            ways[left] = 1;
            for (int symbol = 0; symbol < alphabet_size; ++symbol) {
                std::vector<Value> after = ways;
                for (int first = left; first < count; ++first) {
                    if (ways[first] == Value{}) continue;
                    for (int last = first + 1; last <= count; ++last) {
                        const int value = patterns[last - 1][position];
                        if (value != wildcard && value != symbol) break;
                        after[last] += ways[first] * next[first][last];
                    }
                }
                ways = std::move(after);
            }
            for (int right = left; right <= count; ++right) current[left][right] = ways[right];
        }
        next = std::move(current);
    }
    return next[0][count];
}

/// O(m n^3)。数字と?からなる同長文字列を整数として狭義増加へ補完する方法数を返す。
template <int modulus>
staticmodint<modulus> count_strictly_increasing_wildcard_numbers(
    const std::vector<std::string>& patterns) {
    std::vector<std::vector<int>> converted;
    converted.reserve(patterns.size());
    for (const auto& pattern : patterns) {
        std::vector<int> digits;
        digits.reserve(pattern.size());
        for (const char character : pattern) {
            assert(character == '?' || ('0' <= character && character <= '9'));
            digits.push_back(character == '?' ? -1 : character - '0');
        }
        converted.push_back(std::move(digits));
    }
    return count_strictly_increasing_wildcard_sequences<staticmodint<modulus>>(
        converted, 10);
}

}
