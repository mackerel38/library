---
title: "重み付き二部matching"
documentation_of: //cp/flow/matching.hpp
---

# 重み付き二部matching

- Header: `cp/flow/matching.hpp`
- Symbol: `poe::weightedbipartiteedge`, `poe::minimum_weight_bipartite_matching`
- Status: experimental

## どんな問題に使えるか

左右の頂点を高々一度ずつ使う辺集合を自由な本数だけ選び、辺重み和を最小化する。
空matchingを許すため、正の辺だけを選ぶことはない。負辺を一定値だけ平行移動し、
matchingサイズごとの最小費用を`mincostslope`で列挙して元の重みへ戻す処理を内部化している。

```cpp
#include "flow/matching.hpp"

std::vector<poe::weightedbipartiteedge<long long>> edges;
edges.push_back({left, right, weight});
long long cost = poe::minimum_weight_bipartite_matching(left_size, right_size, edges);
```

選んだ辺の復元やmatchingサイズ固定には対応しない。その場合は`costflowgraph`と`mincost`を直接使う。

## 計算量

選ばれる最大辺数を`F`、残余グラフの頂点数を`n`、辺数を`m`として
`O(F(m+n log n))`時間、`O(n+m)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `weightedbipartiteedge`

```cpp
template<class Cost> struct weightedbipartiteedge
```

minimum_weight_bipartite_matchingへ渡す重み付き二部辺。

### `minimum_weight_bipartite_matching`

```cpp
template<class Cost> Cost minimum_weight_bipartite_matching( int left_size, int right_size, const std::vector<weightedbipartiteedge<Cost>>& edges )
```

O(F(m+n log n))。空matchingも許して重み和が最小の二部matchingの重みを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC407 G - Domino Covering SUM](https://atcoder.jp/contests/abc407/tasks/abc407_g)では、
市松模様で盤面を二部化し、隣接2マスの値の和を辺重みにする。
全マス和から最小matching重みを引けば答えになる。
`verify/atcoder_abc407_g.cpp`で公式sampleを確認済み。judge ACは未確認。
小さい二部グラフの全部分集合とも照合済み。
