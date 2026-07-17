---
title: "Power Projection"
documentation_of: //cp/fps/powerprojection.hpp
---

# Power Projection


## 概要

同じ長さ`n+1`の多項式`f`, `g`に対し、

## 厳密な定義

- `power_projection`: O(n log^2 n)。result[i]=[x^n]function(x)^i*multiplier(x)をi=0..nで返す。

`h[i] = [x^n] f(x)^i g(x)`

を`i=0..n`についてまとめて列挙する。冪を一つずつ掛ける`O(n^2 log n)`相当の処理を避け、
二変数Bostan–MoriとKronecker substitutionによる一次元畳み込みで高速化する。

## Include

```cpp
#include "fps/powerprojection.hpp"

poe::fps998244353 h = poe::power_projection(f, g);
```

計算量は`O(n log^2 n)`時間、`O(n)`領域。定数項`f[0]`が非0の場合も、
内部で定数項を分離して高速binomial transformを行う。`n < mod`かつNTT可能な法を使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `power_projection`

```cpp
template <int mod> fps<mod> power_projection(fps<mod> function, const fps<mod>& multiplier)
```

O(n log^2 n)。result[i]=[x^n]function(x)^i*multiplier(x)をi=0..nで返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC439 G - Sugoroku 6](https://atcoder.jp/contests/abc439/tasks/abc439_g)では、
一人が`i`回行動した後も未到達である確率を
`[x^(N-1)] D(x)^i (1+x+...+x^(N-1))`として一括計算する。
`verify/atcoder_abc439_g.cpp`に全体の使用例を収録する。

[AtCoder ABC387 G - Many LCS](https://atcoder.jp/contests/abc387/tasks/abc387_g)も
power projectionを利用できる代表例である。

property testでは定数項0・非0の双方について、小次数の愚直な多項式冪と全係数を比較する。
