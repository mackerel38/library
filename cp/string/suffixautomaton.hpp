#pragma once
#include <bits/stdc++.h>

namespace poe {

/// suffix automatonの一状態。next、suffix link、最長長、出現数、最初の終了位置を持つ。
template <class Symbol>
struct suffixstate {
    std::map<Symbol, int> next;
    int link = -1;
    int length = 0;
    long long occurrence = 0;
    int first_end = -1;
    bool terminal = false;

private:
    long long base_occurrence = 0;
    template <class>
    friend struct suffixautomaton;
};

/// 別列との最長共通部分列結果。substringは連続部分列を表す。
struct suffixmatch {
    int length = 0;
    int end = 0;

    /// O(1)。別列側の一致開始位置を返す。
    int begin() const noexcept { return end - length; }
};

/// 全連続部分列を表す最小DFA: suffixautomaton<char> sam(text); 構築ならしO(n log sigma)。
template <class Symbol = char>
struct suffixautomaton {
    /// O(1)。空列のautomatonを作る。
    suffixautomaton() : states_(1) {}

    /// ならしO(n log sigma)。sequenceを順に追加してbuildする。
    template <std::ranges::input_range Range>
    explicit suffixautomaton(const Range& sequence) : suffixautomaton() {
        add(sequence);
        build();
    }

    /// ならしO(log sigma)。末尾へsymbolを追加し、新しい列全体に対応する状態を返す。
    int add(const Symbol& symbol) {
        built_ = false;
        const int current = static_cast<int>(states_.size());
        states_.push_back({});
        states_[current].length = states_[last_].length + 1;
        states_[current].first_end = static_cast<int>(text_.size());
        states_[current].base_occurrence = 1;
        text_.push_back(symbol);

        int previous = last_;
        while (previous != -1 && !states_[previous].next.contains(symbol)) {
            states_[previous].next[symbol] = current;
            previous = states_[previous].link;
        }
        if (previous == -1) {
            states_[current].link = 0;
        } else {
            const int next = states_[previous].next[symbol];
            if (states_[previous].length + 1 == states_[next].length) {
                states_[current].link = next;
            } else {
                const int clone = static_cast<int>(states_.size());
                states_.push_back(states_[next]);
                states_[clone].length = states_[previous].length + 1;
                states_[clone].base_occurrence = 0;
                states_[clone].occurrence = 0;
                states_[clone].terminal = false;
                while (previous != -1) {
                    auto iterator = states_[previous].next.find(symbol);
                    if (iterator == states_[previous].next.end() ||
                        iterator->second != next) {
                        break;
                    }
                    iterator->second = clone;
                    previous = states_[previous].link;
                }
                states_[next].link = states_[current].link = clone;
            }
        }
        last_ = current;
        return current;
    }

    /// ならしO(length log sigma)。sequenceを末尾へ追加する。
    template <std::ranges::input_range Range>
    void add(const Range& sequence) {
        for (const auto& raw_symbol : sequence) {
            add(static_cast<Symbol>(raw_symbol));
        }
    }

    /// O(states log states)。出現回数とsuffix状態を確定する。追加後にcount等を使う前に呼ぶ。
    void build() {
        order_.resize(size());
        std::iota(order_.begin(), order_.end(), 0);
        std::sort(order_.begin(), order_.end(), [&](int left, int right) {
            return states_[left].length < states_[right].length;
        });
        for (auto& state : states_) {
            state.occurrence = state.base_occurrence;
            state.terminal = false;
        }
        for (int index = size() - 1; index > 0; --index) {
            const int state = order_[index];
            states_[states_[state].link].occurrence += states_[state].occurrence;
        }
        for (int state = last_; state != -1; state = states_[state].link) {
            states_[state].terminal = true;
        }
        built_ = true;
    }

    /// O(length log sigma)。patternが連続部分列として存在するか返す。
    template <std::ranges::input_range Range>
    bool contains(const Range& pattern) const {
        return find_state(pattern) != -1;
    }

