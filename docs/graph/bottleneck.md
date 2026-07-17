---
title: "Minimax距離の辺重み増加感度"
documentation_of: //cp/graph/bottleneck.hpp
---

# Minimax距離の辺重み増加感度


## 概要

連結な重み付き無向グラフで、二頂点間のpath上最大辺重みを最小化した値をminimax距離とする。
固定グラフを前処理し、指定した一辺の重みを増やしたとき、指定二頂点間のminimax距離が
真に増えるかを判定する。

## 厳密な定義

- `bottlenecksensitivity`: minimax距離に指定辺が不可欠かを判定する: bottlenecksensitivity sensitivity(graph)。

## Include

```cpp
#include "graph/bottleneck.hpp"

poe::bottlenecksensitivity sensitivity(graph);
bool changed = sensitivity.increases(edge_id, source, target);
```

前処理は`O(m log m+n log n)`、各queryは`O(log n)`、領域は`O(n log n+m)`。
自己辺・多重辺・同重み辺を扱える。

Kruskal法の各同重み層で、それ未満の辺による成分を縮約してLowLinkを行う。
指定辺が縮約グラフの橋であり、そのMST辺が二頂点を分離し、かつMST path最大重みが
指定辺重みと一致するときだけ不可欠である。`critical(edge_id)`では縮約層の橋かだけを取得できる。

グラフは空でなく連結であること。重み型は比較・等値比較・デフォルト構築が可能であること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `bottlenecksensitivity`

```cpp
template<weighted_graph_type Graph> requires (!Graph::is_directed) struct bottlenecksensitivity
```

minimax距離に指定辺が不可欠かを判定する: bottlenecksensitivity sensitivity(graph)。

### `bottlenecksensitivity`

```cpp
explicit bottlenecksensitivity(const Graph& graph) : graph_(&graph), critical_(graph.edge_count()), edge_child_(graph.edge_count(), -1), tree_(graph.size())
```

O(m log m+n log n)。連結な重み付き無向グラフを前処理する。

### `increases`

```cpp
bool increases(int edge_id, int s, int t) const
```

O(log n)。辺edge_idの重み増加でs-t minimax距離が増えるならtrueを返す。

### `critical`

```cpp
bool critical(int edge_id) const
```

O(1)。辺edge_idがその重み以下の部分グラフで不可欠な橋ならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC301 Ex - Difference of Distance](https://atcoder.jp/contests/abc301/tasks/abc301_h)では、
各queryを`sensitivity.increases(edge, source, target)`だけで処理する。
`verify/atcoder_abc301_h.cpp`で公式sample 2件を確認済み。

## 検証

- `tests/api/graph/bottleneck.cpp`: 同重み多重辺と不可欠辺の基本例
- `tests/property/graph/bottleneck.cpp`: 一辺を実際に1増加したminimax Floydとの全query比較
