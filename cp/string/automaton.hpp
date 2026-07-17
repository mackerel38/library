#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 決定性有限automaton。alphabetは0..alphabet_size-1の整数で表す。
struct dfa {
    int start = 0;
    std::vector<std::vector<int>> next;
    std::vector<char> accepting;

    /// O(1)。状態数を返す。
    int size() const noexcept { return static_cast<int>(next.size()); }

    /// O(1)。alphabetの大きさを返す。
    int alphabet_size() const noexcept {
        return next.empty() ? 0 : static_cast<int>(next[0].size());
    }

    /// O(|word|)。各要素をprojectでalphabet番号へ変換し、wordを受理するか返す。
    template<std::ranges::input_range Range, class Project = std::identity>
    bool accepts(const Range& word, Project project = {}) const {
        assert(0 <= start && start < size());
        int state = start;
        for (const auto& raw : word) {
            const int symbol = static_cast<int>(std::invoke(project, raw));
            assert(0 <= symbol && symbol < alphabet_size());
            state = next[state][symbol];
        }
        return accepting[state];
    }
};

/// accepted_substring_statsの結果。受理部分文字列がなければlongest=-1、count=0。
struct acceptedsubstringstats {
    int longest = -1;
    long long count = 0;
};

/// O(nq)。q状態DFAが受理する非空部分文字列の最長長と位置を区別した総数を返す。
template<std::ranges::input_range Range, class Project = std::identity>
acceptedsubstringstats accepted_substring_stats(
    const dfa& automaton,
    const Range& text,
    Project project = {}
) {
    const int states = automaton.size();
    assert(states > 0);
    std::vector<long long> count(states), next_count(states);
    const int infinity = std::numeric_limits<int>::max();
    std::vector<int> earliest(states, infinity), next_earliest(states, infinity);
    acceptedsubstringstats result;
    int position = 0;
    for (const auto& raw : text) {
        const int symbol = static_cast<int>(std::invoke(project, raw));
        assert(0 <= symbol && symbol < automaton.alphabet_size());
        std::ranges::fill(next_count, 0);
        std::ranges::fill(next_earliest, infinity);
        const int one = automaton.next[automaton.start][symbol];
        ++next_count[one];
        next_earliest[one] = position;
        for (int state = 0; state < states; ++state) {
            if (count[state] == 0) continue;
            const int next = automaton.next[state][symbol];
            next_count[next] += count[state];
            next_earliest[next] = std::min(next_earliest[next], earliest[state]);
        }
        count.swap(next_count);
        earliest.swap(next_earliest);
        for (int state = 0; state < states; ++state) {
            if (!automaton.accepting[state]) continue;
            result.count += count[state];
            if (earliest[state] != infinity) {
                result.longest = std::max(result.longest, position - earliest[state] + 1);
            }
        }
        ++position;
    }
    return result;
}

/// membership oracleを長さcontext_length以下の右文脈で分類し、候補DFAを構築する。
template<class Accept>
dfa right_context_dfa(int alphabet_size, int context_length, Accept accepts) {
    assert(alphabet_size >= 1 && context_length >= 0);
    std::vector<std::vector<int>> contexts(1);
    std::vector<std::vector<int>> layer(1);
    for (int length = 1; length <= context_length; ++length) {
        std::vector<std::vector<int>> next_layer;
        for (const auto& prefix : layer) {
            for (int symbol = 0; symbol < alphabet_size; ++symbol) {
                auto word = prefix;
                word.push_back(symbol);
                contexts.push_back(word);
                next_layer.push_back(std::move(word));
            }
        }
        layer.swap(next_layer);
    }

    auto signature = [&](const std::vector<int>& prefix) {
        std::vector<char> result;
        result.reserve(contexts.size());
        for (const auto& suffix : contexts) {
            auto word = prefix;
            word.insert(word.end(), suffix.begin(), suffix.end());
            result.push_back(static_cast<bool>(std::invoke(accepts, word)));
        }
        return result;
    };

    dfa result;
    std::vector<std::vector<int>> representatives;
    std::map<std::vector<char>, int> state_by_signature;
    auto intern = [&](const std::vector<int>& word) {
        const auto key = signature(word);
        if (const auto iterator = state_by_signature.find(key);
            iterator != state_by_signature.end()) return iterator->second;
        const int state = static_cast<int>(representatives.size());
        state_by_signature.emplace(key, state);
        representatives.push_back(word);
        result.next.emplace_back(alphabet_size, -1);
        result.accepting.push_back(static_cast<bool>(std::invoke(accepts, word)));
        return state;
    };
    result.start = intern({});
    for (int state = 0; state < static_cast<int>(representatives.size()); ++state) {
        for (int symbol = 0; symbol < alphabet_size; ++symbol) {
            auto word = representatives[state];
            word.push_back(symbol);
            result.next[state][symbol] = intern(word);
        }
    }
    return result;
}

}