    /// O(length log sigma)。patternの重なりを含む出現回数を返す。build後に使う。
    template <std::ranges::input_range Range>
    long long count(const Range& pattern) const {
        assert_built();
        const int state = find_state(pattern);
        if (state == -1) return 0;
        if (state == 0) return static_cast<long long>(text_.size()) + 1;
        return states_[state].occurrence;
    }

    /// O(states)。空でない相異なる連続部分列数を返す。
    unsigned long long distinct_substrings() const {
        unsigned long long result = 0;
        for (int state = 1; state < size(); ++state) {
            result += static_cast<unsigned long long>(
                states_[state].length - states_[states_[state].link].length);
        }
        return result;
    }

    /// O(states + answer length)。0-indexed辞書順k番目の相異なる非空部分列。なければnullopt。
    std::optional<std::vector<Symbol>> kth(unsigned long long k) const {
        std::vector<unsigned long long> ways(size());
        std::vector<int> order(size());
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int left, int right) {
            return states_[left].length > states_[right].length;
        });
        constexpr unsigned long long cap = std::numeric_limits<unsigned long long>::max();
        for (int state : order) {
            for (const auto& [symbol, next] : states_[state].next) {
                static_cast<void>(symbol);
                const unsigned long long add = ways[next] == cap ? cap : ways[next] + 1;
                ways[state] = ways[state] > cap - add ? cap : ways[state] + add;
            }
        }
        if (k >= ways[0]) return std::nullopt;
        std::vector<Symbol> result;
        int state = 0;
        while (true) {
            for (const auto& [symbol, next] : states_[state].next) {
                const unsigned long long group = ways[next] == cap ? cap : ways[next] + 1;
                if (k >= group) {
                    k -= group;
                    continue;
                }
                result.push_back(symbol);
                if (k == 0) return result;
                --k;
                state = next;
                break;
            }
        }
    }

    /// O(other length log sigma)。別列との最長共通連続部分列の長さと別列側終了位置を返す。
    template <std::ranges::input_range Range>
    suffixmatch longest_common_substring(const Range& other) const {
        int state = 0;
        int length = 0;
        int position = 0;
        suffixmatch result;
        for (const auto& raw_symbol : other) {
            const Symbol symbol = static_cast<Symbol>(raw_symbol);
            while (state != 0 && !states_[state].next.contains(symbol)) {
                state = states_[state].link;
                length = states_[state].length;
            }
            const auto iterator = states_[state].next.find(symbol);
            if (iterator == states_[state].next.end()) {
                state = 0;
                length = 0;
            } else {
                state = iterator->second;
                ++length;
            }
            ++position;
            if (length > result.length) result = {length, position};
        }
        return result;
    }

    /// O(states)。二回以上現れる連続部分列の最大長を返す。build後に使う。
    int longest_repeated_substring() const {
        assert_built();
        int result = 0;
        for (const auto& state : states_) {
            if (state.occurrence >= 2) result = std::max(result, state.length);
        }
        return result;
    }

    /// O(1)。状態情報を返す。
    const suffixstate<Symbol>& operator[](int state) const {
        assert_state(state);
        return states_[state];
    }

    /// O(1)。状態数を返す。2n以下。
    int size() const noexcept { return static_cast<int>(states_.size()); }

    /// O(1)。元の列長を返す。
    int length() const noexcept { return static_cast<int>(text_.size()); }

    /// O(1)。元の列全体に対応する状態を返す。
    int last_state() const noexcept { return last_; }

private:
    template <std::ranges::input_range Range>
    int find_state(const Range& pattern) const {
        int state = 0;
        for (const auto& raw_symbol : pattern) {
            const Symbol symbol = static_cast<Symbol>(raw_symbol);
            const auto iterator = states_[state].next.find(symbol);
            if (iterator == states_[state].next.end()) return -1;
            state = iterator->second;
        }
        return state;
    }

    void assert_built() const { assert(built_); }

    void assert_state(int state) const { assert(0 <= state && state < size()); }

    std::vector<suffixstate<Symbol>> states_;
    std::vector<Symbol> text_;
    std::vector<int> order_;
    int last_ = 0;
    bool built_ = false;
};

}
