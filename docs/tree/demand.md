---
title: "木上の需要残余DP"
documentation_of: //cp/tree/demand.hpp
---

# 木上の需要残余DP

## 概要

根付き木で、各頂点の子部分木から上がってくる需要をその頂点の処理可能量だけ減らし、
自分の需要を加えた残余を求める。頂点値を需要の有無へ変換する閾値探索も行える。

## 厳密な定義

頂点 $v$ の需要を $d_v\ge0$、処理可能量を $a_v\ge0$ とする。根付き木の各頂点について

$$
r_v=d_v+\max\left(\sum_{c\in\operatorname{children}(v)}r_c-a_v,0\right)
$$

で定めた残余 $r_v$ を返す。

閾値版では根以外の頂点に値 $A_v$ があり、$A_v\ge x$ の頂点だけを需要$1$、全頂点の
処理可能量を一定値$a$とする。根の残余が正となる最大の入力値$x$を返す。

## Include

```cpp
#include "tree/demand.hpp"
```

## APIの選び方

| 条件 | API | 出力 |
|---|---|---|
| 需要・処理可能量を直接指定 | `tree_demand_residual` | 全頂点の$r_v$ |
| 頂点値を閾値で需要$0/1$へ変換 | `maximum_tree_demand_threshold` | 根の残余が正となる最大値 |

```cpp
auto residual = tree_demand_residual(graph, demand, allowance, root);

auto threshold = maximum_tree_demand_threshold(
    graph, values, root, allowance_per_vertex);
```

`maximum_tree_demand_threshold`は該当する閾値がなければ`std::nullopt`を返す。根の`values[root]`は
使用しない。入力に現れる非根頂点の値だけを候補として探索する。

- `tree_demand_residual`: $O(n)$時間、$O(n)$領域
- `maximum_tree_demand_threshold`: $O(n\log n)$時間、$O(n)$領域

実装は再帰を使わないため、深さ$n$の木でもcall stackを消費しない。

## 頻出事故

- `graph`は連結な無向木である必要がある。
- 根の需要も`tree_demand_residual`では通常どおり加算される。
- 閾値版だけは根を需要化しない。
- 子の残余は合計してから処理可能量を一度引く。子ごとには引かない。
- 需要・残余の総和が型`T`の範囲に収まる必要がある。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `tree_demand_residual`

```cpp
template<undirected_graph_type Graph, class T> std::vector<T> tree_demand_residual( const Graph& graph, const std::vector<T>& demand, const std::vector<T>& allowance, int root = 0 )
```

O(n)。dp[v]=demand[v]+max(sum(dp[child])-allowance[v],0)を根付き木上で返す。

### `maximum_tree_demand_threshold`

```cpp
template<undirected_graph_type Graph, class Value> std::optional<Value> maximum_tree_demand_threshold( const Graph& graph, const std::vector<Value>& values, int root = 0, long long allowance_per_vertex = 1 )
```

O(n log n)。値がthreshold以上の非根頂点を需要1としたとき根の残余が正となる最大値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC246 G - Game on Tree 3](https://atcoder.jp/contests/abc246/tasks/abc246_g)では、
全頂点の処理可能量を$1$として`maximum_tree_demand_threshold`へ木と頂点値を渡す。
返り値がなければ答えは$0$である。`verify/atcoder_abc246_g.cpp`に提出用コードを用意している。
