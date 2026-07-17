#pragma once
#include <bits/stdc++.h>
#include "graph/scc.hpp"

namespace poe {

/// 2-SATを解く: twosat sat(n); sat.add_clause(i, f, j, g); 計算O(n+m)。
struct twosat {
    /// O(n)。n個の真偽変数を作る。
    explicit twosat(int n) : n_(n), graph_(2 * n), answer_(n) {
        assert(n >= 0);
    }

    /// O(1)。(x_i=f)または(x_j=g)という節を追加する。
    void add_clause(int i, bool f, int j, bool g) {
        assert_variable(i);
        assert_variable(j);
        graph_.add_edge(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));
        graph_.add_edge(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));
    }

    /// O(1)。x_i=valueを必須にする。
    void force(int i, bool value) {
        add_clause(i, value, i, value);
    }

    /// O(1)。(x_i=from_value)ならば(x_j=to_value)という含意を追加する。
    void imply(int i, bool from_value, int j, bool to_value) {
        add_clause(i, !from_value, j, to_value);
    }

    /// O(1)。リテラル(x_i=f)と(x_j=g)のちょうど一方を真にする。
    void add_xor(int i, bool f, int j, bool g) {
        add_clause(i, f, j, g);
        add_clause(i, !f, j, !g);
    }

    /// O(1)。リテラル(x_i=f)と(x_j=g)の真偽を一致させる。
    void add_equal(int i, bool f, int j, bool g) {
        imply(i, f, j, g);
        imply(j, g, i, f);
        imply(i, !f, j, !g);
        imply(j, !g, i, !f);
    }

    /// O(k^2)。指定したリテラルのうち高々一つだけを真にする。
    void at_most_one(const std::vector<std::pair<int, bool>>& literals) {
        for (int left = 0; left < static_cast<int>(literals.size()); ++left) {
            assert_variable(literals[left].first);
            for (int right = left + 1; right < static_cast<int>(literals.size()); ++right) {
                assert_variable(literals[right].first);
                add_clause(literals[left].first, !literals[left].second,
                           literals[right].first, !literals[right].second);
            }
        }
    }

    /// O(n+m)。充足可能ならtrueを返し、answer()を構築する。
    bool satisfiable() {
        has_answer_ = false;
        const auto [group_count, ids] = graph_.scc_ids();
        static_cast<void>(group_count);
        for (int variable = 0; variable < n_; ++variable) {
            if (ids[2 * variable] == ids[2 * variable + 1]) {
                return false;
            }
            answer_[variable] = ids[2 * variable] < ids[2 * variable + 1];
        }
        has_answer_ = true;
        return true;
    }

    /// O(1)。直前のsatisfiable()で得た割当を返す。
    const std::vector<bool>& answer() const {
        assert(has_answer_);
        return answer_;
    }

    /// O(1)。sat[index]は直前に得た変数indexの割当を返す。
    bool operator[](int index) const {
        assert_variable(index);
        assert(has_answer_);
        return answer_[index];
    }

    /// O(1)。変数数を返す。
    int size() const noexcept {
        return n_;
    }

private:
    /// O(1)。変数番号が範囲内であることを確認する。
    void assert_variable(int variable) const {
        assert(0 <= variable && variable < n_);
    }

    int n_;
    sccgraph graph_;
    std::vector<bool> answer_;
    bool has_answer_ = false;
};

}
