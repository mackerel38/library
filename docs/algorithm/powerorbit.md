---
title: "有限monoid上の冪の周期"
documentation_of: //cp/algorithm/powerorbit.hpp
---

# 有限monoid上の冪の周期


## 概要

有限個の状態しか持たないmonoidで`1, base, base^2, ...`を生成し、前周期と周期へ圧縮する。
巨大指数の剰余、行列や写像の小さい有限状態、指数区間に現れる状態の頻度計算に使える。

## 厳密な定義

- `finitepowerorbit`: 有限monoidのbase^k列: finitepowerorbit orbit(one, base, multiply); 構築O(s log s)、参照O(1)。

## Include

```cpp
#include "algorithm/powerorbit.hpp"

finitepowerorbit orbit(1, 2, [](int x, int y) { return x * y % 8; });
int value = orbit[1'000'000'000'000ULL];
```

相異なる状態数を`s`として構築`O(s log s)`時間・`O(s)`領域、冪の参照`O(1)`。
`State`には比較演算、`multiply`には有限集合内で閉じた結合的な積が必要。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `finitepowerorbit`

```cpp
template <class State> struct finitepowerorbit
```

有限monoidのbase^k列: finitepowerorbit orbit(one, base, multiply); 構築O(s log s)、参照O(1)。

### `finitepowerorbit`

```cpp
template <class Multiply> finitepowerorbit(State one, const State& base, Multiply multiply)
```

O(s log s)。初めて同じ状態へ戻るまで冪を列挙し、前周期と周期を求める。

### `operator[]`

```cpp
const State& operator[](unsigned long long exponent) const
```

O(1)。base^exponentを返す。

### `preperiod`

```cpp
int preperiod() const noexcept
```

O(1)。周期へ入る最初の指数を返す。

### `period`

```cpp
int period() const noexcept
```

O(1)。周期の長さを返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。保持している相異なる状態数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC444 G - Kyoen](https://atcoder.jp/contests/abc444/tasks/abc444_g)では、
法`C`のガウス整数環における素数の冪を周期化し、最大`10^18`の指数分布を数える。
`verify/atcoder_abc444_g.cpp`で使用する。
