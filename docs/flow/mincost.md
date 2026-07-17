---
title: "mincost / mincostslope"
documentation_of: //cp/flow/mincost.hpp
---

# `mincost` / `mincostslope`

## Include

```cpp
#include "flow/mincost.hpp"
```

`costflowgraph`へポテンシャル付きDijkstraで最小費用流を流す。
計算量は流量を`F`として`O(F(m+n log n))`。初期辺費用は非負で、同じグラフへの呼び出しは一回とする。

頂点ごとの供給上限と必ず満たす需要が与えられる輸送問題は、超始点・超終点を手で作らず
`mincost_transport`へ渡せる。

```cpp
using edge = poe::costtransportedge<int, long long>;
auto result = poe::mincost_transport<int, long long>(
    supply, demand, vector<edge>{{from, to, capacity, cost}}
);
if (!result.feasible()) cout << -1 << '\n';
else cout << result.cost << '\n';
```

供給は使い切らなくてよいが、需要はすべて満たす必要がある。同じ頂点が供給と需要の両方を持ってもよい。
辺容量・辺費用・供給・需要は非負。負費用辺や下限制約がある場合はこのshortcutの対象外。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `mincostslope`

```cpp
template<costflowgraph_type Graph> std::vector<std::pair<typename Graph::cap_type, typename Graph::cost_type>> mincostslope( Graph& graph, int source, int sink, typename Graph::cap_type limit )
```

O(F(m+n log n))。費用の折れ点をlimitまで返す。各辺の初期費用は非負。

### `mincostslope`

```cpp
template<costflowgraph_type Graph> std::vector<std::pair<typename Graph::cap_type, typename Graph::cost_type>> mincostslope( Graph& graph, int source, int sink )
```

O(F(m+n log n))。費用の折れ点を最大流まで返す。各辺の初期費用は非負。

### `mincost`

```cpp
template<costflowgraph_type Graph> std::pair<typename Graph::cap_type, typename Graph::cost_type> mincost( Graph& graph, int source, int sink, typename Graph::cap_type limit )
```

O(F(m+n log n))。sourceからsinkへlimitまで流した流量と最小費用を返す。

### `mincost`

```cpp
template<costflowgraph_type Graph> std::pair<typename Graph::cap_type, typename Graph::cost_type> mincost( Graph& graph, int source, int sink )
```

O(F(m+n log n))。sourceからsinkへ最大限流した流量と最小費用を返す。

### `costtransportedge`

```cpp
template<class Cap, class Cost> struct costtransportedge
```

mincost_transportへ渡す有向輸送辺。capacityとcostは非負。

### `mincosttransportresult`

```cpp
template<class Cap, class Cost> struct mincosttransportresult
```

mincost_transportの結果。feasible()なら全需要を満たしcostが最小費用。

### `feasible`

```cpp
bool feasible() const noexcept
```

O(1)。全需要を満たせたならtrueを返す。

### `mincost_transport`

```cpp
template<class Cap, class Cost> mincosttransportresult<Cap, Cost> mincost_transport( const std::vector<Cap>& supply, const std::vector<Cap>& demand, const std::vector<costtransportedge<Cap, Cost>>& edges )
```

O(F(m+n log n))。頂点ごとの供給上限から必須需要へ輸送する最小費用を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ GRL_6_B - Minimum Cost Flow](https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B)。

```cpp
poe::costflowgraph<long long, long long> graph(n);
graph.add_edge(from, to, capacity, cost);
auto [flow, minimum_cost] = poe::mincost(graph, source, sink, required_flow);
if (flow != required_flow) minimum_cost = -1;
```

候補問題。

[AtCoder ABC421 G - Increase to make it Increasing](https://atcoder.jp/contests/abc421/tasks/abc421_g)では、
差分列の正部分を供給、負部分を需要、区間加算を右端から左端直前への輸送辺として使う。
`verify/atcoder_abc421_g.cpp`で公式sampleを確認済み。
