#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n)。連続する等しい要素を(value, count)へまとめる。
template <std::ranges::input_range Range>
auto runlength(const Range& sequence) {
    using T = std::ranges::range_value_t<Range>;
    std::vector<std::pair<T, int>> result;
    for (const auto& value : sequence) {
        if (result.empty() || result.back().first != value) {
            result.emplace_back(value, 1);
        } else {
            ++result.back().second;
        }
    }
    return result;
}

/// O(復元後の長さ)。非負のcountを持つ(value, count)列を元の列へ戻す。
template <class T>
std::vector<T> runlength_decode(const std::vector<std::pair<T, int>>& runs) {
    std::size_t size = 0;
    for (const auto& [value, count] : runs) {
        static_cast<void>(value);
        assert(count >= 0);
        size += static_cast<std::size_t>(count);
    }
    std::vector<T> result;
    result.reserve(size);
    for (const auto& [value, count] : runs) {
        result.insert(result.end(), count, value);
    }
    return result;
}

/// O(復元後の長さ)。charの(value, count)列を文字列へ戻す。
inline std::string runlength_string(const std::vector<std::pair<char, int>>& runs) {
    const auto decoded = runlength_decode(runs);
    return std::string(decoded.begin(), decoded.end());
}

}
