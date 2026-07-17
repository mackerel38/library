---
title: "Heavy-Light Decomposition"
documentation_of: //cp/tree/heavylightdecomposition.hpp
---

# Heavy-Light Decomposition

## 概要

Heavy-Light Decompositionによるパス・部分木区間化を扱う。

## 厳密な定義

- `pathsegment`: HLD上の半開区間。reversedならright-1からleftへ向かう順で使う。
- `hld`: 木のパスと部分木を列上の区間へ分解する: hld tree(graph, root); 構築O(n)。


## Include

```cpp
#include "tree/heavylightdecomposition.hpp"
```

`hld`は木を`O(n)`で前処理し、頂点パス・辺パスを`O(log n)`個の半開区間へ分解する。
`subtree`は部分木区間、`edge_index`は辺を子頂点の位置へ変換する。非可換演算では`reversed`に従う。
`parent`、`depth`、`head`、`root`で根付き木とheavy pathの情報も取得できる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `pathsegment`

```cpp
struct pathsegment
```

HLD上の半開区間。reversedならright-1からleftへ向かう順で使う。

### `hld`

```cpp
template<undirected_graph_type Graph> struct hld
```

木のパスと部分木を列上の区間へ分解する: hld tree(graph, root); 構築O(n)。

### `hld`

```cpp
explicit hld(const Graph& graph, int root = 0) : tree_(graph, root), head_(graph.size()), index_(graph.size()), vertex_(graph.size()), heavy_(graph.size(), -1), edge_index_(graph.edge_count(), -1)
```

O(n)。無向木をroot根としてHeavy-Light Decompositionする。

### `lca`

```cpp
int lca(int left, int right) const
```

O(log n)。二頂点の最小共通祖先を返す。

### `path`

```cpp
std::vector<pathsegment> path(int from, int to) const
```

O(log n)。fromからtoへの頂点列を順序付き半開区間へ分解する。

### `edge_path`

```cpp
std::vector<pathsegment> edge_path(int from, int to) const
```

O(log n)。fromからtoへの辺列を、子頂点の位置を使う順序付き半開区間へ分解する。

### `subtree`

```cpp
std::pair<int, int> subtree(int vertex) const
```

O(1)。頂点vertexの部分木を表す半開区間を返す。

### `index`

```cpp
int index(int vertex) const
```

O(1)。頂点vertexの列上の位置を返す。

### `operator[]`

```cpp
int operator[](int vertex) const
```

O(1)。頂点vertexの列上の位置を返す。

### `vertex`

```cpp
int vertex(int index) const
```

O(1)。列上の位置indexにある頂点を返す。

### `edge_index`

```cpp
int edge_index(int id) const
```

O(1)。辺番号idを、その子頂点に対応する列上の位置へ変換する。

### `parent`

```cpp
int parent(int vertex) const
```

O(1)。頂点vertexの親を返す。根では-1。

### `depth`

```cpp
int depth(int vertex) const
```

O(1)。頂点vertexの根からの辺数を返す。

### `head`

```cpp
int head(int vertex) const
```

O(1)。頂点vertexが属するheavy pathの最上位頂点を返す。

### `root`

```cpp
int root() const noexcept
```

O(1)。根を返す。空木では-1。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Vertex Add Path Sum](https://judge.yosupo.jp/problem/vertex_add_path_sum)。

```cpp
poe::hld tree(graph, 0);
long long answer = 0;
for (const auto& segment : tree.path(from, to)) {
    answer += sums.sum(segment.left, segment.right);
}
```

`verify/heavylightdecomposition.test.cpp`で2026-07-15に全ケースAC確認済み。

[AtCoder ABC460 G - Vertex Flip Query](https://atcoder.jp/contests/abc460/tasks/abc460_g)では、
HLD区間上のFenwick Treeで同色祖先区間の上端を探し、木上区間加算・一点取得を行う。
`verify/atcoder_abc460_g.cpp`で公式sampleを確認済み。
