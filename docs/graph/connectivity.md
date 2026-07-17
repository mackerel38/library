---
title: "連結成分・二部グラフ"
documentation_of: //cp/graph/connectivity.hpp
---

# 連結成分・二部グラフ

## Include

```cpp
#include "graph/connectivity.hpp"
```

`connected_components`は無向グラフの成分番号・成分列・各成分の辺数を返す。
`result.is_tree(component)`で自己辺・多重辺も考慮した木判定ができる。
`bipartite_coloring`は0/1彩色、失敗時は同色を結ぶ辺番号を返す。いずれも`O(n+m)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `componentresult`

```cpp
struct componentresult
```

連結成分分解の結果: result[vertex]で成分番号を取得する。

### `operator[]`

```cpp
int operator[](int vertex) const
```

O(1)。頂点vertexの成分番号を返す。

### `same`

```cpp
bool same(int left, int right) const
```

O(1)。二頂点が同じ成分に属するか返す。

### `is_tree`

```cpp
bool is_tree(int component) const
```

O(1)。componentが木ならtrueを返す。孤立頂点も木として扱う。

### `connected_components`

```cpp
template<undirected_graph_type Graph> componentresult connected_components(const Graph& graph)
```

O(n+m)。無向グラフを連結成分分解する。成分内と成分間は頂点番号順になる。

### `bipartiteresult`

```cpp
struct bipartiteresult
```

二部グラフ判定の結果。colorは各頂点の0/1彩色、失敗時のconflict_edgeは同色を結ぶ辺番号。

### `is_bipartite`

```cpp
bool is_bipartite() const noexcept
```

O(1)。二部グラフならtrueを返す。

### `operator[]`

```cpp
int operator[](int vertex) const
```

O(1)。頂点vertexの色0/1を返す。

### `bipartite_coloring`

```cpp
template<undirected_graph_type Graph> bipartiteresult bipartite_coloring(const Graph& graph)
```

O(n+m)。無向グラフを二部彩色し、二部でない場合は衝突辺を返す。

### `is_bipartite`

```cpp
template<undirected_graph_type Graph> bool is_bipartite(const Graph& graph)
```

O(n+m)。無向グラフが二部グラフならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ ALDS1_11_D - Connected Components](https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_11_D)。

```cpp
poe::undirected_graph<> graph(n);
for (auto [left, right] : edges) graph.add_edge(left, right);
auto component = poe::connected_components(graph);
for (auto [u, v] : queries) std::cout << (component[u] == component[v]) << '\n';
```

`verify/aoj_alds1_11_d.test.cpp`で2026-07-15にAOJ公開全25ケース通過。

[AtCoder ABC434 E - Distribute Bunnies](https://atcoder.jp/contests/abc434/tasks/abc434_e)では、
各ウサギの二つの移動先を無向辺とみなす。各成分の最大相異移動先数は頂点数と辺数の小さい方なので、
`groups`と`edge_count`から集計できる。`verify/atcoder_abc434_e.cpp`で公式sampleを確認済み。judge ACは未確認。
