#pragma once
#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"
#include "structure/fenwicktree.hpp"

namespace poe {

/// O(n log n)。multisetの相異なる順列で隣接項next>=current-maximum_dropとなる個数を返す。
template <int modulus>
staticmodint<modulus> count_permutations_max_drop(
    std::vector<long long> values,
    long long maximum_drop
) {
    assert(maximum_drop >= 0);
    assert(values.size() < static_cast<std::size_t>(modulus));
    std::ranges::sort(values);
    std::vector<long long> distinct;
    std::vector<int> frequency;
    for (const long long value : values) {
        if (distinct.empty() || distinct.back() != value) {
            distinct.push_back(value);
            frequency.push_back(0);
        }
        ++frequency.back();
    }

    combination<staticmodint<modulus>> comb(static_cast<int>(values.size()));
    staticmodint<modulus> answer = 1;
    int left = 0;
    int window_count = 0;
    for (int index = 0; index < static_cast<int>(distinct.size()); ++index) {
        while (left < index
               && static_cast<__int128>(distinct[left])
                    < static_cast<__int128>(distinct[index]) - maximum_drop) {
            window_count -= frequency[left++];
        }
        answer *= comb.choose(window_count + frequency[index], frequency[index]);
        window_count += frequency[index];
    }
    return answer;
}

/// O((n+q)log n)。各0-indexed rankについて、1回swap後の列を辞書順に並べたrank番目の位置組を返す。
/// 同じ列を作るswapも別々に数え、同順位の列にはそのいずれかを返す。
template <class T>
std::vector<std::pair<int, int>> one_swap_lexicographic_pairs(
    const std::vector<T>& values,
    const std::vector<long long>& ranks
) {
    const int n = static_cast<int>(values.size());
    assert(n >= 2);
    const long long total = 1LL * n * (n - 1) / 2;
    for (const long long rank : ranks) assert(0 <= rank && rank < total);

    std::vector<T> compressed = values;
    std::ranges::sort(compressed);
    compressed.erase(std::unique(compressed.begin(), compressed.end()), compressed.end());
    fenwicktree<long long> frequency(static_cast<int>(compressed.size()));
    long long smaller_count = 0;
    for (int index = n - 1; index >= 0; --index) {
        const int rank = std::lower_bound(compressed.begin(), compressed.end(), values[index])
                       - compressed.begin();
        smaller_count += frequency.sum(rank);
        frequency.add(rank, 1);
    }
    std::map<T, long long> occurrences;
    for (const T& value : values) ++occurrences[value];
    long long equal_count = 0;
    std::pair<int, int> equal_pair{-1, -1};
    std::map<T, int> first_index;
    for (int index = 0; index < n; ++index) {
        if (const auto iterator = first_index.find(values[index]); iterator != first_index.end()) {
            equal_pair = {iterator->second, index};
        } else {
            first_index.emplace(values[index], index);
        }
    }
    for (const auto& [value, count] : occurrences) equal_count += count * (count - 1) / 2;

    std::vector<std::pair<int, int>> answer(ranks.size(), {-1, -1});
    std::vector<std::pair<long long, int>> smaller_queries, greater_queries;
    for (int query = 0; query < static_cast<int>(ranks.size()); ++query) {
        if (ranks[query] < smaller_count) {
            smaller_queries.emplace_back(ranks[query], query);
        } else if (ranks[query] < smaller_count + equal_count) {
            assert(equal_pair.first != -1);
            answer[query] = equal_pair;
        } else {
            greater_queries.emplace_back(ranks[query] - smaller_count - equal_count, query);
        }
    }
    std::ranges::sort(smaller_queries);
    std::ranges::sort(greater_queries);

    {
        std::vector<std::pair<T, int>> keys(n);
        for (int index = 0; index < n; ++index) keys[index] = {values[index], -index};
        std::ranges::sort(keys);
        std::vector<int> key_index(n);
        fenwicktree<long long> active(n);
        for (int index = 0; index < n; ++index) {
            key_index[index] = std::lower_bound(keys.begin(), keys.end(),
                                                std::pair<T, int>{values[index], -index}) - keys.begin();
            active.add(key_index[index], 1);
        }
        long long passed = 0;
        int query = 0;
        for (int left = 0; left < n && query < static_cast<int>(smaller_queries.size()); ++left) {
            active.add(key_index[left], -1);
            const int bound = std::lower_bound(
                keys.begin(), keys.end(), values[left],
                [](const auto& key, const T& value) { return key.first < value; }
            ) - keys.begin();
            const long long count = active.sum(bound);
            while (query < static_cast<int>(smaller_queries.size())
                   && smaller_queries[query].first < passed + count) {
                const long long offset = smaller_queries[query].first - passed;
                const int position = active.lower_bound(offset + 1);
                answer[smaller_queries[query].second] = {left, -keys[position].second};
                ++query;
            }
            passed += count;
        }
        assert(query == static_cast<int>(smaller_queries.size()));
    }

    {
        std::vector<std::pair<T, int>> keys(n);
        for (int index = 0; index < n; ++index) keys[index] = {values[index], index};
        std::ranges::sort(keys);
        std::vector<int> key_index(n);
        for (int index = 0; index < n; ++index) {
            key_index[index] = std::lower_bound(keys.begin(), keys.end(),
                                                std::pair<T, int>{values[index], index}) - keys.begin();
        }
        fenwicktree<long long> active(n);
        long long passed = 0;
        int query = 0;
        for (int left = n - 1; left >= 0 && query < static_cast<int>(greater_queries.size()); --left) {
            if (left + 1 < n) active.add(key_index[left + 1], 1);
            const int bound = std::upper_bound(
                keys.begin(), keys.end(), values[left],
                [](const T& value, const auto& key) { return value < key.first; }
            ) - keys.begin();
            const long long before = active.sum(bound);
            const long long count = active.sum() - before;
            while (query < static_cast<int>(greater_queries.size())
                   && greater_queries[query].first < passed + count) {
                const long long offset = greater_queries[query].first - passed;
                const int position = active.lower_bound(before + offset + 1);
                answer[greater_queries[query].second] = {left, keys[position].second};
                ++query;
            }
            passed += count;
        }
        assert(query == static_cast<int>(greater_queries.size()));
    }

    for (const auto [left, right] : answer) assert(0 <= left && left < right && right < n);
    return answer;
}

/// path_order_positionsの結果。ranges[value]は可能な0-indexed位置の閉区間、counts[position]は候補数。
struct pathorderpositions {
    std::vector<std::pair<int, int>> ranges;
    std::vector<int> counts;
};

/// O(n)。隣接value間だけに前後制約がある順列で、各valueの可能位置区間と各位置の候補数を返す。
template<std::ranges::input_range Range, class Forward>
pathorderpositions path_order_positions(const Range& relations, Forward is_forward) {
    std::vector<char> forward;
    for (const auto& relation : relations) {
        forward.push_back(static_cast<bool>(std::invoke(is_forward, relation)));
    }
    const int size = static_cast<int>(forward.size()) + 1;
    std::vector<int> left_forward(size), left_backward(size);
    for (int value = 1; value < size; ++value) {
        if (forward[value - 1]) left_forward[value] = left_forward[value - 1] + 1;
        else left_backward[value] = left_backward[value - 1] + 1;
    }
    std::vector<int> right_forward(size), right_backward(size);
    for (int value = size - 2; value >= 0; --value) {
        if (forward[value]) right_forward[value] = right_forward[value + 1] + 1;
        else right_backward[value] = right_backward[value + 1] + 1;
    }

    pathorderpositions result;
    result.ranges.resize(size);
    std::vector<int> difference(size + 1);
    for (int value = 0; value < size; ++value) {
        const int left = left_forward[value] + right_backward[value];
        const int right = size - 1 - left_backward[value] - right_forward[value];
        assert(left <= right);
        result.ranges[value] = {left, right};
        ++difference[left];
        --difference[right + 1];
    }
    result.counts.resize(size);
    std::partial_sum(difference.begin(), difference.begin() + size, result.counts.begin());
    return result;
}

}
