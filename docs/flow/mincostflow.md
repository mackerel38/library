---
title: "mincostflow"
documentation_of: //cp/flow/mincostflow.hpp
---

# `mincostflow`

## Include

```cpp
#include "flow/mincostflow.hpp"
```

`costflowgraph`と`mincost`を包むACL互換wrapper。
`flow`は`{流量, 最小費用}`、`slope`は費用曲線の折れ点を返す。
初期辺費用は非負で、`flow/slope`の呼び出しは合計一回とする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `mincostflow`

```cpp
template<class Cap, class Cost> struct mincostflow : costflowgraph<Cap, Cost>
```

ACL互換の最小費用流。新規コードではcostflowgraphとmincostも使える。

### `flow`

```cpp
std::pair<Cap, Cost> flow(int source, int sink)
```

O(F(m+n log n))。sourceからsinkへ最大限流した流量と最小費用を返す。

### `flow`

```cpp
std::pair<Cap, Cost> flow(int source, int sink, Cap limit)
```

O(F(m+n log n))。sourceからsinkへlimitまで流した流量と最小費用を返す。

### `slope`

```cpp
std::vector<std::pair<Cap, Cost>> slope(int source, int sink)
```

O(F(m+n log n))。費用の折れ点を最大流まで返す。

### `slope`

```cpp
std::vector<std::pair<Cap, Cost>> slope(int source, int sink, Cap limit)
```

O(F(m+n log n))。費用の折れ点をlimitまで返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ GRL_6_B - Minimum Cost Flow](https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_B)。

```cpp
poe::mincostflow<long long, long long> graph(n);
graph.add_edge(from, to, capacity, cost);
auto [flow, answer] = graph.flow(source, sink, required_flow);
std::cout << (flow == required_flow ? answer : -1) << '\n';
```

候補問題。
