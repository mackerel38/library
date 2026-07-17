#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n)。odd[i]を中心iの奇数長回文半径として返す。回文長は2*odd[i]-1。
template <class T>
std::vector<int> manacher_odd(const std::vector<T>& sequence) {
    const int n = static_cast<int>(sequence.size());
    std::vector<int> radius(n);
    for (int i = 0, left = 0, right = -1; i < n; ++i) {
        int current = i > right ? 1 : std::min(radius[left + right - i], right - i + 1);
        while (i - current >= 0 && i + current < n &&
               sequence[i - current] == sequence[i + current]) {
            ++current;
        }
        radius[i] = current;
        if (i + current - 1 > right) {
            left = i - current + 1;
            right = i + current - 1;
        }
    }
    return radius;
}

/// O(n)。even[i]を境界(i-1,i)中心の偶数長回文半径として返す。回文長は2*even[i]。
template <class T>
std::vector<int> manacher_even(const std::vector<T>& sequence) {
    const int n = static_cast<int>(sequence.size());
    std::vector<int> radius(n);
    for (int i = 0, left = 0, right = -1; i < n; ++i) {
        int current = i > right ? 0 : std::min(radius[left + right - i + 1], right - i + 1);
        while (i - current - 1 >= 0 && i + current < n &&
               sequence[i - current - 1] == sequence[i + current]) {
            ++current;
        }
        radius[i] = current;
        if (i + current - 1 > right) {
            left = i - current;
            right = i + current - 1;
        }
    }
    return radius;
}

/// O(n)。文字列textの奇数長回文半径を返す。
inline std::vector<int> manacher_odd(const std::string& text) {
    return manacher_odd(std::vector<char>(text.begin(), text.end()));
}

/// O(n)。文字列textの偶数長回文半径を返す。
inline std::vector<int> manacher_even(const std::string& text) {
    return manacher_even(std::vector<char>(text.begin(), text.end()));
}

}
