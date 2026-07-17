#pragma once
#include <bits/stdc++.h>
#include "flow/dinic.hpp"

namespace poe {

/// 二値劣モジュラ最小化の解。value[i]は変数iの0/1、costは最小値。
template<class Cost>
struct submodularresult {
    Cost cost;
    std::vector<bool> value;
};

/// 二値単項・二項劣モジュラ関数をgraph cutで最小化する: submodular<long long> f(n)。
template<std::signed_integral Cost>
struct submodular {
    /// O(n)。n個の二値変数を持つ目的関数を作る。
    explicit submodular(int n) : size_(n), source_(n), sink_(n + 1), graph_(n + 2) {
        assert(n >= 0);
    }

    /// O(1)。目的関数へ定数costを加える。
    void add_constant(Cost cost) {
        constant_ += cost;
    }

    /// 償却O(1)。変数iが0ならcost0、1ならcost1を目的関数へ加える。
    void add_unary(int i, Cost cost0, Cost cost1) {
        assert_variable(i);
        const Cost base = std::min(cost0, cost1);
        constant_ += base;
        const Cost normalized0 = narrow(static_cast<__int128_t>(cost0) - base);
        const Cost normalized1 = narrow(static_cast<__int128_t>(cost1) - base);
        if (normalized1 > 0) {
            graph_.add_edge(source_, i, normalized1);
        }
        if (normalized0 > 0) {
            graph_.add_edge(i, sink_, normalized0);
        }
    }

    /// 償却O(1)。i=x、j=yのときcostxyを加える。cost00+cost11<=cost01+cost10。
    void add_pair(
        int i,
        int j,
        Cost cost00,
        Cost cost01,
        Cost cost10,
        Cost cost11
    ) {
        assert_variable(i);
        assert_variable(j);
        if (i == j) {
            add_unary(i, cost00, cost11);
            return;
        }
        using wide = __int128_t;
        const wide weight = static_cast<wide>(cost01) + cost10 - cost00 - cost11;
        assert(weight >= 0);
        const Cost a = narrow(static_cast<wide>(cost10) - cost00);
        const Cost b = narrow(static_cast<wide>(cost11) - cost10);
        constant_ += cost00;
        add_linear_one(i, a);
        add_linear_one(j, b);
        if (weight > 0) {
            graph_.add_edge(i, j, narrow(weight));
        }
    }

    /// 償却O(1)。i!=jのときcostを加える。cost>=0。
    void add_neq_cost(int i, int j, Cost cost) {
        assert(cost >= 0);
        add_pair(i, j, 0, cost, cost, 0);
    }

    /// 償却O(1)。i=1かつj=0のときcostを加える。cost>=0。
    void add_implication_cost(int i, int j, Cost cost) {
        assert_variable(i);
        assert_variable(j);
        assert(cost >= 0);
        if (cost > 0) {
            graph_.add_edge(j, i, cost);
        }
    }

    /// 最大流計算1回。目的関数の最小値とそれを達成する0/1割当を返す。
    submodularresult<Cost> minimize() const {
        auto graph = graph_;
        const Cost cut = dinic(graph, source_, sink_);
        const std::vector<bool> source_side = mincut(graph, source_);
        std::vector<bool> value(size_);
        for (int i = 0; i < size_; ++i) {
            value[i] = !source_side[i];
        }
        return submodularresult<Cost>{constant_ + cut, std::move(value)};
    }

    /// O(1)。変数数を返す。
    int size() const noexcept {
        return size_;
    }

private:
    void add_linear_one(int i, Cost cost) {
        if (cost >= 0) {
            if (cost > 0) {
                graph_.add_edge(source_, i, cost);
            }
        } else {
            constant_ += cost;
            graph_.add_edge(i, sink_, narrow(-static_cast<__int128_t>(cost)));
        }
    }

    static Cost narrow(__int128_t value) {
        assert(value >= static_cast<__int128_t>(std::numeric_limits<Cost>::min()));
        assert(value <= static_cast<__int128_t>(std::numeric_limits<Cost>::max()));
        return static_cast<Cost>(value);
    }

    void assert_variable(int i) const {
        assert(0 <= i && i < size_);
    }

    int size_;
    int source_;
    int sink_;
    Cost constant_ = 0;
    flowgraph<Cost> graph_;
};

}
