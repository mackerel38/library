#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 素集合を併合して管理する: dsu uf(n); 併合・判定はならしO(alpha(n))。
struct dsu {
    /// O(n)。n頂点を互いに異なる集合として作る。
    explicit dsu(int n) : component_count_(n), parent_or_size_(n, -1) {
        assert(n >= 0);
    }

    /// ならしO(alpha(n))。aとbを併合し、新しい代表元を返す。
    int merge(int a, int b) {
        assert_index(a);
        assert_index(b);
        int x = leader(a);
        int y = leader(b);
        if (x == y) {
            return x;
        }
        if (-parent_or_size_[x] < -parent_or_size_[y]) {
            std::swap(x, y);
        }
        parent_or_size_[x] += parent_or_size_[y];
        parent_or_size_[y] = x;
        --component_count_;
        return x;
    }

    /// ならしO(alpha(n))。aとbが同じ集合ならtrueを返す。
    bool same(int a, int b) {
        assert_index(a);
        assert_index(b);
        return leader(a) == leader(b);
    }

    /// ならしO(alpha(n))。aが属する集合の代表元を返す。
    int leader(int a) {
        assert_index(a);
        if (parent_or_size_[a] < 0) {
            return a;
        }
        return parent_or_size_[a] = leader(parent_or_size_[a]);
    }

    /// ならしO(alpha(n))。aが属する集合の要素数を返す。
    int size(int a) {
        assert_index(a);
        return -parent_or_size_[leader(a)];
    }

    /// ならしO(alpha(n))。uf[a]はaの代表元を返す。
    int operator[](int a) {
        return leader(a);
    }

    /// O(1)。現在の連結成分数を返す。
    int count() const noexcept {
        return component_count_;
    }

    /// O(n)。全連結成分を頂点番号の昇順で返す。
    std::vector<std::vector<int>> groups() {
        const int n = static_cast<int>(parent_or_size_.size());
        std::vector<int> leaders(n), sizes(n);
        for (int i = 0; i < n; ++i) {
            leaders[i] = leader(i);
            ++sizes[leaders[i]];
        }
        std::vector<std::vector<int>> result(n);
        for (int i = 0; i < n; ++i) {
            result[i].reserve(sizes[i]);
        }
        for (int i = 0; i < n; ++i) {
            result[leaders[i]].push_back(i);
        }
        result.erase(
            std::remove_if(result.begin(), result.end(),
                           [](const auto& group) { return group.empty(); }),
            result.end());
        return result;
    }

private:
    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_index(int a) const {
        assert(0 <= a && a < static_cast<int>(parent_or_size_.size()));
    }

    int component_count_;
    std::vector<int> parent_or_size_;
};

/// XOR差を保つDSU: paritydsu uf(n); uf.merge(a,b,1)で異なる二色を要求する。
struct paritydsu {
    /// O(n)。全頂点のparityを0とした独立成分を作る。
    explicit paritydsu(int n)
        : parent_or_size_(n, -1), xor_to_parent_(n), counts_(n, {1, 0}) {
        assert(n >= 0);
    }

    /// ならしO(alpha(n))。parity(a) xor parity(b)=differenceを追加し、矛盾しなければtrue。
    bool merge(int a, int b, int difference) {
        assert((difference & ~1) == 0);
        const int parity_a = parity(a);
        const int parity_b = parity(b);
        int root_a = leader(a);
        int root_b = leader(b);
        if (root_a == root_b) {
            if ((parity_a ^ parity_b) != difference) consistent_ = false;
            return consistent_;
        }
        int root_difference = parity_a ^ parity_b ^ difference;
        minimum_color_size_ -= std::min(counts_[root_a][0], counts_[root_a][1]);
        minimum_color_size_ -= std::min(counts_[root_b][0], counts_[root_b][1]);
        if (-parent_or_size_[root_a] < -parent_or_size_[root_b]) {
            std::swap(root_a, root_b);
        }
        parent_or_size_[root_a] += parent_or_size_[root_b];
        parent_or_size_[root_b] = root_a;
        xor_to_parent_[root_b] = root_difference;
        for (int color = 0; color < 2; ++color)
            counts_[root_a][color ^ root_difference] += counts_[root_b][color];
        minimum_color_size_ += std::min(counts_[root_a][0], counts_[root_a][1]);
        return consistent_;
    }

