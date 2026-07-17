---
title: "Dirichlet畳み込みと積necklace"
documentation_of: //cp/math/dirichlet.hpp
---

# Dirichlet畳み込みと積necklace

  `poe::prime_omega_table`, `poe::dirichlet_log`, `poe::product_necklace_counts`

## 概要

添字の積に対して状態を合成する数列を扱う。通常の畳み込みが`i+j=n`を集めるのに対し、
Dirichlet畳み込みは`i*j=n`を集める。約数DP、乗法的な列の形式的な逆元・対数、要素の積で
分類する列や巡回列の数え上げに使える。

## 厳密な定義

- `dirichlet_convolution`: O(n log n)。添字1始まりの列left,rightのDirichlet畳み込みをn未満で返す。
- `dirichlet_inverse`: O(n log n)。values[1]が可逆な列のDirichlet畳み込み逆元を返す。添字0は使わない。
- `prime_omega_table`: O(n)。omega[n]=nの素因数の重複込み個数となる表を返す。
- `dirichlet_log`: O(n log n)。values[1]=1の形式Dirichlet級数についてlog(values)を返す。
- `product_necklace_counts`: O(u log u)。frequency[w]種類の重みwを使う有向necklaceを、積ごとに数える。frequency[1]=0。

## Include

```cpp
#include "math/dirichlet.hpp"

vector<mint> frequency(u + 1);
for (int beauty : beauty_values) ++frequency[beauty];
auto answer = poe::product_necklace_counts(frequency);
```

列は添字1始まりで、添字0を使わない。`dirichlet_convolution`の二列は同じ長さとする。
`dirichlet_inverse`は`values[1]`が可逆であること、`dirichlet_log`は`values[1]=1`であることが必要。
`product_necklace_counts`は`frequency[w]`種類の区別される要素が重み`w`を持つとき、回転を同一視し
反転を同一視しない非空necklaceを積ごとに返す。有限個にするため`frequency[1]=0`が必要である。

各主要操作は`O(u log u)`時間、`O(u)`追加領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `dirichlet_convolution`

```cpp
template<class T> std::vector<T> dirichlet_convolution(const std::vector<T>& left, const std::vector<T>& right)
```

O(n log n)。添字1始まりの列left,rightのDirichlet畳み込みをn未満で返す。

### `dirichlet_inverse`

```cpp
template<class T> std::vector<T> dirichlet_inverse(const std::vector<T>& values)
```

O(n log n)。values[1]が可逆な列のDirichlet畳み込み逆元を返す。添字0は使わない。

### `prime_omega_table`

```cpp
inline std::vector<int> prime_omega_table(int limit)
```

O(n)。omega[n]=nの素因数の重複込み個数となる表を返す。

### `dirichlet_log`

```cpp
template<class T> std::vector<T> dirichlet_log(const std::vector<T>& values)
```

O(n log n)。values[1]=1の形式Dirichlet級数についてlog(values)を返す。

### `product_necklace_counts`

```cpp
template<class T> std::vector<T> product_necklace_counts(const std::vector<T>& frequency)
```

O(u log u)。frequency[w]種類の重みwを使う有向necklaceを、積ごとに数える。frequency[1]=0。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC428 G - Necklace](https://atcoder.jp/contests/abc428/tasks/abc428_g)で、
各宝石の美しさを重み、同じ美しさを持つ宝石数を`frequency`として使う。提出用コードでは
頻度を作って`product_necklace_counts`を一度呼ぶだけでよい。公式sample確認済み。

## 検証

- `tests/api/math/dirichlet.cpp`: 基本APIとABC428 Gのsample相当
- `tests/property/math/dirichlet.cpp`: 畳み込み逆元・形式対数を愚直式と比較し、積necklaceを全列挙と比較
