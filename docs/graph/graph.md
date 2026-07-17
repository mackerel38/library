---
title: "`directed_graph` / `undirected_graph`"
documentation_of: //cp/graph/graph.hpp
---

# `directed_graph` / `undirected_graph`

## Include

```cpp
#include "graph/graph.hpp"
```

有向・無向を型で固定し、テンプレート引数を省略すると重みなし、指定すると重み付きになる。
辺追加は償却`O(1)`、`operator[]`・`edge_at`・次数取得は`O(1)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `edge`

```cpp
template<class Cost = void> struct edge
```

重みなし辺を表す: edge<> e{from, to, id}; 各フィールドの取得O(1)。

### `edge`

```cpp
template<> struct edge<void>
```

重みなし辺を表す特殊化。costフィールドを持たない。

### `directed_graph`

```cpp
template<class Cost = void> using directed_graph = detail::graphbase<true, Cost>
```

重み型Costの有向グラフ: directed_graph<long long> graph(n); 重みなしはCostを省略する。

### `undirected_graph`

```cpp
template<class Cost = void> using undirected_graph = detail::graphbase<false, Cost>
```

重み型Costの無向グラフ: undirected_graph<long long> graph(n); 重みなしはCostを省略する。

### `graph_type`

```cpp
template<class Graph> concept graph_type = requires(const Graph& graph, int vertex)
```

directed_graphとundirected_graphを受け取るためのconcept。

### `weighted_graph_type`

```cpp
template<class Graph> concept weighted_graph_type = graph_type<Graph> && Graph::is_weighted
```

重み付きグラフだけを受け取るためのconcept。

### `directed_graph_type`

```cpp
template<class Graph> concept directed_graph_type = graph_type<Graph> && Graph::is_directed
```

有向グラフだけを受け取るためのconcept。

### `undirected_graph_type`

```cpp
template<class Graph> concept undirected_graph_type = graph_type<Graph> && (!Graph::is_directed)
```

無向グラフだけを受け取るためのconcept。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Shortest Path](https://judge.yosupo.jp/problem/shortest_path)。

```cpp
poe::directed_graph<long long> graph(n);
for (auto [from, to, cost] : edges) graph.add_edge(from, to, cost);
auto result = poe::dijkstra(graph, source);
```

`verify/shortestpath.test.cpp`で使用し、2026-07-15に全ケースAC確認済み。
