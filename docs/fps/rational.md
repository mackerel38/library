---
title: "有理型母関数"
documentation_of: //cp/fps/rational.hpp
---

# 有理型母関数

  `poe::weightedsumvariable`, `poe::weighted_sum_rational`, `poe::count_weighted_sum`, `poe::power_sums`

## 概要

母関数が`P(x)/Q(x)`で表せるとき、複数の有理式の和・積、先頭項展開、非常に遠い一係数を扱う。
`power_sums(values,n)`は`sum_i 1/(1-values[i]x)`を構築し、各値の0乗から`n-1`乗までの総和を返す。

## 厳密な定義

- `rationalfps`: rationalfps<mod>{p,q}: 有理型母関数p(x)/q(x)を表す。q[0]!=0。
- `rational_sum`: O(M(S) log k)。rational_sum(terms): 有理式列の和を分割統治で返す。空和は0。
- `rational_product`: O(M(S) log k)。rational_product(terms): 有理式列の積を分割統治で返す。空積は1。
- `weightedsumvariable`: weighted_sum_rationalへ渡す変数。weight*xを加え、limit<0ならx>=0、他は0<=x<=limit。
- `weighted_sum_rational`: O(M(S) log k)。独立な非負整数変数の重み付き和を数える母関数を制約から構築する。
- `count_weighted_sum`: O(M(S) log k + M(S) log target)。sum weight[i]*x[i]=targetとなる制約付き非負整数組を数える。
- `power_sums`: O(M(n) log n)。power_sums(values,n)[k]=sum value^kをk=0..n-1で返す。

## Include

```cpp
#include "fps/rational.hpp"

using mint = poe::modint998244353;
poe::rationalfps<998244353> f{{1}, {1, -1, -1}};
mint far = f.coeff(1000000000000000000LL);
mint subset_total = f.submask_sum(mask);
auto sums = poe::power_sums<998244353>(values, k + 1);
```

分母の定数項は非0であること。`coeff`はBostan–Mori法を使う。多数の分母を足すと分母次数も増えるため、
同じ分母を持つ項は先に分子をまとめる方がよい。
`submask_sum(mask)`は係数`a[i]`のうち`i`が`mask`のsubmaskであるものを
`O(M(k) log mask)`で合計する。分子に多項式部分がある場合も自動で分離して加える。

非負整数変数`x_i`について`sum weight_i*x_i=target`の解数を求める場合は、母関数を手で展開せず
変数の制約を直接渡せる。`limit`を省略すると上限なし、非負値を指定するとその値以下になる。

```cpp
// 0<=a<=1, 0<=b<=2, c,d>=0についてa+b+2c+3d=targetを数える。
mint answer = poe::count_weighted_sum<998244353>(
    target, {{1, 1}, {1, 2}, {2}, {3}}
);
```

`weighted_sum_rational`は同じ制約から有理型母関数自体を返すため、複数係数を使う場合にも再利用できる。
有限上限の因子は`1+x^w+...+x^(limit*w)`、上限なしは`1/(1-x^w)`として構築する。
全因子の分子・分母次数の和を`S`、変数数を`k`として構築は`O(M(S) log k)`が目安であり、
巨大な重みや有限上限を
密な多項式として作る用途には向かない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rationalfps`

```cpp
template<int mod> struct rationalfps
```

rationalfps<mod>{p,q}: 有理型母関数p(x)/q(x)を表す。q[0]!=0。

### `operator*`

```cpp
friend rationalfps operator*(const rationalfps& left, const rationalfps& right)
```

O(M(n))。left * right: 有理式の積を返す。

### `operator+`

```cpp
friend rationalfps operator+(const rationalfps& left, const rationalfps& right)
```

O(M(n))。left + right: 有理式の和を返す。

### `prefix`

```cpp
fps<mod> prefix(int size) const
```

O(M(n) log n)。f.prefix(n): 有理式をx^n未満のFPSへ展開する。

### `coeff`

```cpp
staticmodint<mod> coeff(long long index) const
```

O(M(k) log index)。f.coeff(index): 有理式のx^indexの係数を返す。

### `submask_sum`

```cpp
staticmodint<mod> submask_sum(unsigned long long mask) const
```

O(M(k) log mask)。係数a[i]についてiがmaskのsubmaskであるa[i]の総和を返す。

### `rational_sum`

```cpp
template<int mod> rationalfps<mod> rational_sum(std::vector<rationalfps<mod>> terms)
```

O(M(S) log k)。rational_sum(terms): 有理式列の和を分割統治で返す。空和は0。

### `rational_product`

```cpp
template<int mod> rationalfps<mod> rational_product(std::vector<rationalfps<mod>> terms)
```

O(M(S) log k)。rational_product(terms): 有理式列の積を分割統治で返す。空積は1。

### `weightedsumvariable`

```cpp
struct weightedsumvariable
```

weighted_sum_rationalへ渡す変数。weight*xを加え、limit<0ならx>=0、他は0<=x<=limit。

### `weighted_sum_rational`

```cpp
template<int mod> rationalfps<mod> weighted_sum_rational(const std::vector<weightedsumvariable>& variables)
```

O(M(S) log k)。独立な非負整数変数の重み付き和を数える母関数を制約から構築する。

### `count_weighted_sum`

```cpp
template<int mod> staticmodint<mod> count_weighted_sum( long long target, const std::vector<weightedsumvariable>& variables )
```

O(M(S) log k + M(S) log target)。sum weight[i]*x[i]=targetとなる制約付き非負整数組を数える。

### `power_sums`

```cpp
template<int mod> fps<mod> power_sums(const std::vector<staticmodint<mod>>& values, int size)
```

O(M(n) log n)。power_sums(values,n)[k]=sum value^kをk=0..n-1で返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [FPS 24題 B - 整数の組](https://atcoder.jp/contests/fps-24/tasks/fps_24_b):
  重みと上限だけを`count_weighted_sum`へ渡し、有理型母関数の構築と遠い係数取得をまとめて行う。
- [FPS 24題 Q - サイコロ](https://atcoder.jp/contests/fps-24/tasks/fps_24_q): 冪和列挙。
- [FPS 24題 T - カラフル](https://atcoder.jp/contests/fps-24/tasks/fps_24_t): 有理式の構築と遠い係数。
- [AtCoder ABC300 Ex - Fibonacci: Revisited](https://atcoder.jp/contests/abc300/tasks/abc300_h):
  K-bonacci母関数のsubmask係数和。

小次数の直接展開と比較するproperty testを行う。
