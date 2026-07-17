#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n)。prefix[i]=sequence[0..k)とsequence[0..i]の最長一致長kを返す。
template <class T>
std::vector<int> prefix_function(const std::vector<T>& sequence) {
    const int n = static_cast<int>(sequence.size());
    std::vector<int> prefix(n);
    for (int i = 1; i < n; ++i) {
        int length = prefix[i - 1];
        while (length > 0 && sequence[i] != sequence[length]) {
            length = prefix[length - 1];
        }
        if (sequence[i] == sequence[length]) ++length;
        prefix[i] = length;
    }
    return prefix;
}

/// O(n)。文字列textのprefix functionを返す。
inline std::vector<int> prefix_function(const std::string& text) {
    return prefix_function(std::vector<char>(text.begin(), text.end()));
}

/// O(n+m)。text内でpatternが始まる全位置を昇順で返す。空patternは0..nを返す。
template <class T>
std::vector<int> find_all(const std::vector<T>& text, const std::vector<T>& pattern) {
    if (pattern.empty()) {
        std::vector<int> result(text.size() + 1);
        std::iota(result.begin(), result.end(), 0);
        return result;
    }
    const auto prefix = prefix_function(pattern);
    std::vector<int> result;
    int length = 0;
    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        while (length > 0 && text[i] != pattern[length]) {
            length = prefix[length - 1];
        }
        if (text[i] == pattern[length]) ++length;
        if (length == static_cast<int>(pattern.size())) {
            result.push_back(i + 1 - length);
            length = prefix[length - 1];
        }
    }
    return result;
}

/// O(n+m)。text内でpatternが始まる全位置を昇順で返す。
inline std::vector<int> find_all(const std::string& text, const std::string& pattern) {
    return find_all(std::vector<char>(text.begin(), text.end()),
                    std::vector<char>(pattern.begin(), pattern.end()));
}

}
