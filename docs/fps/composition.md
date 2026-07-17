---
title: "FPSの合成"
documentation_of: //cp/fps/composition.hpp
---

# FPSの合成

- Header: `cp/fps/composition.hpp`
- Symbol: `poe::compose`, `poe::functional_power`
- Status: experimental

```cpp
#include "fps/composition.hpp"
```

## どんな問題に使えるか

`compose(f,g,n)`は`f(g(x)) mod x^n`、`functional_power(f,k,n)`は`f`の`k`回自己合成を返す。
漸化式の変数変換、生成関数の代入、形式的写像の反復に使う。現在は`g[0]=0`を前提とする。

NTT可能な法で必要な2冪長が法の原始根に収まる場合は、転置NTTを使う高速合成を選び、
1回の合成が`O(n log^2 n)`時間・`O(n)`領域になる。その他の法では平方分割へ戻り、
`O(n^2+sqrt(n)M(n))`時間・`O(n sqrt(n))`領域。自己合成冪は合成を`O(log k)`回行う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `compose`

```cpp
template<int mod> fps<mod> compose(const fps<mod>& outer, const fps<mod>& inner, int size = -1)
```

NTT可能ならO(n log^2 n)、それ以外はO(n^2+sqrt(n)M(n))。compose(f,g,n): f(g(x)) mod x^nを返す。g[0]=0。

### `functional_power`

```cpp
template<int mod> fps<mod> functional_power(fps<mod> function, long long exponent, int size = -1)
```

O(log k)回のcompose。functional_power(f,k,n): fをk回自己合成する。恒等写像はx。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[FPS 24題 X - 関数的平方根](https://atcoder.jp/contests/fps-24/tasks/fps_24_x)では、特性`p`で
求める平方根が`F`の`(p+1)/2`回合成になる。`verify/atcoder_fps_24_x.cpp`で使用し、両サンプルと
`n=8000`のローカル実行を確認済み。judge ACは未確認。

高速合成の構成は、同問題の[公式解説](https://atcoder.jp/contests/fps-24/editorial/13898)と、
[AC実装](https://atcoder.jp/contests/fps-24/submissions/70667150)の転置NTTによる方法を参考にしている。
property testでは高速経路を通る64・95・128項を愚直な冪の和と比較する。
