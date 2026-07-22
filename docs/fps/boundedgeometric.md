---
title: "上限付き等比級数積の巨大次数係数"
documentation_of: //cp/fps/boundedgeometric.hpp
---

# 上限付き等比級数積の巨大次数係数

## 概要

各種類を選べる個数に上限があり、選んだ個数だけ値を掛ける多重集合型の母関数について、
非常に遠い一つの係数だけを求めます。種類数が小さく、求める次数や上限が巨大な場合に使います。

## 厳密な定義

素数法上で互いに異なる非零の値 $a_0,a_1,\ldots,a_{n-1}$ と、非負整数
$b_0,b_1,\ldots,b_{n-1}$ に対して

$$
F(x)=\prod_{i=0}^{n-1}\left(\sum_{k=0}^{b_i}(a_ix)^k\right)
$$

と定義します。`bounded_geometric_product_coefficient(values, limits, degree)` は
$[x^d]F(x)$ を返します。

各因子を

$$
\sum_{k=0}^{b_i}(a_ix)^k=\frac{1-(a_ix)^{b_i+1}}{1-a_ix}
$$

と変形します。分子は部分集合を列挙し、分母
$\prod_i(1-a_ix)^{-1}$ は、$a_i$ が相異なることを使って一次分数の和へ分解します。
時間計算量は $O(n^2\log B+n2^n)$、領域計算量は $O(2^n)$ です。

## Include

```cpp
#include "fps/boundedgeometric.hpp"
```

## 使い方

```cpp
using mint = modint998244353;
vector<mint> values = {3, 5, 6};
vector<long long> limits = {1, 2, 3};
mint coefficient = bounded_geometric_product_coefficient(values, limits, 3);
```

```cpp
template<int mod>
staticmodint<mod> bounded_geometric_product_coefficient(
    const vector<staticmodint<mod>>& values,
    const vector<long long>& limits,
    long long degree);
```

- `values[i]`: $a_i$。すべて非零かつ互いに異なる必要があります。
- `limits[i]`: $b_i$。$0$ 以上でなければなりません。
- `degree`: 求める次数 $d$。$0$ 以上でなければなりません。
- 種類数は24以下です。空積の0次係数は1、それ以外は0です。
- `mod` は素数でなければなりません。

## 選び方

| 条件 | API |
| --- | --- |
| 種類数が小さく、上限と次数が巨大、$a_i$が相異なる | このheader |
| 分子・分母の次数が実際に小さい一般の有理型母関数 | [`fps/rational.hpp`](rational.md) |
| 低次数までの係数をすべて求めたい | [`fps/fps.hpp`](fps.md) |

## 頻出事故

- 同じ値を持つ種類をそのまま二つ渡せません。一次分数分解の分母が0になります。
- 同じ値のカードを区別して選ぶ二項係数型ではありません。各種類から選ぶ枚数だけを区別します。
- $b_i$ 枚から $k$ 枚選ぶ重みは $a_i^k$ であり、$\binom{b_i}{k}a_i^k$ ではありません。
- $n2^n$ が支配的なので、種類数が大きい問題には向きません。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `bounded_geometric_product_coefficient`

```cpp
template<int mod> staticmodint<mod> bounded_geometric_product_coefficient( const std::vector<staticmodint<mod>>& values, const std::vector<long long>& limits, long long degree )
```

O(n^2 log B+n2^n)時間・O(2^n)領域。prod_i sum_{k=0}^{limit[i]}(value[i]x)^kの指定係数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC241 Ex - Card Deck Score](https://atcoder.jp/contests/abc241/tasks/abc241_h)では、
値 $A_i$ のカードを $0$ 枚から $B_i$ 枚まで選べます。同じ値のカードは区別しないため、
全選び方のscore和は

$$
[x^M]\prod_i\sum_{k=0}^{B_i}(A_ix)^k
$$

そのものです。`A`、`B`、`M` をAPIへ直接渡せます。
