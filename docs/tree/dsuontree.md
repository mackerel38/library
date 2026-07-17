---
title: "DSU on Tree"
documentation_of: //cp/tree/dsuontree.hpp
---

# DSU on Tree


## 概要

各頂点の部分木に対する統計を、最大部分木を持つ子の状態だけ使い回して列挙する。
親配列から構築し、現在の表へ頂点を出し入れする`add`、`remove`と、答えを読む
`query`を渡す。

## 厳密な定義

- `dsuontree`: 部分木情報を使い回すDSU on Tree: dsuontree tree(parent); run(add,remove,query)。

```cpp
dsuontree tree(parent);
tree.run(
    [&](int vertex) { ++count[color[vertex]]; },
    [&](int vertex) { --count[color[vertex]]; },
    [&](int vertex) { answer[vertex] = current_value; });
```

`query(vertex)`が呼ばれた時点では、表にちょうど`vertex`の部分木だけが入っている。
`run()`開始前の表は空であること。終了後も空へ戻る。
実装は再帰を使わないため、細長い巨大木でもcall stackを消費しない。

## Include

```cpp
#include "tree/dsuontree.hpp"
```

## 計算量

構築`O(n)`、`add`と`remove`の呼び出し合計`O(n log n)`、`query`は`n`回、領域`O(n)`。
各callbackが`O(1)`なら全体`O(n log n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `dsuontree`

```cpp
struct dsuontree
```

部分木情報を使い回すDSU on Tree: dsuontree tree(parent); run(add,remove,query)。

### `dsuontree`

```cpp
explicit dsuontree(std::vector<int> parent) : parent_(std::move(parent)), children_(parent_.size()), in_(parent_.size()), out_(parent_.size()), subtree_size_(parent_.size(), 1), heavy_(parent_.size(), -1)
```

O(n)。parent[root]=-1である根付き木を前処理する。

### `run`

```cpp
template <class Add, class Remove, class Query> void run(Add add, Remove remove, Query query) const
```

O(n log n)回のcallback。query(v)時に表はvの部分木だけを含み、終了時は空になる。

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

### `parent`

```cpp
int parent(int vertex) const
```

O(1)。頂点vertexの親を返す。根では-1。

### `heavy_child`

```cpp
int heavy_child(int vertex) const
```

O(1)。頂点vertexのheavy childを返す。葉では-1。

### `subtree_size`

```cpp
int subtree_size(int vertex) const
```

O(1)。頂点vertexの部分木サイズを返す。

### `index`

```cpp
int index(int vertex) const
```

O(1)。頂点vertexのEuler tour上の位置を返す。

### `out`

```cpp
int out(int vertex) const
```

O(1)。頂点vertexの部分木区間[index(vertex),out(vertex))の右端を返す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(1)。Euler tour上のindex番目の頂点を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC454 G - Mode in the Subtree](https://atcoder.jp/contests/abc454/tasks/abc454_g)では、
現在の部分木における色別個数、頻度別の色数、最大頻度を配列で管理する。
`verify/atcoder_abc454_g.cpp`に使用例を収録した。

## 検証

- `tests/api/tree/dsuontree.cpp`: 公開APIと部分木和
- `tests/property/tree/dsuontree.cpp`: ランダム木で部分木色頻度の愚直比較
- `verify/atcoder_abc454_g.cpp`: 実在問題用コード
