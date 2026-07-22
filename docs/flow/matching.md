---
title: "重み付き二部matching"
documentation_of: //cp/flow/matching.hpp
---

# 重み付き二部matching


## 概要

重み付き二部グラフで、matchingの辺数ごとの重み和の最小値・最大値をまとめて求める。
辺数を自由に選んだ最小重みmatchingだけが必要な場合も、一つの関数で求められる。

## 厳密な定義

左頂点集合 $L$、右頂点集合 $R$ と重み付き辺集合 $E$ が与えられる。matching $M\subseteq E$ は、
どの二辺も端点を共有しない辺集合である。`*_matching_values` は、matchingが存在する全ての
$0\le k\le F$ について

$$
\min_{M:\lvert M\rvert=k}\sum_{e\in M}w_e
\quad\text{または}\quad
\max_{M:\lvert M\rvert=k}\sum_{e\in M}w_e
$$

を添字 $k$ に格納した列を返す。$F$ は最大matchingサイズである。

## Include

```cpp
#include "flow/matching.hpp"

std::vector<poe::weightedbipartiteedge<long long>> edges;
edges.push_back({left, right, weight});
long long cost = poe::minimum_weight_bipartite_matching(left_size, right_size, edges);
```

## APIの選び方

| 目的 | API | 出力 |
|---|---|---|
| 辺数を自由に選び、重み和を最小化 | `minimum_weight_bipartite_matching` | 最小重み |
| 辺数ごとの最小重み | `minimum_weight_bipartite_matching_values` | `answer[k]` はちょうど $k$ 辺の最小重み |
| 辺数ごとの最大重み | `maximum_weight_bipartite_matching_values` | `answer[k]` はちょうど $k$ 辺の最大重み |

```cpp
auto maximum = maximum_weight_bipartite_matching_values(
    left_size, right_size, edges);
```

負の重みと平行辺を許す。返り値は必ず空matchingの値`answer[0] = 0`を含む。
選んだ辺の復元には対応しないため、必要なら`costflowgraph`と`mincost`を直接使う。

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

### `minimum_weight_bipartite_matching_values`

```cpp
template<class Cost> std::vector<Cost> minimum_weight_bipartite_matching_values( int left_size, int right_size, const std::vector<weightedbipartiteedge<Cost>>& edges )
```

O(F(m+n log n))。matchingサイズkごとの最小重みをk=0..Fの順に返す。

### `maximum_weight_bipartite_matching_values`

```cpp
template<class Cost> std::vector<Cost> maximum_weight_bipartite_matching_values( int left_size, int right_size, const std::vector<weightedbipartiteedge<Cost>>& edges )
```

O(F(m+n log n))。matchingサイズkごとの最大重みをk=0..Fの順に返す。

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
`verify/atcoder_abc407_g.cpp`で公式sampleを確認済み。
小さい二部グラフの全部分集合とも照合済み。

[AtCoder ABC247 G - Dream Team](https://atcoder.jp/contests/abc247/tasks/abc247_g)では、
大学を左頂点、得意分野を右頂点、人を重み付き辺として
`maximum_weight_bipartite_matching_values`へ渡す。返り値の添字$1$以降を順に出力する。
`verify/atcoder_abc247_g.cpp`に提出用コードを用意している。
