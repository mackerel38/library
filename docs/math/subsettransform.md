---
title: "部分集合変換"
documentation_of: //cp/math/subsettransform.hpp
---

# 部分集合変換

  `poe::superset_mobius`, `poe::subset_convolution`,
  `poe::subset_convolution_unit_power`, `poe::exact_match_counts`,
  `poe::divisibility_match_counts`

## 概要

状態がbitmaskで、全部分集合または全上位集合から値を集める問題に使う。包含関係に沿った一括集約、
SOS DP、包除の前処理に向く。配列長は2冪で、演算は加減算可能でなければならない。
`xor_transform`と`xor_convolution`は、二つのmaskをbitwise XORで合成する分布を扱う。
`subset_convolution(left, right)`は
`result[S] = sum_{T subset S} left[T] right[S-T]`を全`S`について返す。
`subset_convolution_unit_power(values, exponent)`は`values[0]=1`の列をsubset convolutionで
`exponent`乗する。指数は巨大でもよく、ranked zeta後の各多項式を微分関係で直接冪乗する。

## 厳密な定義

- `xor_transform`: 列$f$を$F_s=\sum_x(-1)^{\mathrm{popcount}(s\mathbin{\&}x)}f_x$へ変換する。
- `xor_convolution`: $C_k=\sum_{i\mathbin{\mathrm{xor}}j=k}A_iB_j$で定義される列$C$を返す。

- `subset_zeta`: O(n 2^n)。subset_zeta(values): values[S]をsum_{T subset S} values[T]へ変換する。
- `subset_mobius`: O(n 2^n)。subset_mobius(values): subset_zetaの逆変換を行う。
- `superset_zeta`: O(n 2^n)。superset_zeta(values): values[S]をsum_{T superset S} values[T]へ変換する。
- `superset_mobius`: O(n 2^n)。superset_mobius(values): superset_zetaの逆変換を行う。
- `subset_convolution`: O(n^2 2^n)。subset_convolution(left, right)[S]=sum_{T subset S} left[T]right[S-T]。
- `subset_convolution_unit_power`: O(n^2 2^n)。values[0]=1のsubset convolutionをexponent回行った列を返す。
- `exact_match_counts`: O(n 2^n)。all[S]=Sの全条件を満たす対象数から、ちょうどk条件を満たす対象数を返す。
- `divisibility_match_counts`: O(n 2^n)。1以上limit以下で、divisorsのうちちょうどk個の倍数である整数の個数を返す。

`exact_match_counts(all)`は、`all[S]`を「条件集合`S`をすべて満たす対象数」として、
「ちょうど`k`個の条件を満たす対象数」を全`k`について返す。個々の一致集合を提出コードで
包除する必要がない。

`divisibility_match_counts(limit, divisors)`は、1以上`limit`以下の整数について、
`divisors`のうちちょうど`k`個の倍数である個数を返す。LCMが`limit`を超える場合を
積の前に判定するため、入力範囲内でのoverflowを避ける。

## Include

```cpp
#include "math/subsettransform.hpp"

poe::subset_zeta(values);
// values[S] = sum_{T subset S} original[T]
poe::subset_mobius(values);

auto xor_product = poe::xor_convolution(left, right);

auto product = poe::subset_convolution(left, right);
auto power = poe::subset_convolution_unit_power(unit, exponent);

auto by_matches = poe::exact_match_counts(all_conditions);
auto by_divisors = poe::divisibility_match_counts(limit, divisors);
long long answer = by_divisors[required];
```

配列長は2冪でなければならない。ゼータ変換とメビウス変換は互いに逆変換である。

## 使い方

## 計算量

zeta・Möbius・Walsh--Hadamard変換とXOR畳み込みは`O(n 2^n)`時間。
部分集合畳み込みとunit powerは
`O(n^2 2^n)`時間、`O(n 2^n)`領域。`exact_match_counts`と
`divisibility_match_counts`は`O(n 2^n)`時間、`O(2^n)`領域。条件数を`n`とする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `xor_transform`

```cpp
template <class T> void xor_transform(std::vector<T>& values, bool inverse = false)
```

O(n 2^n)。xor_transform(values,inverse): Walsh-Hadamard変換または逆変換を行う。

### `xor_convolution`

```cpp
template <class T> std::vector<T> xor_convolution(std::vector<T> left, std::vector<T> right)
```

O(n 2^n)。result[k]=sum_(i xor j=k) left[i]right[j]を返す。

### `subset_zeta`

```cpp
template <class T> void subset_zeta(std::vector<T>& values)
```

O(n 2^n)。subset_zeta(values): values[S]をsum_{T subset S} values[T]へ変換する。

### `subset_mobius`

```cpp
template <class T> void subset_mobius(std::vector<T>& values)
```

O(n 2^n)。subset_mobius(values): subset_zetaの逆変換を行う。

### `superset_zeta`

```cpp
template <class T> void superset_zeta(std::vector<T>& values)
```

O(n 2^n)。superset_zeta(values): values[S]をsum_{T superset S} values[T]へ変換する。

### `superset_mobius`

```cpp
template <class T> void superset_mobius(std::vector<T>& values)
```

O(n 2^n)。superset_mobius(values): superset_zetaの逆変換を行う。

### `subset_convolution`

```cpp
template <class T> std::vector<T> subset_convolution( const std::vector<T>& left, const std::vector<T>& right)
```

O(n^2 2^n)。subset_convolution(left, right)[S]=sum_{T subset S} left[T]right[S-T]。

### `subset_convolution_unit_power`

```cpp
template <class T> std::vector<T> subset_convolution_unit_power( const std::vector<T>& values, long long exponent)
```

O(n^2 2^n)。values[0]=1のsubset convolutionをexponent回行った列を返す。

### `exact_match_counts`

```cpp
template <class T> std::vector<T> exact_match_counts(std::vector<T> all)
```

O(n 2^n)。all[S]=Sの全条件を満たす対象数から、ちょうどk条件を満たす対象数を返す。

### `divisibility_match_counts`

```cpp
inline std::vector<long long> divisibility_match_counts( long long limit, const std::vector<long long>& divisors)
```

O(n 2^n)。1以上limit以下で、divisorsのうちちょうどk個の倍数である整数の個数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[NDPC M - 数字](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_m)のSOS DPで利用できる。

```cpp
poe::subset_zeta(count);
// count[mask]から、maskの部分集合に対する総和を参照する。
```

`verify/atcoder_ndpc2026_m.cpp`で2026-07-15にjudge AC確認済み。
変換と逆変換のAPI testも実施済み。

[AtCoder ABC423 F - Loud Cicada](https://atcoder.jp/contests/abc423/tasks/abc423_f)では、
各周期を倍数条件として次のように使える。

```cpp
cout << divisibility_match_counts(y, periods)[m] << '\n';
```

`verify/atcoder_abc423_f.cpp`で公式sampleを確認済み。
小さい整数範囲を直接走査するproperty testも実施済み。
