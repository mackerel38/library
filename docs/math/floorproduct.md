---
title: "Floor Sumのモノイド積版"
documentation_of: //cp/math/floorproduct.hpp
---

# Floor Sumのモノイド積版

- Header: `cp/math/floorproduct.hpp`
- Symbol: `poe::monoid_power`, `poe::powersummonoid`, `poe::floor_product`
- Status: experimental

## どんな問題に使えるか

通常のFloor Sumが格子境界の下側面積を加算するのに対し、`floor_product`は
`floor((a*i+b)/m)`が描く格子経路に沿って任意のモノイド元`x`,`y`を掛ける。
ユークリッド互除法と同じ反転を行い、巨大な`n,m,a,b`を対数時間で処理する。

```cpp
#include "math/floorproduct.hpp"

using monoid = poe::powersummonoid<mint>;
auto result = poe::floor_product(n, m, a, b, monoid::x(A), monoid::y(B));
mint answer = result.answer();
```

`powersummonoid`を使うと
`sum_{i=0}^{n-1} A^i B^floor((a*i+b)/m)`を得る。独自モノイドは
`identity()`、`operator*=`を用意する。`monoid_power`と`floor_product`は
`O(log(n+m+a+b))`回のモノイド積、再帰深さ`O(log)`。引数は非負整数で`m>0`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `monoid_power`

```cpp
template<class Monoid, std::unsigned_integral Integer> Monoid monoid_power(Monoid value, Integer exponent)
```

O(log exponent)。Monoid::identity()を単位元としてvalue^exponentを返す。

### `powersummonoid`

```cpp
template<class T> struct powersummonoid
```

powersummonoid<T>::x(a), y(b)をfloor_productへ渡すとsum a^i*b^floorを得る。

### `identity`

```cpp
static powersummonoid identity()
```

O(1)。積の単位元を返す。

### `x`

```cpp
static powersummonoid x(const T& ratio)
```

O(1)。添字を1進め、現在項を和へ加える元を返す。

### `y`

```cpp
static powersummonoid y(const T& ratio)
```

O(1)。floor値を1進める元を返す。

### `operator*=`

```cpp
powersummonoid& operator*=(const powersummonoid& right)
```

O(1)。処理列を連結する。

### `answer`

```cpp
const T& answer() const noexcept
```

O(1)。累積和を返す。

### `floor_product`

```cpp
template<class Monoid, std::unsigned_integral Integer> Monoid floor_product(Integer n, Integer m, Integer a, Integer b, Monoid x, Monoid y)
```

O(log(n+m+a+b))。格子境界floor((a*i+b)/m)に沿うx,yのモノイド積を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC429 G - Sum of Pow of Mod of Linear](https://atcoder.jp/contests/abc429/tasks/abc429_g)の
互いに素な法の部分で使用する。合成数法全体の処理は`math/modularsum.hpp`が包む。
公式sample確認済み。

## 検証

- `tests/property/math/floorproduct.cpp`: 小さい引数を定義式の愚直和と比較
- `tests/api/math/floorproduct.cpp`: 基本呼び出し
