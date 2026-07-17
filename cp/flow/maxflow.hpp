#pragma once
#include <bits/stdc++.h>
#include "flow/dinic.hpp"

namespace poe {

/// ACL互換の最大流: maxflow<long long> graph(n); 新規コードではflowgraphとdinicも使える。
template<class Cap>
struct maxflow : flowgraph<Cap> {
    using flowgraph<Cap>::flowgraph;

    /// O(n^2m)。sourceからsinkへ流せる最大流を追加し、その量を返す。
    Cap flow(int source, int sink) {
        return dinic(*this, source, sink);
    }

    /// O(n^2m)。sourceからsinkへlimitまで最大流を追加し、その量を返す。
    Cap flow(int source, int sink, Cap limit) {
        return dinic(*this, source, sink, limit);
    }

    /// O(n+m)。残余グラフでsourceから到達できる頂点を返す。
    std::vector<bool> min_cut(int source) const {
        return mincut(*this, source);
    }
};

}
