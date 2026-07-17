---
title: "flowgraph / costflowgraph"
documentation_of: //cp/flow/flowgraph.hpp
---

# `flowgraph` / `costflowgraph`

## Include

```cpp
#include "flow/flowgraph.hpp"
```

容量だけの残余グラフは`flowgraph<Cap>`、単位費用も持つ残余グラフは
`costflowgraph<Cap, Cost>`を使う。辺追加は償却`O(1)`、辺取得は`O(1)`、全辺取得は`O(m)`。
`operator[]`と`residual`は残余辺列、`edge_at`は元容量と現在流量を返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `flowgraph`

```cpp
template<class Cap> using flowgraph = detail::flowgraphbase<Cap, void>
```

コストなしフロー用残余グラフ: flowgraph<long long> graph(n)。

### `costflowgraph`

```cpp
template<class Cap, class Cost> using costflowgraph = detail::flowgraphbase<Cap, Cost>
```

コスト付きフロー用残余グラフ: costflowgraph<long long, long long> graph(n)。

### `flowgraph_type`

```cpp
template<class Graph> concept flowgraph_type = requires(Graph& graph, int vertex)
```

コストの有無を問わずフロー用残余グラフを受け取るconcept。

### `costflowgraph_type`

```cpp
template<class Graph> concept costflowgraph_type = flowgraph_type<Graph> && Graph::is_costed
```

コスト付きフロー用残余グラフだけを受け取るconcept。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ GRL_6_A - Maximum Flow](https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A)では次の形で使う。

```cpp
poe::flowgraph<long long> graph(vertex_count);
graph.add_edge(from, to, capacity);
long long answer = poe::dinic(graph, 0, vertex_count - 1);
```

`verify/maxflow.test.cpp`では互換wrapper経由で同じ基盤を使用し、2026-07-15に全ケースAC確認済み。
直接APIと`costflowgraph`のjudge ACは未確認。
