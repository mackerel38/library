#pragma once
#include <bits/stdc++.h>

namespace poe {

/// O(n)。sequenceとそのsuffix arrayから隣接suffix間のLCP配列を返す。
template <class T>
std::vector<int> lcp_array(const std::vector<T>& sequence,
                           const std::vector<int>& suffix_array) {
    assert(sequence.size() == suffix_array.size());
    const int n = static_cast<int>(sequence.size());
    if (n == 0) {
        return {};
    }
    std::vector<int> rank(n);
    for (int i = 0; i < n; ++i) {
        assert(0 <= suffix_array[i] && suffix_array[i] < n);
        rank[suffix_array[i]] = i;
    }
    std::vector<int> lcp(n - 1);
    int length = 0;
    for (int i = 0; i < n; ++i) {
        if (length > 0) {
            --length;
        }
        if (rank[i] == 0) {
            continue;
        }
        const int other = suffix_array[rank[i] - 1];
        while (i + length < n && other + length < n &&
               sequence[i + length] == sequence[other + length]) {
            ++length;
        }
        lcp[rank[i] - 1] = length;
    }
    return lcp;
}

/// O(n)。textとそのsuffix arrayから隣接suffix間のLCP配列を返す。
inline std::vector<int> lcp_array(const std::string& text,
                                  const std::vector<int>& suffix_array) {
    return lcp_array(std::vector<char>(text.begin(), text.end()), suffix_array);
}

}
