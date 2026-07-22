---
title: "積の剰余を指定した列の数え上げ"
documentation_of: //cp/math/modularproduct.hpp
---

# 積の剰余を指定した列の数え上げ

## 概要

$0$以上$M$未満の整数を$K$個並べ、その積が$M$で割って指定値になる列の個数を求める。
$K$が巨大でも、$M$を素数冪へ分解して$p$進付値だけを数える。

## 厳密な定義

非負整数$K$、正整数$M$、$0\le N<M$に対し、

$$
0\le A_i<M,\qquad
\prod_{i=1}^{K}A_i\equiv N\pmod M
$$

を満たすordered tuple $(A_1,A_2,\ldots,A_K)$ の個数を返す。$K=0$では空積を$1$とする。

## Include

```cpp
#include "math/modularproduct.hpp"
```

```cpp
auto answer = count_modular_product_sequences<998244353>(
    length, target, modulus);
```

## 計算方法

$M=\prod p^q$を素数冪へ分解する。中国剰余定理により、各$p^q$に対する個数を掛ければよい。
目標値の$p$進付値を$r<q$とすると、一つの指定された目標値への列数は

$$
\varphi(p^q)^{K-1}{K+r-1\choose r}
$$

である。目標値が$0\pmod{p^q}$なら、全$(p^q)^K$列から積の付値が$q$未満の列を引く。
付値$r$を持つ剰余類の個数

$$
p^{q-r-1}(p-1)
$$

を整数として掛けるため、計算途中で$p$や$p-1$によるmodular divisionを行わない。

64bit整数の素因数分解に期待$O(M^{1/4})$程度を使う。分解後の計算は
$O(\sum q+\log K)$、使用領域は$O(\sum q)$である。各指数$q$は出力法`mod`より小さく、
`mod`は素数である必要がある。

## 頻出事故

- 列は順序を区別する。
- `target`は`modulus`未満である必要がある。
- $N=0$は付値$q$として扱い、非零の剰余とは式が異なる。
- $M=1$では剰余は$0$だけで、任意の正の$K$に対する答えは$1$である。
- $K=0$では空積$1$の剰余と`target`が一致するときだけ$1$を返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_modular_product_sequences`

```cpp
template<int mod> staticmodint<mod> count_modular_product_sequences( long long length, unsigned long long target, unsigned long long modulus )
```

期待O(M^(1/4)+omega(M)log M)。0<=a_i<Mかつ積a_i≡target(mod M)となる長さKの列を数える。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC245 Ex - Product Modulo 2](https://atcoder.jp/contests/abc245/tasks/abc245_h)は、
入力の$K,N,M$をそのまま`count_modular_product_sequences<998244353>`へ渡せる。
`verify/atcoder_abc245_h.cpp`に提出用コードを用意している。
