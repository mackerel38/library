---
title: "`rootedtree`"
documentation_of: //cp/tree/rootedtree.hpp
---

# `rootedtree`

## Include

```cpp
#include "tree/rootedtree.hpp"
```

無向木を指定した根から`O(n)`で前処理する。親、親辺、深さ、根からの距離、部分木サイズ、
DFS行きがけ順、祖先判定を`O(1)`で取得できる。入力は連結な木であること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rootedtree`

```cpp
template<undirected_graph_type Graph> struct rootedtree
```

無向木をroot根の根付き木として前処理する: rootedtree tree(graph, root); 構築O(n)。

### `rootedtree`

```cpp
explicit rootedtree(const Graph& graph, int root = 0) : root_(root), parent_(graph.size(), -2), parent_edge_(graph.size(), -1), depth_(graph.size()), distance_(graph.size()), in_(graph.size()), out_(graph.size()), subtree_size_(graph.size(), 1)
```

O(n)。無向木をroot根で前処理する。空でないグラフは連結な木であること。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `root`

```cpp
int root() const noexcept
```

O(1)。根を返す。空木では-1を返す。

### `parent`

```cpp
int parent(int vertex) const
```

O(1)。頂点vertexの親を返す。根では-1を返す。

### `parent_edge`

```cpp
int parent_edge(int vertex) const
```

O(1)。親へ向かう辺番号を返す。根では-1を返す。

### `depth`

```cpp
int depth(int vertex) const
```

O(1)。根から頂点vertexまでの辺数を返す。

### `distance`

```cpp
distance_type distance(int vertex) const
```

O(1)。根から頂点vertexまでの重み和を返す。重みなし辺は1と数える。

### `subtree_size`

```cpp
int subtree_size(int vertex) const
```

O(1)。頂点vertexの部分木頂点数を返す。

### `index`

```cpp
int index(int vertex) const
```

O(1)。頂点vertexのDFS行きがけ順の位置を返す。

### `out`

```cpp
int out(int vertex) const
```

O(1)。頂点vertexの部分木を表す半開区間[index(vertex), out(vertex))の右端を返す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(1)。行きがけ順index番目の頂点を返す。

### `order`

```cpp
const std::vector<int>& order() const noexcept
```

O(1)。DFS行きがけ順の全頂点を返す。

### `is_ancestor`

```cpp
bool is_ancestor(int ancestor, int vertex) const
```

O(1)。ancestorがvertexの祖先ならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Lowest Common Ancestor](https://judge.yosupo.jp/problem/lca)の木の親・深さの前処理に使える。

```cpp
poe::rootedtree tree(graph, 0);
for (int vertex = 1; vertex < n; ++vertex) {
    std::cout << tree.parent(vertex) << ' ' << tree.depth(vertex) << '\n';
}
```

`verify/lca.test.cpp`などでは内部から使用し、2026-07-15に関連verifyの全ケースAC確認済み。
直接APIでのjudge ACは未確認。
