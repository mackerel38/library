---
title: "lcatree"
documentation_of: //cp/tree/lca.hpp
---

# `lcatree`

## 概要

二分累乗によるLCA・木上jump・パス所属・切断側サイズを扱う。

## 厳密な定義

- `lcatree`: 二分累乗でLCAと木上移動を扱う: lcatree tree(graph, root); 構築O(n log n)、取得O(log n)。


## Include

```cpp
#include "tree/lca.hpp"
```

二分累乗表を`O(n log n)`で構築し、`lca/kth_ancestor/jump/distance/edge_distance/on_path/side_size`を
`O(log n)`で返す。重み付き・重みなしの無向木に対応する。
DFS順の`index`・`operator[]`、部分木右端`out`、根からの距離`root_distance`は`O(1)`で返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lcatree`

```cpp
template<undirected_graph_type Graph> struct lcatree
```

二分累乗でLCAと木上移動を扱う: lcatree tree(graph, root); 構築O(n log n)、取得O(log n)。

### `lcatree`

```cpp
explicit lcatree(const Graph& graph, int root = 0) : tree_(graph, root)
```

O(n log n)。無向木をroot根として前処理する。

### `kth_ancestor`

```cpp
int kth_ancestor(int vertex, int step) const
```

O(log n)。頂点vertexから上へstep辺移動した頂点を返す。存在しなければ-1。

### `lca`

```cpp
int lca(int left, int right) const
```

O(log n)。二頂点の最小共通祖先を返す。

### `jump`

```cpp
int jump(int from, int to, int step) const
```

O(log n)。fromからtoへの単純路をstep辺進んだ頂点を返す。路を越えるなら-1。

### `distance`

```cpp
distance_type distance(int left, int right) const
```

O(log n)。二頂点間の重み和を返す。重みなし辺は1と数える。

### `edge_distance`

```cpp
int edge_distance(int left, int right) const
```

O(log n)。二頂点間の辺数を返す。

### `on_path`

```cpp
bool on_path(int vertex, int from, int to) const
```

O(log n)。vertexがfromからtoへの単純路上にあればtrueを返す。

### `side_size`

```cpp
int side_size(int vertex, int toward) const
```

O(log n)。vertexからtowardへ向かう最初の辺を切ったとき、vertex側の頂点数を返す。
vertex == towardなら木全体の頂点数を返す。

### `parent`

```cpp
int parent(int vertex) const
```

O(1)。頂点vertexの親を返す。根では-1。

### `parent_edge`

```cpp
int parent_edge(int vertex) const
```

O(1)。親へ向かう辺番号を返す。根では-1。

### `depth`

```cpp
int depth(int vertex) const
```

O(1)。根から頂点vertexまでの辺数を返す。

### `subtree_size`

```cpp
int subtree_size(int vertex) const
```

O(1)。頂点vertexの部分木頂点数を返す。

### `is_ancestor`

```cpp
bool is_ancestor(int ancestor, int vertex) const
```

O(1)。ancestorがvertexの祖先ならtrueを返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `root`

```cpp
int root() const noexcept
```

O(1)。根を返す。空木では-1。

### `index`

```cpp
int index(int vertex) const
```

O(1)。頂点vertexのDFS行きがけ順の位置を返す。

### `out`

```cpp
int out(int vertex) const
```

O(1)。頂点vertexの部分木区間の右端を返す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(1)。DFS行きがけ順index番目の頂点を返す。

### `root_distance`

```cpp
distance_type root_distance(int vertex) const
```

O(1)。根からvertexまでの重み和を返す。重みなし辺は1。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Lowest Common Ancestor](https://judge.yosupo.jp/problem/lca)。

```cpp
poe::lcatree tree(graph, 0);
while (query_count--) {
    int left, right;
    std::cin >> left >> right;
    std::cout << tree.lca(left, right) << '\n';
}
```

`verify/lca.test.cpp`で2026-07-15に全ケースAC確認済み。

[AtCoder ABC438 F - Sum of Mex](https://atcoder.jp/contests/abc438/tasks/abc438_f)では、
番号prefixを含む最小パスの端点更新を`on_path`で判定し、両端の外側にある頂点数を`side_size`で数える。
`verify/atcoder_abc438_f.cpp`は公式サンプル一致を確認済み。
