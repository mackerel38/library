---
title: "最小全域木・最小全域森"
documentation_of: //cp/graph/minimumspanningtree.hpp
---

# 最小全域木・最小全域森

## Include

```cpp
#include "graph/minimumspanningtree.hpp"
```

Kruskal法で`minimum_spanning_forest`または`minimum_spanning_tree`を`O(m log m)`で求める。
結果には重み合計、採用辺番号、連結成分数が入る。後者は非連結入力を許さない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimumspanningforestresult`

```cpp
template<class Cost> struct minimumspanningforestresult
```

最小全域森の結果。costは重み合計、edge_idsは採用した辺番号を表す。

### `connected`

```cpp
bool connected() const noexcept
```

O(1)。空グラフまたは全頂点が連結ならtrueを返す。

### `minimum_spanning_forest`

```cpp
template<weighted_graph_type Graph> requires (!Graph::is_directed) auto minimum_spanning_forest(const Graph& graph)
```

O(m log m)。重み付き無向グラフの最小全域森をKruskal法で求める。

### `minimum_spanning_tree`

```cpp
template<weighted_graph_type Graph> requires (!Graph::is_directed) auto minimum_spanning_tree(const Graph& graph)
```

O(m log m)。連結な重み付き無向グラフの最小全域木を求める。非連結ならassertする。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Minimum Spanning Tree](https://judge.yosupo.jp/problem/minimum_spanning_tree)。

```cpp
auto result = poe::minimum_spanning_tree(graph);
std::cout << result.cost << '\n';
for (int id : result.edge_ids) std::cout << id << ' ';
```

`verify/minimumspanningtree.test.cpp`で2026-07-15に全ケースAC確認済み。