    /// ならしO(alpha(n))。aとbが同じ成分ならtrueを返す。
    bool same(int a, int b) { return leader(a) == leader(b); }

    /// ならしO(alpha(n))。aの代表元を返す。
    int leader(int a) {
        assert_index(a);
        if (parent_or_size_[a] < 0) return a;
        const int parent = parent_or_size_[a];
        const int root = leader(parent);
        xor_to_parent_[a] ^= xor_to_parent_[parent];
        return parent_or_size_[a] = root;
    }

    /// ならしO(alpha(n))。代表元に対するaのparityを返す。
    int parity(int a) {
        leader(a);
        return xor_to_parent_[a];
    }

    /// ならしO(alpha(n))。parity(a) xor parity(b)を返す。同じ成分であること。
    int diff(int a, int b) {
        assert(same(a, b));
        return parity(a) ^ parity(b);
    }

    /// ならしO(alpha(n))。aの成分の二色の頂点数を返す。
    std::array<int, 2> color_sizes(int a) {
        return counts_[leader(a)];
    }

    /// O(1)。追加した全制約が矛盾しないならtrueを返す。
    bool consistent() const noexcept { return consistent_; }

    /// O(1)。各成分を反転できるときの色1の最小総数。矛盾時は-1。
    int minimum_color_size() const noexcept {
        return consistent_ ? minimum_color_size_ : -1;
    }

private:
    void assert_index(int a) const {
        assert(0 <= a && a < static_cast<int>(parent_or_size_.size()));
    }
    std::vector<int> parent_or_size_;
    std::vector<int> xor_to_parent_;
    std::vector<std::array<int, 2>> counts_;
    int minimum_color_size_ = 0;
    bool consistent_ = true;
};

/// 頂点間の加法的な差を保つDSU: weighted_dsu<long long> uf(n); 各操作ならしO(alpha(n))。
template <class T>
struct weighted_dsu {
    /// O(n)。各頂点のpotentialを0として、互いに異なる集合を作る。
    explicit weighted_dsu(int n)
        : component_count_(n), parent_or_size_(n, -1), weight_(n) {
        assert(n >= 0);
    }

    /// ならしO(alpha(n))。potential(b)-potential(a)=differenceを追加する。矛盾時だけfalse。
    bool merge(int a, int b, const T& difference) {
        assert_index(a);
        assert_index(b);
        const T weight_a = potential(a);
        const T weight_b = potential(b);
        int root_a = leader(a);
        int root_b = leader(b);
        if (root_a == root_b) {
            return weight_b - weight_a == difference;
        }

        T root_difference = difference + weight_a - weight_b;
        if (-parent_or_size_[root_a] < -parent_or_size_[root_b]) {
            std::swap(root_a, root_b);
            root_difference = T{} - root_difference;
        }
        parent_or_size_[root_a] += parent_or_size_[root_b];
        parent_or_size_[root_b] = root_a;
        weight_[root_b] = root_difference;
        --component_count_;
        return true;
    }

    /// ならしO(alpha(n))。aとbが同じ集合ならtrueを返す。
    bool same(int a, int b) {
        assert_index(a);
        assert_index(b);
        return leader(a) == leader(b);
    }

    /// ならしO(alpha(n))。aの代表元を返す。
    int leader(int a) {
        assert_index(a);
        if (parent_or_size_[a] < 0) {
            return a;
        }
        const int parent = parent_or_size_[a];
        const int root = leader(parent);
        weight_[a] += weight_[parent];
        return parent_or_size_[a] = root;
    }

    /// ならしO(alpha(n))。代表元を基準としたaのpotentialを返す。
    T potential(int a) {
        assert_index(a);
        leader(a);
        return weight_[a];
    }

    /// ならしO(alpha(n))。potential(b)-potential(a)を返す。同じ集合であること。
    T diff(int a, int b) {
        assert(same(a, b));
        return potential(b) - potential(a);
    }

