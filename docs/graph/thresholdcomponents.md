---
title: "thresholdcomponents"
documentation_of: //cp/graph/thresholdcomponents.hpp
---

# `thresholdcomponents`

## 概要

重み付き無向グラフで、辺の重み上限を変えたときに現れる連結成分をいくつか選び、その頂点集合の和を作る。
同じ集合を作るために最低何成分必要かごとに、異なる頂点集合の個数を求める。

## 厳密な定義

重み付き無向グラフ $G=(V,E)$ に対し、整数 $x$ 以下の辺だけを残した部分グラフを $G_x$ とする。
選択可能な集合族 $\mathcal C$ は、任意の $x$ に対する $G_x$ の全連結成分の頂点集合である。
返り値 `count[k]` は、

$$
S=C_1\cup\cdots\cup C_k\qquad(C_i\in\mathcal C)
$$

と表せるが、$k-1$ 個以下では表せない頂点集合 $S\subseteq V$ の個数である。`count[0]=1` は空集合を表す。

## Include

```cpp
#include "graph/thresholdcomponents.hpp"
```

## API

```cpp
vector<T> threshold_component_union_counts<T>(graph, maximum_selections);
```

- `graph`: 重み付き無向グラフ。非連結、多重辺、自己辺、同じ重みの辺を許す。
- `maximum_selections`: 求める最少選択数の上限。
- 返り値: 長さ `maximum_selections + 1` の個数列。
- `T`: 整数から構築でき、加減乗算を持つ型。通常はmodint。

同じ重みの辺を同時に追加するKruskal merge forest上で多項式DPを行う。
$m$ 個の子成分を併合するとき、子の多項式積に含まれる「全子を一つずつ選ぶ」$X^m$ を、
新成分を一つ選ぶ $X$ へ置き換える。次数を $K$ で打ち切る。

頂点数を $n$、辺数を $m$、`maximum_selections`を $K$ とすると、償却計算量は
$O(m\log m+nK)$、使用メモリは $O(nK+m)$。

## 使用例

[AtCoder ABC235 Ex - Painting Weighted Graph](https://atcoder.jp/contests/abc235/tasks/abc235_h)では、
一操作で一つの閾値連結成分を赤くする。高々 $K$ 回で作れる集合数は係数のprefix和である。

```cpp
auto count = threshold_component_union_counts<modint998244353>(graph, K);
cout << accumulate(count.begin(), count.end(), modint998244353{}) << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `threshold_component_union_counts`

```cpp
template<class T, weighted_graph_type Graph> requires (!Graph::is_directed) std::vector<T> threshold_component_union_counts( const Graph& graph, int maximum_selections )
```

O(m log m+nK)時間・O(nK+m)領域。重み閾値連結成分のunionを作る最少選択数別の集合数。

<!-- END AUTO-GENERATED API REFERENCE -->
