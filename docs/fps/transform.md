---
title: "二項変換"
documentation_of: //cp/fps/transform.hpp
---

# 二項変換

- Header: `cp/fps/transform.hpp`
- Symbol: `poe::binomial_convolution`, `poe::binomial_transform`, `poe::upper_binomial_transform`
- Status: experimental

```cpp
#include "fps/transform.hpp"
```

## どんな問題に使えるか

`b[n]=sum_k C(n,k)a[k]c^(n-k)`という二項変換を、階乗で正規化した畳み込みとして計算する。
`c=-1`なら包除変換、`c=1`なら通常のbinomial transformになる。
二列版`binomial_convolution(a,b)`は`sum_k C(n,k)a[k]b[n-k]`を返す。
上側から集約する`upper_binomial_transform(a,c)`は
`b[k]=sum_{n>=k} C(n,k)a[n]c^(n-k)`を返す。各大きさ`n`の対象から
`k`個を選んで残りへ同じ重みを掛ける集約に使う。

項数`n`が法未満であること。計算量`O(M(n))`、領域`O(n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `binomial_convolution`

```cpp
template<int mod> fps<mod> binomial_convolution(const fps<mod>& left, const fps<mod>& right)
```

O(M(n))。binomial_convolution(a,b)[n]=sum C(n,k)a[k]b[n-k]を返す。

### `binomial_transform`

```cpp
template<int mod> fps<mod> binomial_transform(const fps<mod>& values, staticmodint<mod> coefficient)
```

O(M(n))。binomial_transform(a,c)[n]=sum C(n,k)a[k]c^(n-k)を返す。

### `upper_binomial_transform`

```cpp
template<int mod> fps<mod> upper_binomial_transform(const fps<mod>& values, staticmodint<mod> coefficient)
```

O(M(n))。upper_binomial_transform(a,c)[k]=sum_{n>=k} C(n,k)a[n]c^(n-k)を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[FPS 24題 H - ジャンプ](https://atcoder.jp/contests/fps-24/tasks/fps_24_h)で、原点移動を許した
操作列から禁止した操作列への包除変換に使える。愚直二重和と照合する。
[FPS 24題 Q - サイコロ](https://atcoder.jp/contests/fps-24/tasks/fps_24_q)では二列の冪和を
二項畳み込みする。

[AtCoder ABC432 G - Sum of Binom(A, B)](https://atcoder.jp/contests/abc432/tasks/abc432_g)では、
`a[k]`を値`k`の出現数として`binomial_transform(a, 1)[n]=sum C(n,k)a[k]`を求め、
もう一方の列の各値で参照する。`verify/atcoder_abc432_g.cpp`で公式sampleを確認済み。
。

[AtCoder ABC409 G - Accumulation of Wealth](https://atcoder.jp/contests/abc409/tasks/abc409_g)では、
値`k`が初登場する時刻ごとの期待値を`a[n]`へまとめた後、
`upper_binomial_transform(a, 1-p)`で全ての`k`を一括計算する。
`verify/atcoder_abc409_g.cpp`で公式sampleを確認済み。
