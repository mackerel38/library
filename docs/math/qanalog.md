---
title: "q整数・Gaussian二項係数と有限体の行空間計数"
documentation_of: //cp/math/qanalog.hpp
---

# q整数・Gaussian二項係数と有限体の行空間計数

## 概要

通常の整数・階乗・二項係数を$q$類似へ置き換えた量を前計算する。
有限体上の部分空間数や、指定したベクトルを張れない行列の個数を数えるときに使う。

## 厳密な定義

$q$整数、$q$階乗、Gaussian二項係数をそれぞれ

$$
[n]_q=1+q+\cdots+q^{n-1},
$$

$$
[n]_q!=\prod_{i=1}^n[i]_q,
$$

$$
{n\brack k}_q=\frac{[n]_q!}{[k]_q![n-k]_q!}
$$

と定義する。有限体$\mathbb F_q$上の$n$次元空間に含まれる$k$次元部分空間の個数は
${n\brack k}_q$である。

`matrix_rowspace_avoiding_vector_counts`は、固定した非零ベクトル$v\in\mathbb F_q^D$
に対し、$s$行$D$列行列のうち$v$を行空間に含まないものの個数を、$0\le s\le R$について返す。

## Include

```cpp
#include "math/qanalog.hpp"
```

## Gaussian二項係数

```cpp
using mint = staticmodint<998244353>;
qcombination<mint> combination(limit, mint{2});

mint q_integer = combination.integer(n);
mint q_factorial = combination.factorial(n);
mint subspaces = combination.choose(n, k);
```

`limit`以下の$q$整数が係数の法で可逆でなければならない。
構築は`O(limit)`時間・領域、各取得は`O(1)`。

## 固定ベクトルを含まない行空間

```cpp
auto counts = matrix_rowspace_avoiding_vector_counts<998244353>(
    maximum_rows, dimension, field_size);
```

`counts[s]`は$\mathbb F_q$上の$s\times D$行列で、行空間が固定した非零ベクトルを
含まないものの個数である。非零ベクトルの選び方によって答えは変わらない。
`field_size`は素数冪であり、必要な$q$階乗が係数の法で可逆でなければならない。

計算量は`R=maximum_rows`、`D=dimension`として
$O(D+R+M(R))$時間、$O(D+R)$領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `qcombination`

```cpp
template <class T> struct qcombination
```

構築O(n)、取得O(1)。q整数・q階乗・Gaussian二項係数を前計算する。

### `qcombination`

```cpp
qcombination(int size, T q)
```

O(n)時間・O(n)領域。0<=k<=nのq階乗と逆q階乗を前計算する。

### `integer`

```cpp
T integer(int n) const
```

O(1)。q整数[n]_q=1+q+...+q^(n-1)を返す。1<=n<=前計算上限。

### `factorial`

```cpp
T factorial(int n) const
```

O(1)。q階乗[n]_q!を返す。0<=n<=前計算上限。

### `inverse_factorial`

```cpp
T inverse_factorial(int n) const
```

O(1)。q階乗の逆数1/[n]_q!を返す。0<=n<=前計算上限。

### `choose`

```cpp
T choose(int n, int k) const
```

O(1)。Gaussian二項係数[n choose k]_qを返す。範囲外では0。

### `matrix_rowspace_avoiding_vector_counts`

```cpp
template <int mod> std::vector<staticmodint<mod>> matrix_rowspace_avoiding_vector_counts( int maximum_rows, int dimension, int field_size )
```

O(D+R+M(R))。F_q上のs行D列行列で行空間が固定非零vectorを含まない個数を全0<=s<=Rで返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC278 Ex - make 1](https://atcoder.jp/contests/abc278/tasks/abc278_h)では、
$q=2$、$D=B$とし、xorで1を作れない反復可能な行列列を行数ごとにまとめて求める。

```cpp
auto repeated =
    matrix_rowspace_avoiding_vector_counts<998244353>(n, bit_count, 2);
```

提出用コードは`verify/atcoder_abc278_h.cpp`にあり、公式sample 3件を確認済み。
