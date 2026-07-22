---
title: "区間部分集合による全域被覆"
documentation_of: //cp/algorithm/intervalcover.hpp
---

# 区間部分集合による全域被覆

## 概要

区間を何本か選んで数直線上の全域を覆う方法を、選ぶ本数ごとに数えます。
さらに、区間を毎回一様ランダムに選んで塗り重ねるとき、全域が覆われるまでの期待回数も求められます。

## 厳密な定義

$m$ 個の半開区間 $I_i=[L_i,R_i)$ が与えられます。`interval_cover_subset_counts` は

$$
f_k=\#\left\{S\subseteq\{0,1,\ldots,m-1\}\ \middle|\
|S|=k,\ \bigcup_{i\in S}I_i=[0,n)\right\}
$$

を満たす列 $(f_0,f_1,\ldots,f_m)$ を返します。値は `mod` で割った余りです。

区間を $(L_i,R_i)$ の辞書順に処理し、選択済み区間の和集合が隙間のないprefix
$[0,j)$ である部分集合を、選択本数別にDPします。計算量は $O(nm^2)$ 時間、
$O(nm)$ 領域です。

`expected_random_interval_cover_time` は、各回に $m$ 区間から一つを独立かつ一様に選び、
その区間を被覆済みにする操作を繰り返したとき、$[0,n)$ 全体が覆われるまでの操作回数の
期待値を返します。すべての区間の和集合が $[0,n)$ である必要があります。

## Include

```cpp
#include "algorithm/intervalcover.hpp"
```

## 使い方

### `interval_cover_subset_counts`

```cpp
auto counts = interval_cover_subset_counts<998244353>(length, intervals);
```

`intervals` の各要素は0-indexed半開区間 `[left, right)` です。`counts[k]` が
$k$ 本で全域を覆う部分集合数です。`length == 0` かつ区間が空なら、空集合を一通り数えます。

### `expected_random_interval_cover_time`

```cpp
auto expected = expected_random_interval_cover_time<998244353>(length, intervals);
```

返り値は法上の期待値です。`intervals` は空でなく、その総和集合が全域を覆う必要があります。
また `mod` は素数で、`intervals.size() < mod` が必要です。既定の法は `998244353` です。

## 頻出事故

- 入力区間は閉区間ではなく半開区間です。1-indexed閉区間 $[L,R]$ は `[L-1, R)` に直します。
- 区間は入力順を区別します。同じ端点の区間が複数あれば、それぞれ別の選択肢です。
- `interval_cover_subset_counts` は全域を覆えない入力でも使え、その場合は全要素が $0$ です。
- 期待値版は同じ区間を何度でも引きます。区間を箱から取り除く模型ではありません。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `interval_cover_subset_counts`

```cpp
template<int mod = 998244353> std::vector<staticmodint<mod>> interval_cover_subset_counts( int length, std::vector<std::pair<int, int>> intervals )
```

O(nm^2)時間・O(nm)領域。区間部分集合が[0,n)を覆う個数を選択数別に返す。

### `expected_random_interval_cover_time`

```cpp
template<int mod = 998244353> staticmodint<mod> expected_random_interval_cover_time( int length, const std::vector<std::pair<int, int>>& intervals )
```

O(nm^2)時間・O(nm)領域。一様ランダムな区間で[0,n)を覆うまでの期待選択回数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC242 Ex - Random Painting](https://atcoder.jp/contests/abc242/tasks/abc242_h)では、
入力の閉区間 `[L, R]` を `[L-1, R)` に変換し、期待値APIへ直接渡せます。

```cpp
for (auto& [left, right] : intervals) {
    cin >> left >> right;
    --left;
}
cout << expected_random_interval_cover_time(n, intervals).val() << '\n';
```
