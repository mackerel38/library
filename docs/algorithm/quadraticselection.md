---
title: "固定個数選択の二次目的関数"
documentation_of: //cp/algorithm/quadraticselection.hpp
---

# 固定個数選択の二次目的関数

## 概要

各候補が二つの値$x_i,y_i$を持つとき、ちょうど$k$個を選び、選んだ$x$の総和の二乗と
$y$の総和を合わせた値を最大化する。期待値の二乗項を含む選択、平均と分散を持つ対象の選択など、
候補同士の影響が「$x$の総和の二乗」だけに集約できる問題に使える。

## 厳密な定義

$n$個の整数点$(x_i,y_i)$と整数$k$に対して、

$$
\max_{S\subseteq\{0,1,\ldots,n-1\},\ |S|=k}
\left\{\left(\sum_{i\in S}x_i\right)^2+\sum_{i\in S}y_i\right\}
$$

を求める。

各実数$c$に対して$c x_i+y_i$が大きい$k$点を選ぶと、$k$点和の上側凸包の頂点を得られる。
二点の順位が入れ替わる$c$を大きい順に走査し、上側凸包上で二次目的関数が最大になる点を調べる。
複数の順位交換が同じ$c$で起きる場合も、同じ値を持つ点を一つの成分として処理する。

## Include

```cpp
#include "algorithm/quadraticselection.hpp"
```

## 使い方

```cpp
vector<pair<long long, long long>> points = {
    {1, 2}, {3, 4}, {-2, 10}
};
__int128_t answer = poe::maximum_quadratic_k_selection(points, 2); // 22
```

### `maximum_quadratic_k_selection`

```cpp
__int128_t maximum_quadratic_k_selection(
    const vector<pair<long long, long long>>& points,
    int k
);
```

`points[i]`を$(x_i,y_i)$として、上で定義した最大値を返す。返り値は`__int128_t`である。
`0 <= k <= points.size()`が必要で、`k = 0`なら$0$を返す。点の重複、負の座標、同じ傾きで
複数の順位交換が起こる入力を許す。

## 計算量

点数を$n$として、時間計算量は$O(n^2\log n)$、領域計算量は$O(n^2)$。

## 使用例

[AtCoder ABC257 Ex - Dice Sum 2](https://atcoder.jp/contests/abc257/tasks/abc257_h)では、サイコロ$i$の
面の和を$S_i$、二乗和を$Q_i$、価格を$C_i$とすると、整数点

$$
(x_i,y_i)=(S_i,\ 6Q_i-S_i^2-36C_i)
$$

を使える。選択集合$D$の利益の期待値は

$$
\frac{1}{36}\left\{\left(\sum_{i\in D}x_i\right)^2+\sum_{i\in D}y_i\right\}
$$

となるため、この関数の返り値を$36$で割ればよい。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `maximum_quadratic_k_selection`

```cpp
inline __int128_t maximum_quadratic_k_selection( const std::vector<std::pair<long long, long long>>& points, int k )
```

maximum_quadratic_k_selection(points, k): O(n^2 log n)時間・O(n^2)領域で、k点を選ぶときの(sum x)^2+sum yの最大値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