    /// ならしO(alpha(n))。aが属する集合の要素数を返す。
    int size(int a) {
        assert_index(a);
        return -parent_or_size_[leader(a)];
    }

    /// ならしO(alpha(n))。uf[a]でaの代表元を返す。
    int operator[](int a) {
        return leader(a);
    }

    /// O(1)。現在の連結成分数を返す。
    int count() const noexcept {
        return component_count_;
    }

private:
    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_index(int a) const {
        assert(0 <= a && a < static_cast<int>(parent_or_size_.size()));
    }

    int component_count_;
    std::vector<int> parent_or_size_;
    std::vector<T> weight_;
};

/// 併合を巻き戻せるDSU: rollbackdsu uf(n); 併合・判定O(log n)、undo O(1)。
struct rollbackdsu {
    /// O(n)。n頂点を互いに異なる集合として作る。
    explicit rollbackdsu(int n) : component_count_(n), parent_or_size_(n, -1) {
        assert(n >= 0);
    }

    /// O(log n)。aとbを併合する。実際に併合したならtrueを返す。
    bool merge(int a, int b) {
        assert_index(a);
        assert_index(b);
        int root_a = leader(a);
        int root_b = leader(b);
        if (root_a == root_b) {
            history_.push_back({root_a, root_b, 0, false});
            return false;
        }
        if (-parent_or_size_[root_a] < -parent_or_size_[root_b]) {
            std::swap(root_a, root_b);
        }
        history_.push_back({root_a, root_b, parent_or_size_[root_b], true});
        parent_or_size_[root_a] += parent_or_size_[root_b];
        parent_or_size_[root_b] = root_a;
        --component_count_;
        return true;
    }

    /// O(log n)。aとbが同じ集合ならtrueを返す。
    bool same(int a, int b) const {
        assert_index(a);
        assert_index(b);
        return leader(a) == leader(b);
    }

    /// O(log n)。aの代表元を返す。
    int leader(int a) const {
        assert_index(a);
        while (parent_or_size_[a] >= 0) {
            a = parent_or_size_[a];
        }
        return a;
    }

    /// O(log n)。aが属する集合の要素数を返す。
    int size(int a) const {
        assert_index(a);
        return -parent_or_size_[leader(a)];
    }

    /// O(log n)。uf[a]でaの代表元を返す。
    int operator[](int a) const {
        return leader(a);
    }

    /// O(1)。現在の連結成分数を返す。
    int count() const noexcept {
        return component_count_;
    }

    /// O(1)。現在位置をrollback用の値として返す。
    int snapshot() const noexcept {
        return static_cast<int>(history_.size());
    }

    /// O(1)。直前のmerge一回分を取り消す。履歴があること。
    void undo() {
        assert(!history_.empty());
        const change last = history_.back();
        history_.pop_back();
        if (!last.merged) {
            return;
        }
        parent_or_size_[last.root_a] -= last.size_b;
        parent_or_size_[last.root_b] = last.size_b;
        ++component_count_;
    }

    /// 合計O(取り消すmerge数)。snapshot()で得た時点まで戻す。
    void rollback(int snapshot) {
        assert(0 <= snapshot && snapshot <= static_cast<int>(history_.size()));
        while (static_cast<int>(history_.size()) > snapshot) {
            undo();
        }
    }

private:
    struct change {
        int root_a;
        int root_b;
        int size_b;
        bool merged;
    };

    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_index(int a) const {
        assert(0 <= a && a < static_cast<int>(parent_or_size_.size()));
    }

    int component_count_;
    std::vector<int> parent_or_size_;
    std::vector<change> history_;
};

/// 成分の(頂点数,辺数)スコア総和を保って辺追加を巻き戻す: rollbackgraphdsu uf(n, score)。
template<class Score>
struct rollbackgraphdsu {
    using value_type = std::invoke_result_t<Score, int, int>;

    /// O(n)。各孤立頂点のscore(1,0)を初期値として作る。
    rollbackgraphdsu(int n, Score score)
        : component_count_(n), parent_or_size_(n, -1), edge_count_(n), score_(std::move(score)) {
        assert(n >= 0);
        for (int vertex = 0; vertex < n; ++vertex) value_ += score_(1, 0);
    }

