#pragma once
#include <bits/stdc++.h>

namespace poe {

/// 重み付きダブリングの到着頂点と経路集約値。
template <class T>
struct weighteddoublingresult {
    int vertex;
    T aggregate;
};

/// 関数グラフの二分累乗表: doubling table(next, max_step); 構築O(n log K)。
struct doubling {
    /// O(n log max_step)。next[v]は次の頂点、-1は行き止まり。
    explicit doubling(const std::vector<int>& next, unsigned long long max_step)
        : size_(static_cast<int>(next.size())) {
        int levels = 1;
        while (levels < 64 && (max_step >> levels) != 0) ++levels;
        table_.assign(levels, std::vector<int>(next));
        for (int vertex : next) assert(vertex == -1 || (0 <= vertex && vertex < size_));
        for (int level = 1; level < levels; ++level) {
            for (int vertex = 0; vertex < size_; ++vertex) {
                const int middle = table_[level - 1][vertex];
                table_[level][vertex] = middle == -1 ? -1 : table_[level - 1][middle];
            }
        }
    }

    /// O(log step)。startからstep回進んだ頂点を返す。行き止まりなら-1。
    int jump(int start, unsigned long long step) const {
        assert(0 <= start && start < size_);
        int vertex = start;
        for (int level = 0; step > 0 && vertex != -1; ++level, step >>= 1) {
            assert(level < static_cast<int>(table_.size()));
            if (step & 1) vertex = table_[level][vertex];
        }
        return vertex;
    }

    /// O(log K)。到着頂点がpredicateを満たす範囲で進める最大回数と頂点を返す。
    /// start自身は条件を満たし、遷移に沿ってpredicateがtrueからfalseへ一度だけ変わること。
    template <class Predicate>
    std::pair<unsigned long long, int> max_jump(int start, Predicate predicate) const {
        assert(0 <= start && start < size_ && predicate(start));
        unsigned long long steps = 0;
        int vertex = start;
        for (int level = static_cast<int>(table_.size()) - 1; level >= 0; --level) {
            const int candidate = table_[level][vertex];
            if (candidate != -1 && predicate(candidate)) {
                vertex = candidate;
                steps += 1ULL << level;
            }
        }
        return {steps, vertex};
    }

private:
    int size_;
    std::vector<std::vector<int>> table_;
};

/// 関数グラフ上の遷移値も集約する: weighteddoubling table(next, weight, max_step); 構築O(n log K)。
template <class T, class Operation = std::plus<T>>
struct weighteddoubling {
    /// O(n log max_step)。weight[v]はvからnext[v]へ進む一回分の値。nextは全頂点で定義する。
    explicit weighteddoubling(const std::vector<int>& next, const std::vector<T>& weight,
                              unsigned long long max_step, Operation operation = Operation{},
                              T identity = T{})
        : size_(static_cast<int>(next.size())), operation_(std::move(operation)),
          identity_(std::move(identity)) {
        assert(next.size() == weight.size());
        int levels = 1;
        while (levels < 64 && (max_step >> levels) != 0) ++levels;
        next_.assign(levels, std::vector<int>(next));
        aggregate_.assign(levels, std::vector<T>(weight));
        for (int vertex : next) assert(0 <= vertex && vertex < size_);
        for (int level = 1; level < levels; ++level) {
            for (int vertex = 0; vertex < size_; ++vertex) {
                const int middle = next_[level - 1][vertex];
                next_[level][vertex] = next_[level - 1][middle];
                aggregate_[level][vertex] = std::invoke(
                    operation_, aggregate_[level - 1][vertex], aggregate_[level - 1][middle]);
            }
        }
    }

    /// O(log step)。startからstep回進んだ到着頂点と、通過順に畳み込んだ値を返す。
    weighteddoublingresult<T> jump(int start, unsigned long long step) const {
        assert(0 <= start && start < size_);
        int vertex = start;
        T result = identity_;
        for (int level = 0; step > 0; ++level, step >>= 1) {
            assert(level < static_cast<int>(next_.size()));
            if (!(step & 1)) continue;
            result = std::invoke(operation_, result, aggregate_[level][vertex]);
            vertex = next_[level][vertex];
        }
        return {vertex, std::move(result)};
    }

private:
    int size_;
    Operation operation_;
    T identity_;
    std::vector<std::vector<int>> next_;
    std::vector<std::vector<T>> aggregate_;
};

}
