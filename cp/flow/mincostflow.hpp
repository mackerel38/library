#pragma once
#include <bits/stdc++.h>
#include "flow/mincost.hpp"

namespace poe {

/// ACL互換の最小費用流。新規コードではcostflowgraphとmincostも使える。
template<class Cap, class Cost>
struct mincostflow : costflowgraph<Cap, Cost> {
    using costflowgraph<Cap, Cost>::costflowgraph;

    /// O(F(m+n log n))。sourceからsinkへ最大限流した流量と最小費用を返す。
    std::pair<Cap, Cost> flow(int source, int sink) {
        return mincost(*this, source, sink);
    }

    /// O(F(m+n log n))。sourceからsinkへlimitまで流した流量と最小費用を返す。
    std::pair<Cap, Cost> flow(int source, int sink, Cap limit) {
        return mincost(*this, source, sink, limit);
    }

    /// O(F(m+n log n))。費用の折れ点を最大流まで返す。
    std::vector<std::pair<Cap, Cost>> slope(int source, int sink) {
        return mincostslope(*this, source, sink);
    }

    /// O(F(m+n log n))。費用の折れ点をlimitまで返す。
    std::vector<std::pair<Cap, Cost>> slope(int source, int sink, Cap limit) {
        return mincostslope(*this, source, sink, limit);
    }
};

}
