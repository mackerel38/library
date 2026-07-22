---
title: "対称群に対するBurnsideの補題"
documentation_of: //cp/math/burnside.hpp
---

# 対称群に対するBurnsideの補題

## 概要

$n$個の対象の番号を自由に付け替えたものを同一視して、異なる配置の個数を数えます。
$n!$個の置換を直接調べず、同じcycle構造を持つ置換をまとめるため、$n\leq30$程度でも扱えます。

## 厳密な定義

対称群 $S_n$ が有限集合 $X$ に作用しているとします。長さ $l$ のcycleを $m_l$ 個持つ置換の固定点数を
$F(m_1,m_2,\ldots,m_n)$ とすると、軌道数

$$
\frac{1}{n!}\sum_{\sigma\in S_n}|\operatorname{Fix}(\sigma)|
$$

を返します。cycle typeごとにまとめると、これは

$$
\sum_{\sum_l lm_l=n}
\frac{F(m_1,m_2,\ldots,m_n)}
{\prod_l l^{m_l}m_l!}
$$

です。

## Include

```cpp
#include "math/burnside.hpp"
```

## API

```cpp
template<class mint, class FixedCount>
mint symmetric_group_orbit_count(int size, FixedCount fixed_count);
```

`fixed_count(cycle_counts)`には、`cycle_counts[l]`個の長さ$l$のcycleを持つ置換による固定点数を返す関数を渡します。
呼び出し中だけ参照できる`vector<int>`として渡されます。

- $p(n)$を$n$の分割数として、callback以外に $O(p(n)n)$ 時間を使います。
- 追加メモリは $O(n)$ です。
- `mint`の法の下で $1,2,\ldots,n$が可逆である必要があります。

## 使用例

3個の区別できない場所を0または1で塗る方法は、1の個数だけで決まるため4通りです。

```cpp
using mint = staticmodint<1000000007>;
mint answer = symmetric_group_orbit_count<mint>(3, [](const vector<int>& count) {
    int cycles = accumulate(count.begin(), count.end(), 0);
    return mint{2}.pow(cycles);
});
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `symmetric_group_orbit_count`

```cpp
template<class mint, class FixedCount> mint symmetric_group_orbit_count(int size, FixedCount fixed_count)
```

O(p(n)・n)。S_nの置換のcycle個数列から固定点数を返すfixed_countを用いて軌道数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
