---
title: "整数分割と一般五角数定理"
documentation_of: //cp/fps/partition.hpp
---

# 整数分割と一般五角数定理

## 概要

整数を正整数の和へ分ける方法の個数と、Euler積に現れる疎な係数を扱う。
小さい次数までの全分割数だけでなく、一般五角数定理を使って巨大な一つの次数を調べる用途にも使える。

## 厳密な定義

Euler積を

$$
E(x)=\prod_{i=1}^{\infty}(1-x^i)
$$

とする。一般五角数定理より

$$
E(x)=\sum_{k=-\infty}^{\infty}(-1)^k
x^{k(3k-1)/2}
$$

であり、非零係数が現れる次数は一般五角数だけである。

`generalized_pentagonal_terms(n)`は$E(x)$の$n$次以下の非零項を返す。
`partition_numbers<T>(n)[s]`は

$$
p(s)=[x^s]\frac1{E(x)}
$$

で定義される整数分割数を返す。

`euler_product_over_one_minus_x_coefficient<p>(d,a)`は、素数$p$を法として

$$
[x^d]\frac{E(x)}{(1-x)^a}
$$

を返す。$d,a$は巨大でもよいが、Lucasの定理の前計算に$O(p)$を使う。

## Include

```cpp
#include "fps/partition.hpp"
```

## APIの選び方

| 条件 | API | 出力 |
|---|---|---|
| Euler積の疎な非零項がほしい | `generalized_pentagonal_terms(n)` | `(次数, 係数)`の列 |
| $0$から$n$までの分割数がほしい | `partition_numbers<T>(n)` | $p(0),\ldots,p(n)$ |
| 巨大次数の$E(x)/(1-x)^a$の係数がほしい | `euler_product_over_one_minus_x_coefficient<p>(d,a)` | $[x^d]E(x)/(1-x)^a$ |

```cpp
using mint = staticmodint<998244353>;
auto partitions = partition_numbers<mint>(n);

auto coefficient =
    euler_product_over_one_minus_x_coefficient<200003>(degree, power);
```

## 計算量

- `generalized_pentagonal_terms(n)`: $O(\sqrt n)$ 時間・出力領域
- `partition_numbers<T>(n)`: $O(n\sqrt n)$ 時間、$O(n)$ 領域
- `euler_product_over_one_minus_x_coefficient<p>(d,a)`:
  $O(p+\sqrt d\log_p(d+a))$ 時間、$O(p)$ 領域

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `pentagonalterm`

```cpp
struct pentagonalterm
```

Euler積prod(1-x^i)の非零項x^degreeの係数を表す。

### `generalized_pentagonal_terms`

```cpp
inline std::vector<pentagonalterm> generalized_pentagonal_terms( long long maximum_degree )
```

O(sqrt n)。prod_{i>=1}(1-x^i)のn次以下の非零項を次数順に返す。

### `partition_numbers`

```cpp
template <class T> std::vector<T> partition_numbers(int maximum_sum)
```

O(n sqrt n)。0<=s<=nの整数分割数p(s)を返す。answer[0]=1。

### `euler_product_over_one_minus_x_coefficient`

```cpp
template <int prime> staticmodint<prime> euler_product_over_one_minus_x_coefficient( long long degree, long long denominator_power )
```

O(p+sqrt d log_p(d+a))。素数pを法として[x^d]prod(1-x^i)/(1-x)^aを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC279 Ex - Sum of Prod of Min](https://atcoder.jp/contests/abc279/tasks/abc279_h)では、
入力を`n,m`として答えが

$$
[x^{m-n}]\frac{E(x)}{(1-x)^{2n}}
$$

になるため、次の一行で係数を求められる。

```cpp
auto answer =
    euler_product_over_one_minus_x_coefficient<200003>(m - n, 2 * n);
```

提出用コードは`verify/atcoder_abc279_h.cpp`にあり、公式sample 3件を確認済み。
