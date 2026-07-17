#pragma once
#include <bits/stdc++.h>

namespace poe {

/// trieの一頂点。next、親、深さ、通過word数、終端word数をO(1)で参照する。
template <class Symbol>
struct trienode {
    std::map<Symbol, int> next;
    int parent = -1;
    int depth = 0;
    int prefix_count = 0;
    int terminal_count = 0;
    std::vector<int> words;
};

/// 任意文字種のprefix木: trie<char> dictionary; dictionary.add(word); 各操作O(length log sigma)。
template <class Symbol = char>
struct trie {
    /// O(1)。根だけを持つ空のtrieを作る。
    trie() : nodes_(1) {}

    /// O(length log sigma)。wordを追加してword番号を返す。空列も追加できる。
    template <std::ranges::input_range Range>
    int add(const Range& word) {
        const int id = word_count();
        int node = 0;
        ++nodes_[node].prefix_count;
        for (const auto& raw_symbol : word) {
            const Symbol symbol = static_cast<Symbol>(raw_symbol);
            auto iterator = nodes_[node].next.find(symbol);
            int child;
            if (iterator == nodes_[node].next.end()) {
                child = static_cast<int>(nodes_.size());
                nodes_[node].next.emplace(symbol, child);
                nodes_.push_back({});
                nodes_.back().parent = node;
                nodes_.back().depth = nodes_[node].depth + 1;
            } else {
                child = iterator->second;
            }
            node = child;
            ++nodes_[node].prefix_count;
        }
        ++nodes_[node].terminal_count;
        nodes_[node].words.push_back(id);
        terminals_.push_back(node);
        return id;
    }

    /// O(length log sigma)。wordに対応する頂点を返す。存在しなければ-1。
    template <std::ranges::input_range Range>
    int find(const Range& word) const {
        int node = 0;
        for (const auto& raw_symbol : word) {
            node = next(node, static_cast<Symbol>(raw_symbol));
            if (node == -1) return -1;
        }
        return node;
    }

    /// O(length log sigma)。wordが追加された回数を返す。
    template <std::ranges::input_range Range>
    int count(const Range& word) const {
        const int node = find(word);
        return node == -1 ? 0 : nodes_[node].terminal_count;
    }

    /// O(length log sigma)。prefixを接頭辞に持つ追加済みword数を返す。
    template <std::ranges::input_range Range>
    int prefix_count(const Range& prefix) const {
        const int node = find(prefix);
        return node == -1 ? 0 : nodes_[node].prefix_count;
    }

    /// O(log sigma)。nodeからsymbolで進む頂点。辺がなければ-1。
    int next(int node, const Symbol& symbol) const {
        assert_node(node);
        const auto iterator = nodes_[node].next.find(symbol);
        return iterator == nodes_[node].next.end() ? -1 : iterator->second;
    }

    /// O(1)。nodeの情報を返す。
    const trienode<Symbol>& operator[](int node) const {
        assert_node(node);
        return nodes_[node];
    }

    /// O(1)。word番号idの終端頂点を返す。
    int terminal(int id) const {
        assert(0 <= id && id < word_count());
        return terminals_[id];
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept { return static_cast<int>(nodes_.size()); }

    /// O(1)。重複を含む追加済みword数を返す。
    int word_count() const noexcept { return static_cast<int>(terminals_.size()); }

private:
    void assert_node(int node) const {
        assert(0 <= node && node < size());
    }

    std::vector<trienode<Symbol>> nodes_;
    std::vector<int> terminals_;
};

/// Aho--Corasickの一一致。patternが半開区間[begin,end)へ現れる。
struct ahomatch {
    int pattern;
    int begin;
    int end;
};

/// 複数pattern automaton: ahocorasick<char> ac('a','z'); add後build; 遷移O(log sigma)。
template <class Symbol = char>
struct ahocorasick {
    /// O(1)。pattern内に現れる文字をalphabetへ自動追加する空automatonを作る。
    ahocorasick() : nodes_(1) {}

