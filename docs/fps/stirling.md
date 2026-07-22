---
title: "Stirling数と連続一次因子積"
documentation_of: //cp/fps/stirling.hpp
---

# Stirling数と連続一次因子積

## 概要

第一種・第二種Stirling数の固定した一行を高速に求める。
相異なるものを選ぶ列と重複を許す列の変換、置換のcycle数、集合分割の個数に使える。

## 厳密な定義

下降階乗を

$$
(x)_n=x(x-1)\cdots(x-n+1)
$$

とする。符号付き第一種Stirling数$s(n,k)$は

$$
(x)_n=\sum_{k=0}^n s(n,k)x^k
$$

で定義される。

第二種Stirling数$S(n,k)$は、$n$個の区別される要素を$k$個の空でない区別されない集合へ
分割する方法の個数である。

## Include

```cpp
#include "fps/stirling.hpp"
```

## APIの選び方

| 条件 | API | 出力 |
|---|---|---|
| 連続する一次因子の積がほしい | `falling_factorial_polynomial<p>(n)` | $(x)_n$ |
| 固定$n$の第一種Stirling数がほしい | `signed_stirling_first_row<p>(n)` | $s(n,0),\ldots,s(n,n)$ |
| 固定$n$の第二種Stirling数がほしい | `stirling_second_row<p>(n)` | $S(n,0),\ldots,S(n,n)$ |

```cpp
auto first = signed_stirling_first_row<998244353>(n);
auto second = stirling_second_row<998244353>(n);
```

第一種は$(x)_n$を半分ずつ構築し、Taylor shiftで後半の因子を得る。
第二種は

$$
S(n,k)=\sum_{i=0}^k\frac{(-1)^{k-i}i^n}{i!(k-i)!}
$$

を畳み込みとして計算する。`0<=n<mod`が必要。

- `falling_factorial_polynomial`、`signed_stirling_first_row`: $O(M(n))$
- `stirling_second_row`: $O(M(n)+n\log n)$
- 領域: $O(n)$

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `falling_factorial_polynomial`

```cpp
template <int mod> fps<mod> falling_factorial_polynomial(int size)
```

O(M(n))。falling_factorial_polynomial(n)=prod_{i=0}^{n-1}(x-i)を返す。

### `signed_stirling_first_row`

```cpp
template <int mod> fps<mod> signed_stirling_first_row(int n)
```

O(M(n))。符号付き第一種Stirling数s(n,k)をk=0..nの順に返す。

### `stirling_second_row`

```cpp
template <int mod> fps<mod> stirling_second_row(int n)
```

O(M(n)+n log n)。第二種Stirling数S(n,k)をk=0..nの順に返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Stirling Number of the First Kind](https://judge.yosupo.jp/problem/stirling_number_of_the_first_kind)と
[Stirling Number of the Second Kind](https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind)の出力列に対応する。

[AtCoder ABC278 Ex - make 1](https://atcoder.jp/contests/abc278/tasks/abc278_h)では、重複を許して選んだ列の個数
$G(s)$から相異なるカードだけを選んだ列の個数$F(s)$へ

$$
F(s)=\sum_{t=0}^s s(s,t)G(t)
$$

で変換する。提出用コードは`verify/atcoder_abc278_h.cpp`にあり、公式sample 3件を確認済み。
