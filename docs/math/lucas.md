---
title: "Lucasの定理による巨大二項係数"
documentation_of: //cp/math/lucas.hpp
---

# Lucasの定理による巨大二項係数

## 概要

小さい素数 $p$ を法として、非常に大きい $n,k$ に対する二項係数 $\binom nk$ を求める。
通常の階乗表を $n$ まで作れない場合でも、$p-1$ までの前計算だけで処理できる。

法そのものに比例する前計算が必要なため、$10^9+7$のような大きい素数には向かない。

## 厳密な定義

$n,k$の$p$進表記を

$$
n=\sum_i n_i p^i,\qquad k=\sum_i k_i p^i
$$

とする。Lucasの定理

$$
\binom nk\equiv\prod_i\binom{n_i}{k_i}\pmod p
$$

を使い、各桁の二項係数を前計算した階乗と逆階乗から求める。

## Include

```cpp
#include "math/lucas.hpp"
```

## 使い方

```cpp
lucascombination<200003> combination;
auto value = combination.choose(n, k);
```

template引数は素数でなければならない。`choose`は`staticmodint<prime>`を返し、
`n<0`、`k<0`、`k>n`では0を返す。

## 計算量

- 構築: $O(p)$ 時間、$O(p)$ 領域
- `choose(n, k)`: $O(\log_p n)$

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lucascombination`

```cpp
template <int prime> struct lucascombination
```

構築O(p)、取得O(log_p n)。小さい素数pを法とする巨大な二項係数をLucasの定理で求める。

### `lucascombination`

```cpp
lucascombination() : digit_(prime - 1)
```

O(p)時間・O(p)領域。0以上p未満の各桁に使う二項係数を前計算する。

### `choose`

```cpp
mint choose(long long n, long long k) const
```

O(log_p n)。nCk mod pを返す。n<0、k<0、k>nでは0。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC279 Ex - Sum of Prod of Min](https://atcoder.jp/contests/abc279/tasks/abc279_h)では、
最大 $3\times10^{12}$ 程度の上側引数を持つ二項係数を法 $200003$ で求めるために使う。
`fps/partition.hpp`のEuler積係数関数から本構造が利用されている。
