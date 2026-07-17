---
title: "約数・倍数変換"
documentation_of: //cp/math/divisortransform.hpp
---

# 約数・倍数変換


## 概要

添字`1..n`が整数で、約数全体または倍数全体から値を集約するDP・数え上げに使う。
gcd/lcmに関する組の数え上げや、約数包除を一括処理するときに有効である。

## 厳密な定義

- `divisor_zeta`: O(n log log n)。divisor_zeta(a): a[x]をsum_{d|x} original[d]へ変換する。a[0]は不使用。
- `divisor_mobius`: O(n log log n)。divisor_mobius(a): divisor_zetaの逆変換を行う。a[0]は不使用。
- `multiple_zeta`: O(n log log n)。multiple_zeta(a): a[x]をsum_{x|m} original[m]へ変換する。a[0]は不使用。
- `multiple_mobius`: O(n log log n)。multiple_mobius(a): multiple_zetaの逆変換を行う。a[0]は不使用。

演算が通常の加減算でない場合や、添字集合が`1..n`でない場合はそのまま使えない。
`values[0]`は常に不使用である。

## Include

```cpp
#include "math/divisortransform.hpp"

poe::divisor_zeta(values);   // values[x] = sum_{d|x} original[d]
poe::divisor_mobius(values); // 元へ戻す
```

各操作`O(n log log n)`時間、素数列のため`O(n)`追加領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `divisor_zeta`

```cpp
template <class T> void divisor_zeta(std::vector<T>& values)
```

O(n log log n)。divisor_zeta(a): a[x]をsum_{d|x} original[d]へ変換する。a[0]は不使用。

### `divisor_mobius`

```cpp
template <class T> void divisor_mobius(std::vector<T>& values)
```

O(n log log n)。divisor_mobius(a): divisor_zetaの逆変換を行う。a[0]は不使用。

### `multiple_zeta`

```cpp
template <class T> void multiple_zeta(std::vector<T>& values)
```

O(n log log n)。multiple_zeta(a): a[x]をsum_{x|m} original[m]へ変換する。a[0]は不使用。

### `multiple_mobius`

```cpp
template <class T> void multiple_mobius(std::vector<T>& values)
```

O(n log log n)。multiple_mobius(a): multiple_zetaの逆変換を行う。a[0]は不使用。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[NDPC L - 最小公倍数](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_l)は約数ゼータ変換の発想を使う。
問題固有の状態設計は利用側に残る。
`verify/atcoder_ndpc2026_l.cpp`で2026-07-15にjudge AC確認済み。愚直な約数和・逆変換とも照合済み。