    /// O(sigma log sigma)。DP等で使うalphabetを指定する。
    explicit ahocorasick(std::vector<Symbol> alphabet)
        : alphabet_(std::move(alphabet)), nodes_(1) {
        normalize_alphabet();
    }

    /// O(sigma)。整数文字種の[first,last]をalphabetとして指定する。
    ahocorasick(Symbol first, Symbol last) requires std::integral<Symbol> : nodes_(1) {
        assert(first <= last);
        for (long long value = static_cast<long long>(first);
             value <= static_cast<long long>(last); ++value) {
            alphabet_.push_back(static_cast<Symbol>(value));
        }
    }

    /// O(length log sigma)。空でないpatternを追加して番号を返す。追加後はbuild()が必要。
    template <std::ranges::input_range Range>
    int add(const Range& pattern) {
        std::vector<Symbol> symbols;
        for (const auto& raw_symbol : pattern) {
            symbols.push_back(static_cast<Symbol>(raw_symbol));
        }
        assert(!symbols.empty());
        const int id = pattern_count();
        int node = 0;
        for (const Symbol& symbol : symbols) {
            alphabet_.push_back(symbol);
            auto iterator = nodes_[node].next.find(symbol);
            int child;
            if (iterator == nodes_[node].next.end()) {
                child = static_cast<int>(nodes_.size());
                nodes_[node].next.emplace(symbol, child);
                nodes_.push_back({});
            } else {
                child = iterator->second;
            }
            node = child;
        }
        nodes_[node].terminal.push_back(id);
        terminal_.push_back(node);
        length_.push_back(static_cast<int>(symbols.size()));
        built_ = false;
        return id;
    }

    /// O(states sigma log sigma)。failure linkと全alphabet遷移を構築する。
    void build() {
        normalize_alphabet();
        const int states = size();
        failure_.assign(states, 0);
        output_.assign(states, -1);
        go_.assign(states, {});
        order_.clear();
        order_.reserve(states);
        order_.push_back(0);

        std::queue<int> queue;
        for (const Symbol& symbol : alphabet_) {
            const auto iterator = nodes_[0].next.find(symbol);
            const int child = iterator == nodes_[0].next.end() ? 0 : iterator->second;
            go_[0][symbol] = child;
            if (child != 0) {
                failure_[child] = 0;
                output_[child] = -1;
                queue.push(child);
            }
        }
        while (!queue.empty()) {
            const int node = queue.front();
            queue.pop();
            order_.push_back(node);
            for (const Symbol& symbol : alphabet_) {
                const auto iterator = nodes_[node].next.find(symbol);
                if (iterator == nodes_[node].next.end()) {
                    go_[node][symbol] = go_[failure_[node]][symbol];
                    continue;
                }
                const int child = iterator->second;
                failure_[child] = go_[failure_[node]][symbol];
                const int failed = failure_[child];
                output_[child] = !nodes_[failed].terminal.empty() ? failed : output_[failed];
                go_[node][symbol] = child;
                queue.push(child);
            }
        }
        assert(static_cast<int>(order_.size()) == states);
        built_ = true;
    }

    /// O(log sigma)。stateからsymbolを読んだ次状態を返す。build後に使う。
    int next(int state, const Symbol& symbol) const {
        assert_built();
        assert_state(state);
        const auto iterator = go_[state].find(symbol);
        return iterator == go_[state].end() ? 0 : iterator->second;
    }

    /// O(一致pattern数)。stateで終わるpattern番号を返す。
    std::vector<int> patterns(int state) const {
        assert_built();
        assert_state(state);
        std::vector<int> result;
        for (int node = state; node != -1; node = output_[node]) {
            result.insert(result.end(), nodes_[node].terminal.begin(),
                          nodes_[node].terminal.end());
        }
        return result;
    }

