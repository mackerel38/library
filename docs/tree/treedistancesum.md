---
title: "木上距離和"
documentation_of: //cp/tree/treedistancesum.hpp
---

# 木上距離和

- Header: `cp/tree/treedistancesum.hpp`
- Symbol: `poe::treedistancesum`
- Status: experimental

## どんな問題に使えるか

重みなし木を一度前処理し、全頂点・固定根部分木・二つの施設の近い方への距離和を求める。

```cpp
#include "tree/treedistancesum.hpp"

poe::treedistancesum distances(graph);
long long all = distances.sum(vertex);
long long inside = distances.subtree_sum(source, subtree_root);
long long nearest = distances.min_sum(left, right);
```

- `sum(v)`: `v`から全頂点への距離和を`O(1)`
- `subtree_sum(v,r)`: 固定根で`r`の部分木内への距離和を`O(log n)`
- `min_sum(a,b)`: 全頂点`v`について`min(d(v,a),d(v,b))`を合計して`O(log n)`

構築は`O(n log n)`、領域は`O(n log n)`。空でない重みなし木を要求する。
部分木の深さ総和、祖先path上の部分木サイズ総和、全頂点距離和のreroot差分を前計算する。
二点の近い方はpath中央の辺または頂点で木を二領域へ分け、各領域の距離和へ帰着する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `treedistancesum`

```cpp
template<undirected_graph_type Graph> requires (!Graph::is_weighted) struct treedistancesum
```

木上の全体・部分木・二点の近い方への距離和: treedistancesum distances(graph)。

### `treedistancesum`

```cpp
explicit treedistancesum(const Graph& graph, int root = 0) : tree_(graph, root), subtree_depth_sum_(graph.size()), ancestor_size_sum_(graph.size()), distance_sum_(graph.size())
```

O(n log n)。重みなし木をroot根として距離和を前処理する。

### `sum`

```cpp
long long sum(int vertex) const
```

O(1)。vertexから全頂点への距離和を返す。

### `subtree_sum`

```cpp
long long subtree_sum(int source, int subtree_root) const
```

O(log n)。sourceからsubtree_rootの固定根部分木内の全頂点への距離和を返す。

### `min_sum`

```cpp
long long min_sum(int left, int right) const
```

O(log n)。全頂点vについてmin(distance(v,left),distance(v,right))を合計する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC298 Ex - Sum of Min of Length](https://atcoder.jp/contests/abc298/tasks/abc298_h)では、
各queryへ`distances.min_sum(left, right)`を呼ぶだけでよい。
`verify/atcoder_abc298_h.cpp`で公式sample 2件を確認済み。

## 検証

- `tests/api/tree/treedistancesum.cpp`: pathと星型木の基本値
- `tests/property/tree/treedistancesum.cpp`: 全頂点対距離の愚直BFSとの全query比較
