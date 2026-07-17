#pragma once
#include <bits/stdc++.h>
#include "string/prefix.hpp"

namespace poe {

/// O(n)。compareに関して辞書順最小の巡回シフト開始位置を返す。空列は0。
template <class T, class Compare = std::less<T>>
int rotation_index(const std::vector<T>& sequence, Compare compare = {}) {
    const int n = static_cast<int>(sequence.size());
    if (n == 0) return 0;
    int first = 0;
    int second = 1;
    int offset = 0;
    while (first < n && second < n && offset < n) {
        const T& left = sequence[(first + offset) % n];
        const T& right = sequence[(second + offset) % n];
        if (!compare(left, right) && !compare(right, left)) {
            ++offset;
            continue;
        }
        if (compare(left, right)) {
            second += offset + 1;
            if (second == first) ++second;
        } else {
            first += offset + 1;
            if (first == second) ++first;
        }
        offset = 0;
    }
    return std::min(first, second);
}

/// O(n)。辞書順最小の巡回シフト開始位置を返す。空列は0。
template <class T>
int minimum_rotation(const std::vector<T>& sequence) {
    return rotation_index(sequence, std::less<T>{});
}

/// O(n)。辞書順最大の巡回シフト開始位置を返す。空列は0。
template <class T>
int maximum_rotation(const std::vector<T>& sequence) {
    return rotation_index(sequence, std::greater<T>{});
}

/// O(n)。startから始まる巡回シフト列を返す。空列ではstart=0。
template <class T>
std::vector<T> rotated(const std::vector<T>& sequence, int start) {
    const int n = static_cast<int>(sequence.size());
    assert((n == 0 && start == 0) || (0 <= start && start < n));
    std::vector<T> result;
    result.reserve(n);
    for (int offset = 0; offset < n; ++offset) {
        result.push_back(sequence[(start + offset) % n]);
    }
    return result;
}

/// O(n)。文字列の辞書順最小巡回シフト開始位置を返す。空文字列は0。
inline int minimum_rotation(const std::string& text) {
    return minimum_rotation(std::vector<char>(text.begin(), text.end()));
}

/// O(n)。文字列の辞書順最大巡回シフト開始位置を返す。空文字列は0。
inline int maximum_rotation(const std::string& text) {
    return maximum_rotation(std::vector<char>(text.begin(), text.end()));
}

/// O(n)。startから始まる文字列の巡回シフトを返す。空文字列ではstart=0。
inline std::string rotated(const std::string& text, int start) {
    const auto result = rotated(std::vector<char>(text.begin(), text.end()), start);
    return std::string(result.begin(), result.end());
}

/// O(n)。sourceを左巡回シフトしてtargetにする最小回数を返す。不可能なら-1。
template<class T>
int rotation_distance(const std::vector<T>& source, const std::vector<T>& target) {
    assert(source.size() == target.size());
    if (source.empty()) return 0;
    std::vector<T> doubled = source;
    doubled.insert(doubled.end(), source.begin(), source.end() - 1);
    const auto positions = find_all(doubled, target);
    return positions.empty() ? -1 : positions.front();
}

/// O(n)。文字列sourceを左巡回シフトしてtargetにする最小回数を返す。不可能なら-1。
inline int rotation_distance(const std::string& source, const std::string& target) {
    return rotation_distance(std::vector<char>(source.begin(), source.end()),
                             std::vector<char>(target.begin(), target.end()));
}

}
