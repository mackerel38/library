---
title: "最大cut"
documentation_of: //cp/graph/maximumcut.hpp
---

# 最大cut


## 概要

頂点数が小さい無向グラフを二色に分け、異なる色を結ぶ辺の重み和を最大化する。
重みなしグラフではcutに入る辺数を最大化する。

## 厳密な定義

- `maximumcutresult`: 最大cutの重みと二色分け。side[v]が異なる辺がcutへ入る。
- `maximum_cut`: O(2^(n-1)m)。小頂点数の非負重み無向グラフで最大cutと二色分けを返す。

## Include

```cpp
#include "graph/maximumcut.hpp"

auto result = maximum_cut(graph);
cout << result.weight << '\n';
for (int v = 0; v < graph.size(); ++v) cout << result.side[v];
```

`side[v]`が異なる辺だけがcutへ入る。全頂点の色を反転してもcutは変わらないため、
頂点0の色を固定して`2^(n-1)`通り列挙する。非負重みを前提とする。

グラフを二部化するための最小辺削除数は、重みなしなら
`graph.edge_count() - maximum_cut(graph).weight`となる。

## 計算量

頂点数`n`、辺数`m`として`O(2^(n-1)m)`時間、`O(n)`追加領域。
`n<64`を必要とするが、実用上は二十数頂点以下を目安にする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `maximumcutresult`

```cpp
template <class Weight> struct maximumcutresult
```

最大cutの重みと二色分け。side[v]が異なる辺がcutへ入る。

### `maximum_cut`

```cpp
template <undirected_graph_type Graph> auto maximum_cut(const Graph& graph)
```

O(2^(n-1)m)。小頂点数の非負重み無向グラフで最大cutと二色分けを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC427 C - Bipartize](https://atcoder.jp/contests/abc427/tasks/abc427_c):
  全辺から最大cutに残せる辺を引く。

`verify/atcoder_abc427_c.cpp`に提出用コードを収録し、公式sample 3件との一致を確認済み。


`tests/property/graph/maximumcut.cpp`では、返された彩色の重みと再帰全彩色の最適値を比較する。
