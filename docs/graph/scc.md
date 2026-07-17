---
title: "強連結成分分解"
documentation_of: //cp/graph/scc.hpp
---

# 強連結成分分解

## 概要

有向グラフの強連結成分分解を扱う。

## 厳密な定義

- `sccgraph`: 有向グラフを強連結成分分解する: sccgraph graph(n); 計算O(n+m)。
- `scc_ids`: O(n+m)。{成分数, 各頂点の成分番号}をトポロジカル順の番号で返す。
- `scc`: O(n+m)。成分内は頂点昇順、成分間はトポロジカル順で返す。


## Include

```cpp
#include "graph/scc.hpp"
```

`sccgraph`または`directed_graph`を`O(n+m)`で強連結成分分解する。
成分番号と成分列はトポロジカル順になる。DFSは反復実装。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sccgraph`

```cpp
struct sccgraph
```

有向グラフを強連結成分分解する: sccgraph graph(n); 計算O(n+m)。

### `sccgraph`

```cpp
sccgraph() = default
```

O(1)。空グラフを作る。

### `sccgraph`

```cpp
explicit sccgraph(int n) : n_(n)
```

O(n)。n頂点のグラフを作る。

### `add_edge`

```cpp
void add_edge(int from, int to)
```

O(1)。有向辺from->toを追加する。多重辺と自己辺も使える。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `scc_ids`

```cpp
std::pair<int, std::vector<int>> scc_ids() const
```

O(n+m)。{成分数, 各頂点の成分番号}をトポロジカル順の番号で返す。

### `scc`

```cpp
std::vector<std::vector<int>> scc() const
```

O(n+m)。成分内は頂点昇順、成分間はトポロジカル順で返す。

### `scc_ids`

```cpp
template<directed_graph_type Graph> std::pair<int, std::vector<int>> scc_ids(const Graph& source)
```

O(n+m)。共通有向グラフの{成分数, 各頂点の成分番号}をトポロジカル順で返す。

### `scc`

```cpp
template<directed_graph_type Graph> std::vector<std::vector<int>> scc(const Graph& graph)
```

O(n+m)。共通有向グラフを強連結成分分解し、成分をトポロジカル順で返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Strongly Connected Components](https://judge.yosupo.jp/problem/scc)。

```cpp
poe::sccgraph graph(n);
for (auto [from, to] : edges) graph.add_edge(from, to);
auto groups = graph.scc();
```

`verify/scc.test.cpp`で2026-07-15に全ケースAC確認済み。
