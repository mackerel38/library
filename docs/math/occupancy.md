---
title: "occupancy"
documentation_of: //cp/math/occupancy.hpp
---

# `occupancy`

## 概要

区別できる各箱へ一種類以上を置き、種類ごとの使用箱数を上限以下にする方法数を求める。
同じ種類は一つの箱へ高々一回だけ置ける。

## 厳密な定義

$n$ 個のlabel付き箱と $t$ 種類がある。各箱 $i$ に非空集合 $S_i\subseteq\{0,\ldots,t-1\}$ を割り当て、
各種類 $j$ について

$$
\left|\{i\mid j\in S_i\}\right|\le c_j
$$

を満たす列 $(S_0,\ldots,S_{n-1})$ の個数を返す。

## Include

```cpp
#include "math/occupancy.hpp"
```

## API

```cpp
T count_nonempty_subset_occupancies<T>(int boxes, const vector<int>& capacities);
```

- `boxes`: label付き箱の個数。
- `capacities[j]`: 種類 `j` を使用できる箱数の上限。`0 <= capacities[j] <= boxes`。
- `T`: 整数から構築でき、四則演算を持つ体の要素。通常はmodint。

$m$ 個の箱だけを使用可能としたとき、種類 $j$ の置き方は
$F_j(m)=\sum_{r=0}^{c_j}\binom mr$ 通りである。空箱が存在しない条件を包除し、

$$
\sum_{m=0}^{n}(-1)^{n-m}\binom nm\prod_{j=0}^{t-1}F_j(m)
$$

を計算する。$F_j(m+1)=2F_j(m)-\binom{m}{c_j}$ により全prefixを更新する。
計算量は $O(nt)$、使用メモリは $O(n+t)$。

## 使用例

[AtCoder ABC235 G - Gardens](https://atcoder.jp/contests/abc235/tasks/abc235_g)では、庭を箱、果物を種類として在庫数を上限にする。

```cpp
cout << count_nonempty_subset_occupancies<modint998244353>(N, {A, B, C}) << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_nonempty_subset_occupancies`

```cpp
template<class T> T count_nonempty_subset_occupancies( int boxes, const std::vector<int>& capacities )
```

O(boxes*types)時間・O(boxes+types)領域。各箱へ非空な種類集合を置き、種類別使用箱数が上限以下の方法数。

<!-- END AUTO-GENERATED API REFERENCE -->
