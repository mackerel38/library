---
title: "Static top tree"
documentation_of: //cp/tree/statictoptree.hpp
---

# Static top tree

- Header: `cp/tree/statictoptree.hpp`
- Symbol: `poe::statictoptree`, `poe::make_statictoptreedp`
- Status: experimental

## どんな問題に使えるか

木DPの頂点値をオンライン更新し、木全体のDP値を毎回求める問題に使う。元の木を
深さ`O(log n)`のcluster merge木へ変換し、更新頂点からmerge木の祖先だけを再計算する。

木DPを次の5演算へ分解できる必要がある。

- `vertex(v)`: 頂点だけのpath cluster
- `compress(parent, child)`: heavy path方向の二つのpath cluster
- `rake(left, right)`: 同じvirtual rootを持つpoint cluster
- `add_edge(path)`: path clusterの上へvirtual rootを追加
- `add_vertex(point, v)`: virtual rootを実頂点`v`へ置換

通常の部分木DPと違い、`compress`ではclusterの上下二境界を表せる情報が必要になる。
この演算を結合的かつ正しく定義できないDPには使えない。辺更新は、辺を頂点へ分割した`2n-1`頂点木へ
変換すると扱いやすい。

## API・計算量

```cpp
#include "tree/statictoptree.hpp"

poe::statictoptree tree(graph, root);
auto dp = poe::make_statictoptreedp<Path, Point>(
    tree, vertex, compress, rake, add_edge, add_vertex
);
value[v] = x;
dp.update(v);
Path answer = dp.get();
```

構築`O(n log n)`、領域`O(n)`、頂点更新`O(log n)`、全体取得`O(1)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `statictoptree_type`

```cpp
enum class statictoptree_type
```

Static top treeのcluster生成操作。

### `statictoptree`

```cpp
struct statictoptree
```

根付き木を深さO(log n)のcluster merge木へ変換する。構築O(n log n)。

### `statictoptree`

```cpp
template <undirected_graph_type Graph> explicit statictoptree(const Graph& graph, int root = 0)
```

O(n log n)。無向木をrootで根付けてStatic top treeを構築する。

### `vertex_count`

```cpp
int vertex_count() const noexcept
```

O(1)。元の木の頂点数を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。cluster merge木のnode数を返す。

### `root`

```cpp
int root() const noexcept
```

O(1)。cluster merge木の根を返す。空木では-1。

### `parent`

```cpp
int parent(int node) const
```

O(1)。nodeの親、なければ-1を返す。

### `left`

```cpp
int left(int node) const
```

O(1)。nodeの左の子、なければ-1を返す。

### `right`

```cpp
int right(int node) const
```

O(1)。nodeの右の子、なければ-1を返す。

### `type`

```cpp
statictoptree_type type(int node) const
```

O(1)。nodeの生成操作を返す。

### `vertex`

```cpp
int vertex(int node) const
```

O(1)。vertex/add_vertex nodeに対応する元頂点を返す。それ以外は-1。

### `statictoptreedp`

```cpp
template <class Path, class Point, class Vertex, class Compress, class Rake, class AddEdge, class AddVertex> struct statictoptreedp
```

Static top treeへ5演算の木DPを載せ、頂点更新をO(log n)で再計算する。

### `update`

```cpp
void update(int vertex)
```

O(log n)。元頂点vertexの外部値を変更した後、影響するclusterを再計算する。

### `get`

```cpp
const Path& get() const
```

O(1)。木全体のpath cluster値を返す。

### `make_statictoptreedp`

```cpp
template <class Path, class Point, class Vertex, class Compress, class Rake, class AddEdge, class AddVertex> auto make_statictoptreedp( const statictoptree& tree, Vertex vertex, Compress compress, Rake rake, AddEdge add_edge, AddVertex add_vertex )
```

O(n)。make_statictoptreedp(tree, vertex, compress, rake, add_edge, add_vertex)でDPを構築する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC351 G - Hash on Tree](https://atcoder.jp/contests/abc351/tasks/abc351_g)は初等的な木DPの一点更新、
[NDPC T - 独立集合](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_t)はstatic top tree上の高度なDPである。
本実装はランダム木・ランダム更新を`O(n)`再計算と照合済み。
`verify/atcoder_abc351_g.cpp`で2026-07-15にjudge AC確認済み。