    /// O(log n)。無向辺(a,b)を追加し、異なる成分を併合したならtrueを返す。
    bool add_edge(int a, int b) {
        assert_index(a);
        assert_index(b);
        int root_a = leader(a);
        int root_b = leader(b);
        if (root_a == root_b) {
            history_.push_back({
                root_a, root_b, parent_or_size_[root_a], 0,
                edge_count_[root_a], 0, value_, false
            });
            value_ -= score_(-parent_or_size_[root_a], edge_count_[root_a]);
            ++edge_count_[root_a];
            value_ += score_(-parent_or_size_[root_a], edge_count_[root_a]);
            return false;
        }
        if (-parent_or_size_[root_a] < -parent_or_size_[root_b]) {
            std::swap(root_a, root_b);
        }
        history_.push_back({
            root_a, root_b, parent_or_size_[root_a], parent_or_size_[root_b],
            edge_count_[root_a], edge_count_[root_b], value_, true
        });
        value_ -= score_(-parent_or_size_[root_a], edge_count_[root_a]);
        value_ -= score_(-parent_or_size_[root_b], edge_count_[root_b]);
        parent_or_size_[root_a] += parent_or_size_[root_b];
        parent_or_size_[root_b] = root_a;
        edge_count_[root_a] += edge_count_[root_b] + 1;
        value_ += score_(-parent_or_size_[root_a], edge_count_[root_a]);
        --component_count_;
        return true;
    }

    /// O(log n)。aとbが同じ成分ならtrueを返す。
    bool same(int a, int b) const {
        assert_index(a);
        assert_index(b);
        return leader(a) == leader(b);
    }

    /// O(log n)。aの代表元を返す。
    int leader(int a) const {
        assert_index(a);
        while (parent_or_size_[a] >= 0) a = parent_or_size_[a];
        return a;
    }

    /// O(log n)。aの成分の頂点数を返す。
    int size(int a) const {
        return -parent_or_size_[leader(a)];
    }

    /// O(log n)。aの成分の辺数を返す。自己辺と多重辺も一辺ずつ数える。
    int edges(int a) const {
        return edge_count_[leader(a)];
    }

    /// O(log n)。uf[a]でaの代表元を返す。
    int operator[](int a) const {
        return leader(a);
    }

    /// O(1)。現在の連結成分数を返す。
    int count() const noexcept {
        return component_count_;
    }

    /// O(1)。全成分のscore(頂点数,辺数)の総和を返す。
    const value_type& value() const noexcept {
        return value_;
    }

    /// O(1)。現在位置をrollback用の値として返す。
    int snapshot() const noexcept {
        return static_cast<int>(history_.size());
    }

    /// O(1)。直前のadd_edge一回分を取り消す。履歴があること。
    void undo() {
        assert(!history_.empty());
        const change last = history_.back();
        history_.pop_back();
        value_ = last.value;
        parent_or_size_[last.root_a] = last.parent_a;
        edge_count_[last.root_a] = last.edges_a;
        if (last.merged) {
            parent_or_size_[last.root_b] = last.parent_b;
            edge_count_[last.root_b] = last.edges_b;
            ++component_count_;
        }
    }

    /// 合計O(取り消す辺数)。snapshot()で得た時点まで戻す。
    void rollback(int snapshot) {
        assert(0 <= snapshot && snapshot <= static_cast<int>(history_.size()));
        while (static_cast<int>(history_.size()) > snapshot) undo();
    }

private:
    struct change {
        int root_a;
        int root_b;
        int parent_a;
        int parent_b;
        int edges_a;
        int edges_b;
        value_type value;
        bool merged;
    };

    /// O(1)。頂点番号が範囲内であることを確認する。
    void assert_index(int vertex) const {
        assert(0 <= vertex && vertex < static_cast<int>(parent_or_size_.size()));
    }

    int component_count_;
    std::vector<int> parent_or_size_;
    std::vector<int> edge_count_;
    Score score_;
    value_type value_{};
    std::vector<change> history_;
};

}
