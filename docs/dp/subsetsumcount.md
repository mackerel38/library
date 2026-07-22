---
title: "部分集合和の個数"
documentation_of: //cp/dp/subsetsumcount.hpp
---

# 部分集合和の個数

## 概要

非負整数列から要素を選び、各総和を作る方法の数を、選んだ要素数が偶数の場合と奇数の場合に分けて求める。
通常のDPでは要素数と総和上限の積だけ時間がかかるが、このヘッダは同じ値をまとめて多項式を掛ける。

## 厳密な定義

入力を $A=(A_1,\ldots,A_n)$、総和上限を $L$ とする。各 $0\le s\le L$ について

$$
E_s=\#\left\{I\subseteq\{1,\ldots,n\}\mid |I|\equiv0\pmod2,
\ \sum_{i\in I}A_i=s\right\},
$$

$$
O_s=\#\left\{I\subseteq\{1,\ldots,n\}\mid |I|\equiv1\pmod2,
\ \sum_{i\in I}A_i=s\right\}
$$

を法 `mod` で返す。返り値の `even[s]` が $E_s$、`odd[s]` が $O_s$ である。

内部では

$$
P_+(x)=\prod_{i=1}^n(1+x^{A_i}),\qquad
P_-(x)=\prod_{i=1}^n(1-x^{A_i})
$$

を $x^{L+1}$ で打ち切って計算し、

$$
E_s=[x^s]\frac{P_+(x)+P_-(x)}2,\qquad
O_s=[x^s]\frac{P_+(x)-P_-(x)}2
$$

を用いる。

## Include

```cpp
#include "dp/subsetsumcount.hpp"
```

## 使い方

```cpp
vector<int> values = {1, 2, 2};
int limit = 5;
auto count = poe::subset_sum_parity_counts<998244353>(values, limit);

cout << count.even[3] << '\n'; // 2: {1, 2}（2の選び方が2通り）
cout << count.odd[5] << '\n';  // 1: {1, 2, 2}
```

`values`は非負でなければならない。値が`limit`より大きい要素は、上限以下の和に寄与しないため無視される。
値0も使用できる。法`mod`は奇素数、`limit + 1 < mod`が必要である。

## 計算量

$d$を`limit`以下に現れる異なる正の値の個数、$c_v$を値$v$の個数、

$$
T=\sum_v\min(L,c_vv)
$$

とする。$M(k)$を長さ$k$の畳み込み時間として、時間計算量は
$O(n+M(T)\log d)$、領域計算量は$O(T)$。
特に値の種類が少ない場合や、対象とする総和までの要素総和が小さい場合に向く。

可否だけなら`dp/subsetsum.hpp`の`subsetsum`を使う。価値最大化や個数制約なら
`dp/knapsack.hpp`を使う。

## 使用例

[AtCoder ABC267 Ex - Odd Sum](https://atcoder.jp/contests/abc267/tasks/abc267_h)では、
`subset_sum_parity_counts<998244353>(A, M).odd[M]`が答えになる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `subsetsumparitycounts`

```cpp
template <int mod> struct subsetsumparitycounts
```

subset_sum_parity_countsの結果。even[s], odd[s]は和sを作る偶数個・奇数個の部分集合数。

### `subset_sum_parity_counts`

```cpp
template <int mod> subsetsumparitycounts<mod> subset_sum_parity_counts( const std::vector<int>& values, int limit )
```

O(n + M(T) log d)時間・O(T)領域。各和を作る部分集合数を選択個数の偶奇別に返す。
valuesは非負、0<=limitかつlimit+1<mod、modは奇素数であること。
dはlimit以下の異なる正の値の個数、T=sum_v min(limit,count[v]*v)。

<!-- END AUTO-GENERATED API REFERENCE -->
