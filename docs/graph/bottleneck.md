---
title: "頂点・辺bottleneck path"
documentation_of: //cp/graph/bottleneck.hpp
---

# 頂点・辺bottleneck path


## 概要

path上の最大頂点costを最小化する多数queryと、最大辺重みを最小化するminimax距離に対する
辺重み増加の感度判定を扱う。

## 厳密な定義

### `minimum_vertex_bottleneck_paths`

各query $(s,t)$について、$s$から$t$へのpath $P$に対する
$$
\min_P\max_{v\in P} c_v
$$
を返す。到達不能なら`std::nullopt`である。有向・無向、重み付き・重みなしのgraphへ使える。

### `bottlenecksensitivity`

連結な重み付き無向グラフで、path上の最大辺重みを最小化したminimax距離を考える。
指定辺の重みを増やしたとき、指定二頂点間のminimax距離が真に増えるかを判定する。

## Include

```cpp
#include "graph/bottleneck.hpp"

auto costs = poe::minimum_vertex_bottleneck_paths(graph, vertex_cost, queries);
poe::bottlenecksensitivity sensitivity(graph);
bool changed = sensitivity.increases(edge_id, source, target);
```

## 頂点bottleneck query

頂点をcost昇順にWarshall法の中継点へ加え、到達集合を64bit単位のbitsetで更新する。
頂点数を$n$、query数を$q$として、$O(n^3/64+nq+n\log n)$時間、$O(n^2/64+q)$領域。
同じcostの頂点はすべて追加してからqueryを確定する。

path costには始点・終点も含まれる。自己pathではその頂点のcostを返す。

前処理は`O(m log m+n log n)`、各queryは`O(log n)`、領域は`O(n log n+m)`。
自己辺・多重辺・同重み辺を扱える。

Kruskal法の各同重み層で、それ未満の辺による成分を縮約してLowLinkを行う。
指定辺が縮約グラフの橋であり、そのMST辺が二頂点を分離し、かつMST path最大重みが
指定辺重みと一致するときだけ不可欠である。`critical(edge_id)`では縮約層の橋かだけを取得できる。

グラフは空でなく連結であること。重み型は比較・等値比較・デフォルト構築が可能であること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimum_vertex_bottleneck_paths`

```cpp
template<graph_type Graph, class Cost> std::vector<std::optional<Cost>> minimum_vertex_bottleneck_paths( const Graph& graph, const std::vector<Cost>& vertex_cost, const std::vector<std::pair<int, int>>& queries )
```

O(n^3/64+nq+n log n)。各queryのpath上最大頂点costの最小値を返す。到達不能ならnullopt。

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

[AtCoder ABC287 Ex - Directed Graph and Query](https://atcoder.jp/contests/abc287/tasks/abc287_h)では、
頂点$i$のcostを$i+1$として全queryを渡す。`verify/atcoder_abc287_h.cpp`に提出用コードを収録している。

[AtCoder ABC301 Ex - Difference of Distance](https://atcoder.jp/contests/abc301/tasks/abc301_h)では、
各queryを`sensitivity.increases(edge, source, target)`だけで処理する。
`verify/atcoder_abc301_h.cpp`で公式sample 2件を確認済み。

## 検証

- `tests/api/graph/bottleneck.cpp`: 同重み多重辺と不可欠辺の基本例
- `tests/property/graph/bottleneck.cpp`: 頂点costのminimax Floyd、および一辺を実際に1増加したminimax Floydとの比較
