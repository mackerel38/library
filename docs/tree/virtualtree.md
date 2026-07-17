---
title: "Virtual Tree"
documentation_of: //cp/tree/virtualtree.hpp
---

# Virtual Tree


## 概要

元の木から指定した`k`頂点と、それらを結ぶために必要なLCAだけを残した圧縮木を作る。
同じ色の頂点だけで木DPする場合など、全頂点を色ごとに走査する処理を避けられる。

## 厳密な定義

- `virtualtreeresult`: virtual tree構築結果。local indexから元頂点・親・子・圧縮辺長を取得する。
- `virtualtree`: 指定頂点と必要なLCAだけの木を作る: virtualtree builder(graph); buildはO(k log k log n)。

```cpp
virtualtree compressed_tree(graph, root);
auto tree = compressed_tree.build(vertices);
for (int node = tree.size() - 1; node > 0; --node) {
    int parent = tree.parent[node];
    // tree.distance[node]: parentからnodeまでの元の重み和
    // tree.edge_length[node]: parentからnodeまでの元の辺数
}
```

結果の添字はvirtual tree内のlocal indexで、`tree[index]`または`tree.vertex[index]`が元頂点。
`parent`、`children`、圧縮辺の`distance`・`edge_length`、入力で明示指定されたかを示す
`selected`を持つ。`index(original_vertex)`は含まれる元頂点のlocal index、なければ`-1`。

入力頂点は重複してよい。空入力には空木を返す。根のlocal indexは、空でなければ常に`0`。

## Include

```cpp
#include "tree/virtualtree.hpp"
```

## 計算量

元の木の前処理は`O(n log n)`、領域`O(n log n)`。
`build`は入力頂点数を`k`として`O(k log k log n)`、結果の頂点数は高々`2k-1`。
結果領域は`O(k)`。重み付き木・重みなし木の両方を同じtemplateで扱う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `virtualtreeresult`

```cpp
template <class Distance> struct virtualtreeresult
```

virtual tree構築結果。local indexから元頂点・親・子・圧縮辺長を取得する。

### `size`

```cpp
int size() const noexcept
```

O(1)。virtual tree頂点数を返す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(1)。local indexに対応する元の木の頂点を返す。

### `index`

```cpp
int index(int original_vertex) const
```

O(log k)。元頂点のlocal indexを返す。含まれなければ-1。

### `root`

```cpp
int root() const noexcept
```

O(1)。根のlocal indexを返す。空なら-1。

### `virtualtree`

```cpp
template <undirected_graph_type Graph> struct virtualtree
```

指定頂点と必要なLCAだけの木を作る: virtualtree builder(graph); buildはO(k log k log n)。

### `virtualtree`

```cpp
explicit virtualtree(const Graph& graph, int root = 0) : tree_(graph, root)
```

O(n log n)。元の木をroot根としてLCA前処理する。

### `build`

```cpp
virtualtreeresult<distance_type> build(std::vector<int> vertices) const
```

O(k log k log n)。verticesと必要なLCAからなる圧縮木を返す。重複入力は一つにする。

### `distance`

```cpp
distance_type distance(int left, int right) const
```

O(log n)。元の木における二頂点間の重み和を返す。

### `edge_distance`

```cpp
int edge_distance(int left, int right) const
```

O(log n)。元の木における二頂点間の辺数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC359 G - Sum of Tree Distance](https://atcoder.jp/contests/abc359/tasks/abc359_g)では、
値が同じ頂点ごとにvirtual treeを作り、圧縮辺をまたぐ同色頂点対の個数を掛けて距離和を求める。
`verify/atcoder_abc359_g.cpp`を収録した。

## 検証

- `tests/api/tree/virtualtree.cpp`: LCA補完・親子・距離・重複入力
- `tests/property/tree/virtualtree.cpp`: ランダム木の指定頂点対距離和を元のLCA結果と比較
- `verify/atcoder_abc359_g.cpp`: 公式サンプル確認対象
