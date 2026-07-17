#pragma once
#include <bits/stdc++.h>
#include "structure/dsu.hpp"

namespace poe {

/// 辺の追加・削除がある連結性をofflineで解く: dynamicconnectivity dc(n, q); solveはO((q+k)log q log n)。
struct dynamicconnectivity {
    /// O(q)。n頂点、時刻[0, q)の処理列を作る。
    dynamicconnectivity(int n, int q) : n_(n), q_(q), queries_(q) {
        assert(n >= 0 && q >= 0);
    }

    /// O(log q)。時刻timeから無向辺(a,b)を一つ追加する。
    void add(int time, int a, int b) {
        assert_time(time);
        assert_vertex(a);
        assert_vertex(b);
        normalize(a, b);
        active_[{a, b}].push_back(time);
    }

    /// O(log q)。時刻timeから無向辺(a,b)を一つ削除する。対応するaddが必要。
    void remove(int time, int a, int b) {
        assert_time(time);
        assert_vertex(a);
        assert_vertex(b);
        normalize(a, b);
        auto iterator = active_.find({a, b});
        assert(iterator != active_.end() && !iterator->second.empty());
        const int begin = iterator->second.back();
        iterator->second.pop_back();
        assert(begin <= time);
        intervals_.push_back({begin, time, a, b});
        if (iterator->second.empty()) {
            active_.erase(iterator);
        }
    }

    /// O(1)。時刻timeでaとbが連結かを尋ね、solve結果の添字を返す。
    int same(int time, int a, int b) {
        assert_time(time);
        assert_vertex(a);
        assert_vertex(b);
        return add_query(time, querykind::same, a, b);
    }

    /// O(1)。時刻timeでaの連結成分サイズを尋ね、solve結果の添字を返す。
    int size(int time, int a) {
        assert_time(time);
        assert_vertex(a);
        return add_query(time, querykind::size, a, 0);
    }

    /// O(1)。時刻timeで連結成分数を尋ね、solve結果の添字を返す。
    int count(int time) {
        assert_time(time);
        return add_query(time, querykind::count, 0, 0);
    }

    /// O((q+k)log q log n)。登録した質問への答えを、各メソッドが返した添字順で返す。
    std::vector<int> solve() const {
        if (q_ == 0) {
            return {};
        }
        std::vector<std::vector<edge>> segment(4 * q_);
        for (const interval& item : intervals_) {
            add_interval(segment, 1, 0, q_, item.begin, item.end, {item.a, item.b});
        }
        for (const auto& [vertices, starts] : active_) {
            for (int begin : starts) {
                add_interval(segment, 1, 0, q_, begin, q_,
                             {vertices.first, vertices.second});
            }
        }

        rollbackdsu uf(n_);
        std::vector<int> answers(query_count_);
        solve_node(segment, 1, 0, q_, uf, answers);
        return answers;
    }

private:
    struct edge {
        int a;
        int b;
    };

    struct interval {
        int begin;
        int end;
        int a;
        int b;
    };

    enum class querykind { same, size, count };

    struct query {
        int id;
        querykind kind;
        int a;
        int b;
    };

    /// O(1)。無向辺の端点を昇順にする。
    static void normalize(int& a, int& b) {
        if (a > b) {
            std::swap(a, b);
        }
    }

    /// O(1)。質問を登録して結果添字を返す。
    int add_query(int time, querykind kind, int a, int b) {
        const int id = query_count_++;
        queries_[time].push_back({id, kind, a, b});
        return id;
    }

    /// O(log q)。区間[query_left, query_right)へ辺を登録する。
    static void add_interval(std::vector<std::vector<edge>>& segment, int node,
                             int left, int right, int query_left, int query_right,
                             edge value) {
        if (query_right <= left || right <= query_left || query_left == query_right) {
            return;
        }
        if (query_left <= left && right <= query_right) {
            segment[node].push_back(value);
            return;
        }
        const int middle = (left + right) / 2;
        add_interval(segment, 2 * node, left, middle, query_left, query_right, value);
        add_interval(segment, 2 * node + 1, middle, right, query_left, query_right,
                     value);
    }

    /// 全体O((q+k)log q log n)。時間segment treeをDFSする。
    void solve_node(const std::vector<std::vector<edge>>& segment, int node,
                    int left, int right, rollbackdsu& uf,
                    std::vector<int>& answers) const {
        const int snapshot = uf.snapshot();
        for (const edge& value : segment[node]) {
            uf.merge(value.a, value.b);
        }
        if (right - left == 1) {
            for (const query& item : queries_[left]) {
                if (item.kind == querykind::same) {
                    answers[item.id] = uf.same(item.a, item.b);
                } else if (item.kind == querykind::size) {
                    answers[item.id] = uf.size(item.a);
                } else {
                    answers[item.id] = uf.count();
                }
            }
        } else {
            const int middle = (left + right) / 2;
            solve_node(segment, 2 * node, left, middle, uf, answers);
            solve_node(segment, 2 * node + 1, middle, right, uf, answers);
        }
        uf.rollback(snapshot);
    }

    /// O(1)。頂点番号を確認する。
    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < n_);
    }

    /// O(1)。時刻を確認する。
    void assert_time(int time) const {
        assert(0 <= time && time < q_);
    }

    int n_;
    int q_;
    int query_count_ = 0;
    std::map<std::pair<int, int>, std::vector<int>> active_;
    std::vector<interval> intervals_;
    std::vector<std::vector<query>> queries_;
};

}
