---
title: "dinic / mincut"
documentation_of: //cp/flow/dinic.hpp
---

# `dinic` / `mincut`

## Include

```cpp
#include "flow/dinic.hpp"
```

`dinic(graph, source, sink[, limit])`は残余グラフへ最大流を追加して流量を返す。
計算量は一般に`O(n^2m)`。`mincut`は最大流後にsource側の頂点を`O(n+m)`で返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `dinic`

```cpp
template<flowgraph_type Graph> typename Graph::cap_type dinic( Graph& graph, int source, int sink, typename Graph::cap_type limit )
```

O(n^2m)。graph上でsourceからsinkへlimitまで最大流を追加し、その量を返す。

### `dinic`

```cpp
template<flowgraph_type Graph> typename Graph::cap_type dinic(Graph& graph, int source, int sink)
```

O(n^2m)。graph上でsourceからsinkへ流せる最大流を追加し、その量を返す。

### `mincut`

```cpp
template<flowgraph_type Graph> std::vector<bool> mincut(const Graph& graph, int source)
```

O(n+m)。残余グラフでsourceから到達できる頂点を返す。最大流後は最小カット。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ GRL_6_A - Maximum Flow](https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A)。

```cpp
poe::flowgraph<long long> graph(n);
for (auto [from, to, cap] : edges) graph.add_edge(from, to, cap);
std::cout << poe::dinic(graph, 0, n - 1) << '\n';
```

`verify/maxflow.test.cpp`で`maxflow` wrapperを通し、2026-07-15に全ケースAC確認済み。
