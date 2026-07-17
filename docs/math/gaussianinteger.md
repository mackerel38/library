---
title: "ガウス整数"
documentation_of: //cp/math/gaussianinteger.hpp
---

# ガウス整数

- Header: `cp/math/gaussianinteger.hpp`
- Symbol: `poe::gaussianinteger<T>`, `poe::gaussian_power`, `poe::gaussian_mod`
- Status: experimental

## どんな問題に使えるか

`a+bi`を`{a,b}`で持ち、加算・減算・乗算・共役・norm・二分累乗を行う。
二平方和、格子円、複素数を使う回転、ガウス整数上の素因数分解に使える。

```cpp
#include "math/gaussianinteger.hpp"

gaussianinteger<long long> z{1, 2};
auto power = gaussian_power(z, exponent);
auto residue = gaussian_mod(power, modulus);
```

各基本演算は`O(1)`、`gaussian_power`は`O(log exponent)`。
通常の整数型では乗算overflowに注意し、剰余計算では積ごとに`gaussian_mod`を適用する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `gaussianinteger`

```cpp
template <class T> struct gaussianinteger
```

ガウス整数・複素数型: gaussianinteger<T>{real, imaginary}; 四則の加減乗算O(1)。

### `conjugate`

```cpp
gaussianinteger conjugate() const
```

O(1)。複素共役を返す。

### `norm`

```cpp
T norm() const
```

O(1)。real^2+imaginary^2を返す。

### `gaussian_power`

```cpp
template <class T> gaussianinteger<T> gaussian_power( gaussianinteger<T> base, unsigned long long exponent )
```

O(log exponent)。ガウス整数baseの非負整数乗を返す。

### `gaussian_mod`

```cpp
template <std::integral T> gaussianinteger<T> gaussian_mod(gaussianinteger<T> value, T modulus)
```

O(1)。各成分を[0,modulus)へ正規化する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC444 G - Kyoen](https://atcoder.jp/contests/abc444/tasks/abc444_g)では、
`p=x^2+y^2`を`(x+yi)(x-yi)`へ分解し、法`C`で積の分布を追跡する。
`verify/atcoder_abc444_g.cpp`で使用する。judge ACは未確認。
