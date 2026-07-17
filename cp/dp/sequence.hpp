#pragma once
#include <bits/stdc++.h>

namespace poe {

/// lis_result.indicesに狭義LISの元配列上の添字を保持する。
struct lis_result {
    std::vector<int> indices;

    /// O(1)。result.size(): LISの長さを返す。
    int size() const noexcept { return static_cast<int>(indices.size()); }
};

/// O(n log n)。lis(values): 狭義最長増加部分列を一つ復元する。
template <class T>
lis_result lis(const std::vector<T>& values) {
    std::vector<T> tails;
    std::vector<int> tail_indices;
    std::vector<int> parent(values.size(), -1);
    for (int index = 0; index < static_cast<int>(values.size()); ++index) {
        const int length = static_cast<int>(
            std::lower_bound(tails.begin(), tails.end(), values[index]) - tails.begin()
        );
        if (length > 0) parent[index] = tail_indices[length - 1];
        if (length == static_cast<int>(tails.size())) {
            tails.push_back(values[index]);
            tail_indices.push_back(index);
        } else {
            tails[length] = values[index];
            tail_indices[length] = index;
        }
    }
    lis_result result;
    if (tail_indices.empty()) return result;
    for (int index = tail_indices.back(); index != -1; index = parent[index]) {
        result.indices.push_back(index);
    }
    std::reverse(result.indices.begin(), result.indices.end());
    return result;
}

/// O(n log n)。lnds(values): 広義最長増加部分列を一つ復元する。
template <class T>
lis_result lnds(const std::vector<T>& values) {
    std::vector<T> tails;
    std::vector<int> tail_indices;
    std::vector<int> parent(values.size(), -1);
    for (int index = 0; index < static_cast<int>(values.size()); ++index) {
        const int length = static_cast<int>(
            std::upper_bound(tails.begin(), tails.end(), values[index]) - tails.begin()
        );
        if (length > 0) parent[index] = tail_indices[length - 1];
        if (length == static_cast<int>(tails.size())) {
            tails.push_back(values[index]);
            tail_indices.push_back(index);
        } else {
            tails[length] = values[index];
            tail_indices[length] = index;
        }
    }
    lis_result result;
    if (tail_indices.empty()) return result;
    for (int index = tail_indices.back(); index != -1; index = parent[index]) {
        result.indices.push_back(index);
    }
    std::reverse(result.indices.begin(), result.indices.end());
    return result;
}

/// O(n log n)。longest_nested_intervals(intervals): 区間を外側から内側へ狭義包含する最長列を復元する。
template <class T>
lis_result longest_nested_intervals(const std::vector<std::pair<T, T>>& intervals) {
    const int n = static_cast<int>(intervals.size());
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    for (const auto& [left, right] : intervals) assert(left < right);
    std::sort(order.begin(), order.end(), [&](int first, int second) {
        if (intervals[first].second != intervals[second].second) {
            return intervals[first].second > intervals[second].second;
        }
        return intervals[first].first > intervals[second].first;
    });
    std::vector<T> lefts(n);
    for (int i = 0; i < n; ++i) lefts[i] = intervals[order[i]].first;
    auto result = lis(lefts);
    for (int& index : result.indices) index = order[index];
    return result;
}

/// O(nm)時間・O(nm)領域。lcs(a, b): 最長共通部分列を一つ返す。
template <class Sequence>
Sequence lcs(const Sequence& left, const Sequence& right) {
    const int n = static_cast<int>(left.size());
    const int m = static_cast<int>(right.size());
    std::vector dp(n + 1, std::vector<int>(m + 1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (left[i] == right[j]) dp[i + 1][j + 1] = dp[i][j] + 1;
            else dp[i + 1][j + 1] = std::max(dp[i][j + 1], dp[i + 1][j]);
        }
    }
    Sequence result;
    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (left[i - 1] == right[j - 1]) {
            result.push_back(left[i - 1]);
            --i;
            --j;
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

/// O(n(m+k))時間・O(nk)領域。自己重なりしないpatternの出現数をちょうどk増やす最小置換数。
/// minimum_unbordered_pattern_increase(text, pattern, k): 不可能なら-1。patternは空でなくborderを持たないこと。
template <class Sequence>
int minimum_unbordered_pattern_increase(
    const Sequence& text,
    const Sequence& pattern,
    int increase
) {
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());
    assert(m > 0 && increase >= 0);

    std::vector<int> prefix(m);
    for (int i = 1; i < m; ++i) {
        int length = prefix[i - 1];
        while (length > 0 && pattern[i] != pattern[length]) length = prefix[length - 1];
        if (pattern[i] == pattern[length]) ++length;
        prefix[i] = length;
    }
    assert(prefix.back() == 0);
    if (increase == 0) return 0;

    std::vector<int> occurrence(n + 1);
    std::vector<int> mismatch(n + 1);
    for (int end = m; end <= n; ++end) {
        for (int offset = 0; offset < m; ++offset) {
            mismatch[end] += text[end - m + offset] != pattern[offset];
        }
        occurrence[end] = mismatch[end] == 0;
    }
    std::vector<int> occurrence_prefix(n + 1);
    for (int i = 1; i <= n; ++i) {
        occurrence_prefix[i] = occurrence_prefix[i - 1] + occurrence[i];
    }

    constexpr int inf = std::numeric_limits<int>::max() / 4;
    std::vector dp(n + 1, std::vector<int>(increase + 1, inf));
    for (int i = 0; i <= n; ++i) dp[i][0] = 0;
    for (int i = 1; i <= n; ++i) {
        for (int gained = 1; gained <= increase; ++gained) {
            if (gained + occurrence[i] <= increase) {
                dp[i][gained] = dp[i - 1][gained + occurrence[i]];
            }
            if (i >= m) {
                const int erased = occurrence_prefix[i] - occurrence_prefix[i - m];
                assert(erased <= 1);
                const int before = gained - 1 + erased;
                if (0 <= before && before <= increase && dp[i - m][before] < inf) {
                    dp[i][gained] = std::min(
                        dp[i][gained], dp[i - m][before] + mismatch[i]
                    );
                }
            }
        }
    }
    return dp[n][increase] == inf ? -1 : dp[n][increase];
}

}
