#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 回文木の一状態。next、suffix link、回文長、出現数、最初の終了位置を持つ。
template <class Symbol>
struct palindromenode {
    std::map<Symbol, int> next;
    int link = 0;
    int length = 0;
    long long occurrence = 0;
    int first_end = 0;
    bool terminal = false;

private:
    long long base_occurrence = 0;
    template <class>
    friend struct palindromictree;
};

/// 最長回文結果。元の列の半開区間[begin,end)と回文木stateを持つ。
struct palindromematch {
    int begin = 0;
    int end = 0;
    int state = 1;

    /// O(1)。回文長を返す。
    int length() const noexcept { return end - begin; }
};

/// 最小回文分割結果。rangesは元の列を覆う半開区間列。
struct palindromepartitionresult {
    int count = 0;
    std::vector<std::pair<int, int>> ranges;

    /// O(1)。index番目の回文区間を返す。
    const std::pair<int, int>& operator[](int index) const {
        assert(0 <= index && index < count);
        return ranges[index];
    }
};

/// 相異なる回文をonline構築する: palindromictree<char> tree(text); 構築ならしO(n log sigma)。
template <class Symbol = char>
struct palindromictree {
    /// O(1)。長さ-1と0の根だけを持つ空の回文木を作る。
    palindromictree() : nodes_(2) {
        nodes_[0].length = -1;
        nodes_[0].link = 0;
        nodes_[1].length = 0;
        nodes_[1].link = 0;
    }

    /// ならしO(n log sigma)。sequenceを順に追加してbuildする。
    template <std::ranges::input_range Range>
    explicit palindromictree(const Range& sequence) : palindromictree() {
        add(sequence);
        build();
    }

    /// ならしO(log sigma)。末尾へsymbolを追加し、新しい列の最長回文suffix状態を返す。
    int add(const Symbol& symbol) {
        built_ = false;
        const int position = static_cast<int>(text_.size());
        text_.push_back(symbol);
        int parent = suffix_candidate(last_, position, symbol);
        const auto iterator = nodes_[parent].next.find(symbol);
        if (iterator != nodes_[parent].next.end()) {
            last_ = iterator->second;
            ++nodes_[last_].base_occurrence;
            suffix_state_.push_back(last_);
            return last_;
        }

        const int current = size();
        nodes_.push_back({});
        nodes_[current].length = nodes_[parent].length + 2;
        nodes_[current].first_end = position + 1;
        nodes_[current].base_occurrence = 1;
        nodes_[parent].next[symbol] = current;
        if (nodes_[current].length == 1) {
            nodes_[current].link = 1;
        } else {
            const int suffix = suffix_candidate(nodes_[parent].link, position, symbol);
            nodes_[current].link = nodes_[suffix].next.at(symbol);
        }
        last_ = current;
        suffix_state_.push_back(last_);
        if (nodes_[current].length > nodes_[longest_].length) longest_ = current;
        return current;
    }

    /// ならしO(length log sigma)。sequenceを末尾へ追加する。
    template <std::ranges::input_range Range>
    void add(const Range& sequence) {
        for (const auto& raw_symbol : sequence) add(static_cast<Symbol>(raw_symbol));
    }

    /// O(states log states)。各回文の出現回数と末尾suffix状態を確定する。
    void build() {
        std::vector<int> order(size() - 2);
        std::iota(order.begin(), order.end(), 2);
        std::sort(order.begin(), order.end(), [&](int left, int right) {
            return nodes_[left].length > nodes_[right].length;
        });
        for (auto& node : nodes_) {
            node.occurrence = node.base_occurrence;
            node.terminal = false;
        }
        for (int state : order) {
            nodes_[nodes_[state].link].occurrence += nodes_[state].occurrence;
        }
        for (int state = last_; state > 1; state = nodes_[state].link) {
            nodes_[state].terminal = true;
        }
        built_ = true;
    }

    /// O(1)。空でない相異なる回文数を返す。
    int distinct_palindromes() const noexcept { return size() - 2; }

    /// O(1)。元の列全体から最長回文の区間と状態を返す。
    palindromematch longest_palindrome() const {
        const int length = nodes_[longest_].length;
        return {nodes_[longest_].first_end - length, nodes_[longest_].first_end,
                longest_};
    }

    /// O(length)。stateが表す回文列を返す。state>=2。
    std::vector<Symbol> palindrome(int state) const {
        assert(2 <= state && state < size());
        const int end = nodes_[state].first_end;
        const int begin = end - nodes_[state].length;
        return std::vector<Symbol>(text_.begin() + begin, text_.begin() + end);
    }

    /// O(1)。positionで終わる最長回文suffixの状態を返す。
    int suffix_state(int position) const {
        assert(0 <= position && position < length());
        return suffix_state_[position];
    }

    /// O(その位置で終わる回文数)。positionで終わる全非空回文状態を長い順で返す。
    std::vector<int> suffixes(int position) const {
        std::vector<int> result;
        for (int state = suffix_state(position); state > 1; state = nodes_[state].link) {
            result.push_back(state);
        }
        return result;
    }

    /// O(全prefixの回文suffix数)、最悪O(n^2)。元の列の最小回文分割と区間を返す。
    palindromepartitionresult minimum_partition() const {
        const int n = length();
        const int infinity = n + 1;
        std::vector<int> dp(n + 1, infinity), previous(n + 1, -1);
        dp[0] = 0;
        for (int end = 1; end <= n; ++end) {
            for (int state = suffix_state_[end - 1]; state > 1;
                 state = nodes_[state].link) {
                const int begin = end - nodes_[state].length;
                if (dp[begin] + 1 < dp[end]) {
                    dp[end] = dp[begin] + 1;
                    previous[end] = begin;
                }
            }
        }
        palindromepartitionresult result;
        result.count = dp[n];
        for (int end = n; end > 0; end = previous[end]) {
            assert(previous[end] != -1);
            result.ranges.emplace_back(previous[end], end);
        }
        std::reverse(result.ranges.begin(), result.ranges.end());
        return result;
    }

    /// O(1)。状態情報を返す。
    const palindromenode<Symbol>& operator[](int state) const {
        assert(0 <= state && state < size());
        return nodes_[state];
    }

    /// O(1)。状態数を返す。根2個を含みn+2以下。
    int size() const noexcept { return static_cast<int>(nodes_.size()); }

    /// O(1)。元の列長を返す。
    int length() const noexcept { return static_cast<int>(text_.size()); }

private:
    int suffix_candidate(int state, int position, const Symbol& symbol) const {
        while (true) {
            const int opposite = position - 1 - nodes_[state].length;
            if (opposite >= 0 && text_[opposite] == symbol) return state;
            state = nodes_[state].link;
        }
    }

    std::vector<palindromenode<Symbol>> nodes_;
    std::vector<Symbol> text_;
    std::vector<int> suffix_state_;
    int last_ = 1;
    int longest_ = 1;
    bool built_ = false;
};

}
