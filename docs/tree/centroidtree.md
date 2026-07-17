---
title: "Centroid Decomposition"
documentation_of: //cp/tree/centroidtree.hpp
---

# Centroid Decomposition

- Status: experimental
- Header: `cp/tree/centroidtree.hpp`
- Symbol: `poe::centroidtree<Graph>`, `poe::centroidpathentry<Distance>`

## Include

```cpp
#include "tree/centroidtree.hpp"
```

## できること

木を重心で再帰的に分け、深さ`O(log n)`のcentroid treeを構築する。
各元頂点からcentroid ancestorまでの距離も前計算するため、動的な距離queryの土台として使える。

```cpp
centroidtree decomposition(graph);
for (const auto& entry : decomposition.path(vertex)) {
    int centroid = entry.centroid;
    auto distance = entry.distance;
}
```

`parent`、`children`、`depth`はcentroid tree上の情報である。
`operator[]`と`order()`は重心を選んだ順で、親は必ず子より前に現れる。

`path(vertex)`はcentroid treeの根側から`vertex`自身までのentry列を返す。
`entry.branch`は、そのcentroidから元頂点へ向かうとき最初に通る元の木の隣接頂点で、
centroid自身のentryだけ`-1`。同じbranch内の寄与を除外する距離集約に使える。

## 計算量

構築時間・距離表の領域は`O(n log n)`。各頂点の`path`長は`O(log n)`。
重み付き・重みなしの`undirected_graph`を同じtemplateで扱う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `centroidpathentry`

```cpp
template <class Distance> struct centroidpathentry
```

元頂点からcentroid ancestorへの距離。branchはcentroidから最初に進む元頂点。

### `centroidtree`

```cpp
template <undirected_graph_type Graph> struct centroidtree
```

木を重心で再帰分割する: centroidtree decomposition(graph); 構築O(n log n)。

### `centroidtree`

```cpp
explicit centroidtree(const Graph& graph) : parent_(graph.size(), -1), depth_(graph.size()), children_(graph.size()), path_(graph.size()), removed_(graph.size()), component_parent_(graph.size()), component_size_(graph.size())
```

O(n log n)。元の木のcentroid decompositionと全頂点からancestor重心への距離を構築する。

### `root`

```cpp
int root() const noexcept
```

O(1)。centroid treeの根を返す。空木では-1。

### `parent`

```cpp
int parent(int vertex) const
```

O(1)。vertexのcentroid tree上の親を返す。根では-1。

### `depth`

```cpp
int depth(int vertex) const
```

O(1)。vertexのcentroid tree上の深さを返す。

### `children`

```cpp
const std::vector<int>& children(int vertex) const
```

O(1)。vertexのcentroid tree上の子を返す。

### `path`

```cpp
const std::vector<centroidpathentry<distance_type>>& path(int vertex) const
```

O(1)。元頂点vertexからcentroid ancestorへの情報を根側から順に返す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(1)。構築順index番目のcentroidを返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `order`

```cpp
const std::vector<int>& order() const noexcept
```

O(1)。centroidの構築順を返す。親は必ず子より前にある。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC291 Ex - Balanced Tree](https://atcoder.jp/contests/abc291/tasks/abc291_h)は、
centroid treeの各頂点の親をそのまま出力すれば条件を満たす。
`verify/atcoder_abc291_h.cpp`を収録した。judge未提出。

距離の動的集約は複数の問題族があるため、特定の集約方法をcoreへ固定しない。
最短距離・距離以内個数などは、この`path`と`branch`を使うrecipeとして追加する。

## 検証

- `tests/api/tree/centroidtree.cpp`: centroid親子、weighted距離、branch
- `tests/property/tree/centroidtree.cpp`: ランダム木で親chain・LCA距離・深さ上限を比較
- `verify/atcoder_abc291_h.cpp`: 公開問題用コード
