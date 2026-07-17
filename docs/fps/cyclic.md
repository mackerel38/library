---
title: "巡回畳み込み累乗"
documentation_of: //cp/fps/cyclic.hpp
---

# 巡回畳み込み累乗

- Header: `cp/fps/cyclic.hpp`
- Symbol: `poe::cyclic_convolution`, `poe::cyclic_power`
- Status: experimental

```cpp
#include "fps/cyclic.hpp"
```

## どんな問題に使えるか

長さ`n`の巡回畳み込み環、すなわち多項式を`x^n-1`で割った余りとして、積や巨大整数乗を求める。

```cpp
auto product = poe::cyclic_convolution<998244353>(left, right);
auto power = poe::cyclic_power<998244353>(values, exponent);
```

長さが2冪かつ`mod-1`の約数なら、NTT後の各周波数を直接累乗して`O(n log n)`時間で処理する。
一般の長さでは巡回畳み込みによる二分累乗へ自動で切り替わり、
`O(M(n) log exponent)`時間となる。指数は非負であること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `cyclic_convolution`

```cpp
template<int mod> std::vector<staticmodint<mod>> cyclic_convolution( const std::vector<staticmodint<mod>>& left, const std::vector<staticmodint<mod>>& right )
```

O(n log n)。cyclic_convolution(a,b): 同じ長さnの列を巡回畳み込みする。

### `cyclic_power`

```cpp
template<int mod> std::vector<staticmodint<mod>> cyclic_power( std::vector<staticmodint<mod>> values, long long exponent )
```

2冪長はO(n log n)、一般長はO(M(n) log k)。cyclic_power(a,k)で巡回畳み込みのa^kを返す。
k>=0。2冪長かつmod-1の約数ならNTT領域で直接累乗する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[FPS 24題 R - ランダムウォーク](https://atcoder.jp/contests/fps-24/tasks/fps_24_r)で、円環上の
左右移動をまとめて累乗する。小さい円環の愚直遷移と比較する。

[AtCoder ABC309 Ex - Simple Path Counting Problem](https://atcoder.jp/contests/abc309/tasks/abc309_h)では、
鏡像法で両端を吸収壁とするpath DPを長さ`2M+2`の巡回畳み込みへ変換する。
`verify/atcoder_abc309_h.cpp`で公式sample 3件を確認済み。最大sampleはlocal約22秒で。
