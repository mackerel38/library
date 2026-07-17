---
title: "木の直径"
documentation_of: //cp/tree/diameter.hpp
---

# 木の直径

## Include

```cpp
#include "tree/diameter.hpp"
```

非負重みの無向木の直径を`O(n)`で求める。重みなし辺は重み1として扱う。
結果には距離、両端点、頂点列、辺番号列が入る。

`tree_farthest_vertices(graph)`は、全頂点について最遠頂点と距離を`O(n)`で返す。
同距離では既定で頂点番号が大きい方を選ぶ。

```cpp
auto farthest = tree_farthest_vertices(graph);
cout << farthest[v].vertex << ' ' << farthest[v].distance << '\n';
auto smallest = tree_farthest_vertices(graph, std::less<int>{});
```

第二引数の`prefer(a,b)`がtrueなら同距離で`a`を優先する。非負重み付き木にも対応し、
下向き・親側の上位候補を全方位木DPで合成する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `treediameterresult`

```cpp
template<class Distance> struct treediameterresult
```

木の直径結果。pathは端点firstからsecondへの頂点列、edge_idsは辺番号列。

### `farthestvertex`

```cpp
template <class Distance> struct farthestvertex
```

ある頂点からの最遠頂点と距離。

### `tree_diameter`

```cpp
template<undirected_graph_type Graph> auto tree_diameter(const Graph& graph)
```

O(n)。非負重みの無向木の直径を返す。重みなし辺は1と数える。

### `tree_farthest_vertices`

```cpp
template <undirected_graph_type Graph, class Prefer = std::greater<int>> auto tree_farthest_vertices(const Graph& graph, Prefer prefer =
```

O(n)。各頂点からの最遠頂点を返す。同距離ではprefer(a,b)がtrueの頂点aを優先する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Tree Diameter](https://judge.yosupo.jp/problem/tree_diameter)。

```cpp
auto result = poe::tree_diameter(graph);
std::cout << result.distance << ' ' << result.path.size() << '\n';
for (int vertex : result.path) std::cout << vertex << ' ';
```

`verify/treediameter.test.cpp`で2026-07-15に全ケースAC確認済み。

[AtCoder ABC428 E - Farthest Vertex](https://atcoder.jp/contests/abc428/tasks/abc428_e)では、
既定の「同距離なら頂点番号最大」をそのまま使う。
`verify/atcoder_abc428_e.cpp`で公式sample 2件を確認済み。judge ACは未確認。
