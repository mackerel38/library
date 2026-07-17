---
title: "directed_graph / undirected_graph"
documentation_of: //cp/graph/graph.hpp
---

# `directed_graph` / `undirected_graph`

## 概要

型で有向・無向と重みの有無を表す共通グラフ構造を扱う。

## 厳密な定義

- `edge`: 重みなし辺を表す: edge<> e{from, to, id}; 各フィールドの取得O(1)。
- `matrix_graph_type`: 隣接行列型グラフを受け取るためのconcept。
- `directed_graph`: 重み型Costの有向グラフ: directed_graph<long long> graph(n); 重みなしはCostを省略する。
- `undirected_graph`: 重み型Costの無向グラフ: undirected_graph<long long> graph(n); 重みなしはCostを省略する。
- `graph_type`: directed_graphとundirected_graphを受け取るためのconcept。
- `weighted_graph_type`: 重み付きグラフだけを受け取るためのconcept。
- `directed_graph_type`: 有向グラフだけを受け取るためのconcept。
- `undirected_graph_type`: 無向グラフだけを受け取るためのconcept。


## Include

```cpp
#include "graph/graph.hpp"
```

有向・無向を型で固定し、テンプレート引数を省略すると重みなし、指定すると重み付きになる。
辺追加は償却`O(1)`、`operator[]`・`edge_at`・次数取得は`O(1)`。

## 基本的な使い方

```cpp
int n, m;
cin >> n >> m;

poe::undirected_graph<long long> graph(n);
graph.read(m);  // from to costをm行読む。頂点番号は既定で1-index
```

型と入力一行の形式は次の対応になる。

| 型 | 入力形式 |
| --- | --- |
| `directed_graph<>`, `undirected_graph<>` | `from to` |
| `directed_graph<Cost>`, `undirected_graph<Cost>` | `from to cost` |

入力が0-indexなら`graph.read(m, 0)`とする。`read`は既存の辺を消さず、後ろへ追加する。

## 型

```cpp
template<class Cost = void> using directed_graph = ...;
template<class Cost = void> using undirected_graph = ...;
```

`directed_graph`は有向辺、`undirected_graph`は無向辺を保持する。`Cost`を省略すると
重みなしになり、指定すると各辺が`Cost cost`を持つ。頂点番号は内部では常に
`[0, size())`である。

```cpp
template<class Cost> struct edge {
    int from, to, id;
    Cost cost;
};
```

重みなしの`edge<void>`には`cost`がない。無向グラフの隣接リストでは、同じ辺番号の辺が
両方向に入り、`edges()`と`edge_at(id)`では追加時の向きの一本だけを返す。

## 構築・辺追加・入力

```cpp
directed_graph<> graph(n);
undirected_graph<long long> weighted(n);
```

`n`頂点で辺がないグラフを`O(n)`時間・`O(n)`領域で作る。`n >= 0`が必要。

```cpp
directed_graph<long long> graph(matrix);
```

同じ向き・重み型の`matrix_graph`から`O(n^2+m)`で変換する。無向グラフでは
行列の上三角部分だけを読み、辺を二重に追加しない。

```cpp
int vertex = graph.add_vertex();
int first = graph.add_vertices(count);
```

`add_vertex()`は一頂点を追加してその番号を返す。`add_vertices(count)`は連続する
`count`頂点を追加し、追加区間`[first, first + count)`の`first`を返す。
それぞれ償却`O(1)`、償却`O(count)`。

```cpp
int add_edge(int from, int to);
int add_edge(int from, int to, const Cost& cost);
```

重みなしでは第一形式、重み付きでは第二形式を使う。追加した辺番号を返す。
`0 <= from, to < size()`が必要。償却`O(1)`。

```cpp
void read(int m, int indexed = 1);
```

標準入力`cin`からちょうど`m`辺を読み、`add_edge`と同じ順番で追加する。
`indexed == 1`なら端点から1を引き、`indexed == 0`ならそのまま使う。
重みなしは各行`from to`、重み付きは各行`from to cost`。`m >= 0`が必要で、
入力された端点を変換した後に`[0, size())`へ入らなければならない。`O(m)`時間。

## 取得

```cpp
const vector<edge_type>& operator[](int vertex) const;
const edge_type& edge_at(int id) const;
const vector<edge_type>& edges() const;
int size() const;
int edge_count() const;
int outdegree(int vertex) const;
int indegree(int vertex) const;  // 有向だけ
int degree(int vertex) const;    // 無向だけ
```

すべて`O(1)`。`graph[vertex]`はその頂点から出る辺を返す。無向グラフの自己辺は
隣接リストへ二本入るため、`degree(vertex)`では二回数える。

```cpp
void reserve_edges(int count);
directed_graph<Cost> reversed() const;
```

`reserve_edges`は全辺一覧の容量を予約する。`reversed`は有向グラフの全辺を反転した
新しいグラフを`O(n+m)`で返し、元のグラフは変更しない。

`matrix_graph_type`は隣接行列グラフを受け取るためのconceptである。隣接リストを
走査する`graph_type`とは計算量が異なるため、別のconceptとしている。

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

### `matrix_graph_type`

```cpp
template<class Graph> concept matrix_graph_type = requires(const Graph& graph, int from, int to)
```

隣接行列型グラフを受け取るためのconcept。

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
graph.read(m, 0);
auto result = poe::dijkstra(graph, source);
```

`verify/shortestpath.test.cpp`で使用し、2026-07-15に全ケースAC確認済み。
