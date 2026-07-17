---
title: "抽象化木DP"
documentation_of: //cp/tree/dp.hpp
---

# 抽象化木DP

- Header: `cp/tree/dp.hpp`
- Symbol: `poe::treedp`
- Status: experimental

## どんな問題に使えるか

根付き木を葉から処理し、子のDP値を結合して各頂点の部分木DPを求める問題に使う。
`merge`、単位元、頂点作用、辺作用を指定するため、数え上げ、最適化、木ナップサックなどに再利用できる。

```cpp
#include "tree/dp.hpp"

auto dp = poe::treedp(
    graph, root, identity,
    merge,
    add_vertex,
    add_edge
);
```

`merge`は結合則を満たし、`identity`を単位元とすること。
`add_edge(child_value, parent_to_child_edge)`は子の値を親へ渡す形へ変換する。
結合一回を`M`として`O(nM)`時間。固定長状態なら`O(n)`時間、DP配列を状態にすれば結合費用を含む。

全頂点を根とした答えが必要なら`tree/rerooting.hpp`、更新ごとに再計算するなら
`tree/statictoptree.hpp`を使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `treedp`

```cpp
template<undirected_graph_type Graph, class Value, class Merge, class AddVertex, class AddEdge> std::vector<Value> treedp( const Graph& graph, int root, const Value& identity, Merge merge, AddVertex add_vertex, AddEdge add_edge )
```

O(n)。treedp(graph, root, identity, merge, add_vertex, add_edge): 各頂点の部分木DPを返す。
mergeは結合則とidentityを持ち、add_edge(child_value, parent_to_child_edge)で子を親へ渡す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[EDPC P - Independent Set](https://atcoder.jp/contests/dp/tasks/dp_p)へ直接使える。
`verify/atcoder_dp_p.cpp`は公式サンプル確認済み。
