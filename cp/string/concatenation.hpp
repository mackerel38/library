#pragma once
#include <bits/stdc++.h>
#include "string/zalgorithm.hpp"

namespace poe {
namespace detail {

/// O(min(|x|,|y|))。xy<yxならtrueを返す。各文字列自身のZ-arrayを受け取る。
inline bool concatenation_less(
    const std::string& x, const std::vector<int>& z_x,
    const std::string& y, const std::vector<int>& z_y
) {
    const int x_length = static_cast<int>(x.size());
    const int y_length = static_cast<int>(y.size());
    if (x_length >= y_length) {
        for (int i = 0; i < y_length; ++i) {
            if (x[i] != y[i]) return x[i] < y[i];
        }
        if (x_length != y_length && z_x[y_length] < x_length - y_length) {
            return x[y_length + z_x[y_length]] < x[z_x[y_length]];
        }
        for (int i = 0; i < y_length; ++i) {
            if (y[i] != x[i + x_length - y_length]) {
                return y[i] < x[i + x_length - y_length];
            }
        }
    } else {
        for (int i = 0; i < x_length; ++i) {
            if (x[i] != y[i]) return x[i] < y[i];
        }
        if (z_y[x_length] < y_length - x_length) {
            return y[z_y[x_length]] < y[x_length + z_y[x_length]];
        }
        for (int i = 0; i < x_length; ++i) {
            if (y[i + y_length - x_length] != x[i]) {
                return y[i + y_length - x_length] < x[i];
            }
        }
    }
    return false;
}

/// O(|x|+|y|)。xy=yxならtrueを返す。
inline bool concatenation_equal(const std::string& x, const std::string& y) {
    for (std::size_t i = 0; i < x.size() + y.size(); ++i) {
        if ((i < x.size() ? x[i] : y[i - x.size()]) !=
            (i < y.size() ? y[i] : x[i - y.size()])) return false;
    }
    return true;
}

}

/// O(sum|s| log n)。全文字列の連結が辞書順最小になる添字順を返す。
inline std::vector<int> concatenation_order(const std::vector<std::string>& strings) {
    std::vector<std::vector<int>> z(strings.size());
    for (int i = 0; i < static_cast<int>(strings.size()); ++i) z[i] = z_algorithm(strings[i]);
    std::vector<int> order(strings.size());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int left, int right) {
        return detail::concatenation_less(
            strings[left], z[left], strings[right], z[right]);
    });
    return order;
}

/// O(sum|s| log n)。全順列の連結を重複込みで辞書順に並べた2番目を返す。要素数2以上。
inline std::string second_concatenation(const std::vector<std::string>& strings) {
    assert(strings.size() >= 2);
    std::vector<int> order = concatenation_order(strings);
    if (order.size() == 2) return strings[order[1]] + strings[order[0]];

    std::size_t total_length = 0;
    for (const auto& value : strings) total_length += value.size();
    auto concatenate = [&] {
        std::string result;
        result.reserve(total_length);
        for (int index : order) result += strings[index];
        return result;
    };
    for (int i = 1; i < static_cast<int>(order.size()); ++i) {
        if (detail::concatenation_equal(strings[order[i - 1]], strings[order[i]])) {
            return concatenate();
        }
    }
    std::swap(order[order.size() - 1], order[order.size() - 2]);
    std::string first = concatenate();
    std::swap(order[order.size() - 1], order[order.size() - 2]);
    std::swap(order[order.size() - 2], order[order.size() - 3]);
    return std::min(first, concatenate());
}

/// O(Lm+km^2+|answer|)。stringsから重複を許してちょうどk個選ぶ連結の辞書順最小を返す。
inline std::string minimum_repeated_concatenation(
    const std::vector<std::string>& strings, int k) {
    assert(!strings.empty() && k >= 0);
    if (k == 0) return {};
    for (const auto& value : strings) assert(!value.empty());

    std::vector<std::vector<int>> z(strings.size());
    for (int i = 0; i < static_cast<int>(strings.size()); ++i) {
        z[i] = z_algorithm(strings[i]);
    }
    int minimum = 0;
    for (int i = 1; i < static_cast<int>(strings.size()); ++i) {
        const bool less = detail::concatenation_less(
            strings[i], z[i], strings[minimum], z[minimum]);
        const bool greater = detail::concatenation_less(
            strings[minimum], z[minimum], strings[i], z[i]);
        if (less || (!greater && strings[i] < strings[minimum])) minimum = i;
    }

    const std::string& period = strings[minimum];
    const int states = static_cast<int>(period.size());
    const int infinity = std::numeric_limits<int>::max() / 4;
    std::vector best(states, std::vector<int>(states, infinity));
    for (int state = 0; state < states; ++state) {
        for (const auto& value : strings) {
            bool matches = true;
            for (int index = 0; index < static_cast<int>(value.size()); ++index) {
                if (value[index] != period[(state + index) % states]) {
                    matches = false;
                    break;
                }
            }
            if (!matches) continue;
            const int next = (state + static_cast<int>(value.size())) % states;
            best[state][next] =
                std::min(best[state][next], static_cast<int>(value.size()));
        }
    }

    std::vector<int> dp(states, infinity), next(states);
    dp[0] = 0;
    for (int used = 0; used < k; ++used) {
        std::fill(next.begin(), next.end(), infinity);
        for (int state = 0; state < states; ++state) {
            if (dp[state] == infinity) continue;
            for (int destination = 0; destination < states; ++destination) {
                if (best[state][destination] == infinity) continue;
                next[destination] = std::min(
                    next[destination], dp[state] + best[state][destination]);
            }
        }
        dp.swap(next);
    }
    const int length = *std::min_element(dp.begin(), dp.end());
    assert(length < infinity);
    std::string result(length, '\0');
    for (int index = 0; index < length; ++index) result[index] = period[index % states];
    return result;
}

}
