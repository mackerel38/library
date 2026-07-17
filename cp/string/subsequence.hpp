#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(|text||pattern|)。patternを部分列として含まないtextの空でない部分文字列の個数を返す。
template <class Sequence>
long long count_substrings_without_subsequence(
    const Sequence& text,
    const Sequence& pattern
) {
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());
    if (m == 0) return 0;
    std::vector<int> latest(m + 1, -1);
    long long answer = 0;
    for (int index = 0; index < n; ++index) {
        for (int length = m - 1; length >= 1; --length) {
            if (text[index] == pattern[length] && latest[length] != -1) {
                latest[length + 1] = std::max(latest[length + 1], latest[length]);
            }
        }
        if (text[index] == pattern[0]) latest[1] = index;
        answer += index - latest[m];
    }
    return answer;
}

/// O(n|alphabet|)。kth_distinct_subsequence(text,k)で辞書順k番目(1-indexed)の空でない部分列を返す。
/// 存在しなければnullopt。alphabetは重複なし昇順で、textの全文字を含むこと。
inline std::optional<std::string> kth_distinct_subsequence(
    const std::string& text,
    unsigned long long k,
    const std::string& alphabet = "abcdefghijklmnopqrstuvwxyz"
) {
    assert(k >= 1 && std::ranges::is_sorted(alphabet));
    assert(std::adjacent_find(alphabet.begin(), alphabet.end()) == alphabet.end());
    const int n = static_cast<int>(text.size());
    const int sigma = static_cast<int>(alphabet.size());
    std::array<int, 256> id;
    id.fill(-1);
    for (int c = 0; c < sigma; ++c) id[static_cast<unsigned char>(alphabet[c])] = c;
    std::vector next(n + 1, std::vector<int>(sigma, -1));
    for (int index = n - 1; index >= 0; --index) {
        next[index] = next[index + 1];
        const int character = id[static_cast<unsigned char>(text[index])];
        assert(character != -1);
        next[index][character] = index;
    }
    const unsigned long long cap = k == std::numeric_limits<unsigned long long>::max() ? k : k + 1;
    std::vector<unsigned long long> count(n + 1, 1);
    for (int index = n - 1; index >= 0; --index) {
        unsigned long long total = 1;
        for (int c = 0; c < sigma; ++c) {
            const int position = next[index][c];
            if (position == -1) continue;
            total = std::min(cap, total + std::min(cap - total, count[position + 1]));
        }
        count[index] = total;
    }
    if (count[0] <= k) return std::nullopt;
    std::string answer;
    int index = 0;
    while (k > 0) {
        bool selected = false;
        for (int c = 0; c < sigma; ++c) {
            const int position = next[index][c];
            if (position == -1) continue;
            if (k > count[position + 1]) {
                k -= count[position + 1];
            } else {
                answer.push_back(alphabet[c]);
                index = position + 1;
                --k;
                selected = true;
                break;
            }
        }
        if (!selected) return std::nullopt;
    }
    return answer;
}

}
