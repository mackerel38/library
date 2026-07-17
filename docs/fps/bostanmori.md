---
title: "Bostan–Mori法"
documentation_of: //cp/fps/bostanmori.hpp
---

# Bostan–Mori法

- Header: `cp/fps/bostanmori.hpp`
- Symbol: `poe::bostan_mori`, `poe::bostan_mori_submask_sum`,
  `poe::fast_linear_recurrence`
- Status: experimental

## どんな問題に使えるか

有理型母関数 `P(x)/Q(x)` の非常に遠い係数や、次数が大きい固定係数線形漸化式の
第 `n` 項だけを求める。小さい次数なら実装の軽い `math/linearecurrence.hpp`、
次数が数百以上ならこちら、という使い分けを想定している。NTT可能な法では直接NTTし、
それ以外のstatic modintでは`math/convolution.hpp`の3法CRT経路を使う。

## API・計算量

```cpp
#include "fps/bostanmori.hpp"

using mint = poe::modint998244353;
mint a_n = poe::fast_linear_recurrence<998244353>(initial, coefficient, n);
mint coefficient_n = poe::bostan_mori<998244353>(p, q, n);
mint subset_total = poe::bostan_mori_submask_sum<998244353>(p, q, mask);
```

`M(k)`を次数`k`の多項式積の計算量として、`O(M(k) log n)`時間、`O(k)`領域。

`bostan_mori_submask_sum`は`P(x)/Q(x)=sum a[i]x^i`について、
`(i & mask) == i`を満たすすべての`a[i]`を合計する。通常のBostan--Moriが各bitで
偶数項または奇数項を選ぶのに対し、`mask`のbitが1なら両方を足す。
計算量は`O(M(k) log mask)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `bostan_mori`

```cpp
template <int mod> staticmodint<mod> bostan_mori( std::vector<staticmodint<mod>> numerator, std::vector<staticmodint<mod>> denominator, long long index )
```

O(M(k) log n)。P(x)/Q(x)のx^nの係数をBostan-Mori法で返す。
Q[0]!=0かつP.size()<Q.size()。modはNTT可能な素数。

### `bostan_mori_submask_sum`

```cpp
template<int mod> staticmodint<mod> bostan_mori_submask_sum( std::vector<staticmodint<mod>> numerator, std::vector<staticmodint<mod>> denominator, unsigned long long mask )
```

O(M(k) log mask)。P/Qの係数a[i]についてiがmaskのsubmaskであるa[i]の総和を返す。

### `fast_linear_recurrence`

```cpp
template <int mod> staticmodint<mod> fast_linear_recurrence( const std::vector<staticmodint<mod>>& initial, const std::vector<staticmodint<mod>>& coefficient, long long index )
```

O(M(k) log n)。固定係数k項線形漸化式のa[n]を畳み込みで求める。
a[i]=sum coefficient[j-1]*a[i-j]。modはNTT可能な素数。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[TDPC T - フィボナッチ](https://atcoder.jp/contests/tdpc/tasks/tdpc_fibonacci)で使える。
`verify/atcoder_tdpc_fibonacci.cpp`で2026-07-15にjudge AC確認済み。

[AtCoder ABC300 Ex - Fibonacci: Revisited](https://atcoder.jp/contests/abc300/tasks/abc300_h)
ではK-bonacci数列の有理型母関数を作り、係数のsubmask和を求める。
`verify/atcoder_abc300_h.cpp`で公式sample 5件を確認済み。

`tests/property/fps/bostanmorisubmask.cpp`では小さい係数列を直接展開し、
全submask列挙の和と比較する。
