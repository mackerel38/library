#pragma once
#include <bits/stdc++.h>
#include "flow/dinic.hpp"

namespace poe {

/// 下限制約付きs-tフローの解。flowは流量値、edge_flowは辺追加順の流量。
template<class Cap>
struct lowerboundflowresult {
    Cap flow;
    std::vector<Cap> edge_flow;
};

/// 各辺lower<=flow<=upperのフローを解く: lowerboundflow<long long> problem(n)。
template<class Cap>
struct lowerboundflow {
    /// O(n)。n頂点の下限制約付きフロー問題を作る。
    explicit lowerboundflow(int n) : size_(n) {
        assert(n >= 0);
    }

    /// 償却O(1)。lower<=flow<=upperの辺を追加し、辺番号を返す。
    int add_edge(int from, int to, Cap lower, Cap upper) {
        assert_vertex(from);
        assert_vertex(to);
        assert(0 <= lower && lower <= upper);
        const int id = edge_count();
        edges_.push_back(edge{from, to, lower, upper});
        return id;
    }

    /// 最大流計算1回。流量保存を満たす循環があれば辺流量を返す。
    std::optional<std::vector<Cap>> circulation() const {
        auto built = build(false, 0, 0);
        if (!satisfy_demands(built)) {
            return std::nullopt;
        }
        return collect_flows(built.graph, built.original_ids);
    }

    /// 最大流計算2回。下限制約を満たすsourceからsinkへの最大流を返す。
    std::optional<lowerboundflowresult<Cap>> maxflow(int source, int sink) const {
        return solve(source, sink, true);
    }

    /// 最大流計算2回。非負のs-t流のうち下限制約を満たす最小流を返す。
    std::optional<lowerboundflowresult<Cap>> minflow(int source, int sink) const {
        return solve(source, sink, false);
    }

    /// 最大流計算1回。流量保存を満たす循環が存在するか返す。
    bool feasible() const {
        return circulation().has_value();
    }

    /// O(1)。頂点数を返す。
    int size() const noexcept {
        return size_;
    }

    /// O(1)。追加した辺数を返す。
    int edge_count() const noexcept {
        return static_cast<int>(edges_.size());
    }

private:
    struct edge {
        int from;
        int to;
        Cap lower;
        Cap upper;
    };

    struct built_graph {
        flowgraph<Cap> graph;
        std::vector<int> original_ids;
        std::vector<int> auxiliary_ids;
        int super_source;
        int super_sink;
        Cap demand;
        int back_edge = -1;
    };

    built_graph build(bool with_terminals, int source, int sink) const {
        const int super_source = size_;
        const int super_sink = size_ + 1;
        built_graph result{
            flowgraph<Cap>(size_ + 2), {}, {}, super_source, super_sink, 0, -1
        };
        result.original_ids.reserve(edges_.size());
        std::vector<Cap> balance(size_);
        for (const edge& current : edges_) {
            result.original_ids.push_back(
                result.graph.add_edge(current.from, current.to, current.upper - current.lower)
            );
            balance[current.from] -= current.lower;
            balance[current.to] += current.lower;
        }
        if (with_terminals) {
            result.back_edge = result.graph.add_edge(
                sink, source, std::numeric_limits<Cap>::max()
            );
        }
        for (int vertex = 0; vertex < size_; ++vertex) {
            if (balance[vertex] > 0) {
                result.auxiliary_ids.push_back(
                    result.graph.add_edge(super_source, vertex, balance[vertex])
                );
                result.demand += balance[vertex];
            } else if (balance[vertex] < 0) {
                result.auxiliary_ids.push_back(
                    result.graph.add_edge(vertex, super_sink, -balance[vertex])
                );
            }
        }
        return result;
    }

    bool satisfy_demands(built_graph& built) const {
        return dinic(built.graph, built.super_source, built.super_sink) == built.demand;
    }

    void remove_auxiliary_edges(built_graph& built) const {
        for (int id : built.auxiliary_ids) {
            built.graph.change_edge(id, 0, 0);
        }
    }

    std::optional<lowerboundflowresult<Cap>> solve(
        int source,
        int sink,
        bool maximize
    ) const {
        assert_vertex(source);
        assert_vertex(sink);
        assert(source != sink);
        auto built = build(true, source, sink);
        if (!satisfy_demands(built)) {
            return std::nullopt;
        }
        const Cap base = built.graph.edge_at(built.back_edge).flow;
        remove_auxiliary_edges(built);
        built.graph.change_edge(built.back_edge, 0, 0);
        Cap value = base;
        if (maximize) {
            value += dinic(built.graph, source, sink);
        } else {
            value -= dinic(built.graph, sink, source, base);
        }
        return lowerboundflowresult<Cap>{
            value, collect_flows(built.graph, built.original_ids)
        };
    }

    std::vector<Cap> collect_flows(
        const flowgraph<Cap>& graph,
        const std::vector<int>& ids
    ) const {
        std::vector<Cap> result;
        result.reserve(edges_.size());
        for (int id = 0; id < edge_count(); ++id) {
            result.push_back(edges_[id].lower + graph.edge_at(ids[id]).flow);
        }
        return result;
    }

    void assert_vertex(int vertex) const {
        assert(0 <= vertex && vertex < size_);
    }

    int size_;
    std::vector<edge> edges_;
};

}
