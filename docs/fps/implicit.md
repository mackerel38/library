---
title: "FPSの陰関数"
documentation_of: //cp/fps/implicit.hpp
---

# FPSの陰関数


## 概要

未知の形式的冪級数`y`が`F(y)=0`を満たすとき、方程式と`y`に関する形式微分を渡して
解をNewton法で求める。木の再帰的な構造、合成的クラス、自己参照する母関数を
問題ごとにNewton更新から書き直さずに済む。

## 厳密な定義

- `newton_fps`: O((E(n)+M(n)) log n)。F(y)=0をNewton法で解き、y mod x^nを返す。 equation(y,n)はpair{F(y), dF/dy(y)}をそれぞれn項返す。constantはF(constant)=0 mod xを満たすこと。

## Include

```cpp
#include "fps/implicit.hpp"

using fps = poe::fps998244353;
fps y = poe::newton_fps<998244353>(n, 0, [&](const fps& y, int size) {
    // y = x exp(y) を解く。
    fps rhs(size);
    fps exponential = y.exp(size - 1);
    for (int i = 1; i < size; ++i) rhs[i] = exponential[i - 1];
    fps derivative = rhs;
    derivative[0] -= 1; // d(x exp(y)-y)/dy
    return std::pair{rhs - y, derivative};
});
```

`equation(y,size)`は`pair{F(y), dF/dy}`を返す。定数項`constant`は
`F(constant)=0 mod x`を満たし、導関数の定数項が非0でなければならない。
方程式評価を`E(n)`、畳み込みを`M(n)`とすると計算量は`O((E(n)+M(n)) log n)`、領域`O(n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `newton_fps`

```cpp
template<int mod, class Equation> fps<mod> newton_fps(int size, staticmodint<mod> constant, Equation equation)
```

O((E(n)+M(n)) log n)。F(y)=0をNewton法で解き、y mod x^nを返す。
equation(y,n)はpair{F(y), dF/dy(y)}をそれぞれn項返す。constantはF(constant)=0 mod xを満たすこと。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[FPS 24題 S - 根付き木](https://atcoder.jp/contests/fps-24/tasks/fps_24_s)のtype 1で、
勝敗条件を満たす根付き木の母関数`A=x exp(T-A)`として使う。