    /// O(一致pattern数)。stateで終わるpattern番号をbit maskで返す。pattern数<=UIntのbit数。
    template <std::unsigned_integral UInt = unsigned long long>
    UInt pattern_mask(int state) const {
        UInt result = 0;
        for (int id : patterns(state)) {
            assert(id < std::numeric_limits<UInt>::digits);
            result |= UInt{1} << id;
        }
        return result;
    }

    /// O(text log sigma + occurrences)。全一致を終了位置順で返す。
    template <std::ranges::input_range Range>
    std::vector<ahomatch> match(const Range& text) const {
        assert_built();
        std::vector<ahomatch> result;
        int state = 0;
        int end = 0;
        for (const auto& raw_symbol : text) {
            state = next(state, static_cast<Symbol>(raw_symbol));
            ++end;
            for (int node = state; node != -1; node = output_[node]) {
                for (int id : nodes_[node].terminal) {
                    result.push_back({id, end - length_[id], end});
                }
            }
        }
        return result;
    }

    /// O(text log sigma + occurrences)。patternごとの出現開始位置を昇順で返す。
    template <std::ranges::input_range Range>
    std::vector<std::vector<int>> find_all(const Range& text) const {
        assert_built();
        std::vector<std::vector<int>> result(pattern_count());
        int state = 0;
        int end = 0;
        for (const auto& raw_symbol : text) {
            state = next(state, static_cast<Symbol>(raw_symbol));
            ++end;
            for (int node = state; node != -1; node = output_[node]) {
                for (int id : nodes_[node].terminal) {
                    result[id].push_back(end - length_[id]);
                }
            }
        }
        return result;
    }

    /// O(text log sigma + states + patterns)。各patternの重なりを含む出現回数を返す。
    template <std::ranges::input_range Range>
    std::vector<long long> count(const Range& text) const {
        assert_built();
        std::vector<long long> visits(size());
        int state = 0;
        for (const auto& raw_symbol : text) {
            state = next(state, static_cast<Symbol>(raw_symbol));
            ++visits[state];
        }
        for (int index = size() - 1; index >= 1; --index) {
            const int node = order_[index];
            visits[failure_[node]] += visits[node];
        }
        std::vector<long long> result(pattern_count());
        for (int id = 0; id < pattern_count(); ++id) {
            result[id] = visits[terminal_[id]];
        }
        return result;
    }

    /// O(1)。stateのfailure linkを返す。
    int failure(int state) const {
        assert_built();
        assert_state(state);
        return failure_[state];
    }

    /// O(1)。automatonの状態数を返す。
    int size() const noexcept { return static_cast<int>(nodes_.size()); }

    /// O(1)。追加済みpattern数を返す。
    int pattern_count() const noexcept { return static_cast<int>(length_.size()); }

    /// O(1)。pattern番号idの長さを返す。
    int length(int id) const {
        assert(0 <= id && id < pattern_count());
        return length_[id];
    }

    /// O(1)。使用するalphabetを返す。build後は昇順かつ重複なし。
    const std::vector<Symbol>& alphabet() const noexcept { return alphabet_; }

private:
    struct node {
        std::map<Symbol, int> next;
        std::vector<int> terminal;
    };

    void normalize_alphabet() {
        std::sort(alphabet_.begin(), alphabet_.end());
        alphabet_.erase(std::unique(alphabet_.begin(), alphabet_.end()),
                        alphabet_.end());
    }

    void assert_built() const { assert(built_); }

    void assert_state(int state) const {
        assert(0 <= state && state < size());
    }

    std::vector<Symbol> alphabet_;
    std::vector<node> nodes_;
    std::vector<int> terminal_;
    std::vector<int> length_;
    std::vector<int> failure_;
    std::vector<int> output_;
    std::vector<std::map<Symbol, int>> go_;
    std::vector<int> order_;
    bool built_ = false;
};

}
