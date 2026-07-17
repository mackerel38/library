---
title: "周期剰余への初到達期待値"
documentation_of: //cp/math/randomwalk.hpp
---

# 周期剰余への初到達期待値

- Header: `cp/math/randomwalk.hpp`
- Symbol: `poe::cyclic_hitting_expectation`
- Status: experimental

## どんな問題に使えるか

毎回独立に正の整数stepを選んで累積和へ加え、累積和が`remaining (mod period)`へ
初めて到達するまでのstep回数の期待値を求める。

```cpp
#include "math/randomwalk.hpp"

using mint = poe::modint998244353;
mint answer = poe::cyclic_hitting_expectation(
    period, remaining,
    std::vector<mint>{mint{1} / 6, mint{1} / 6, mint{1} / 6,
                      mint{1} / 6, mint{1} / 6, mint{1} / 6}
);
```

確率列の`i`番目はstep `i+1`の確率で、総和は1であること。末尾の確率0は自動で除く。
最大stepを`d`として`period>d`、`1<=remaining<period`を要求する。

期待値の局所漸化式を`d+1`次のaffine行列で進め、周期境界で生じる`d-1`個の未知値だけを
Gauss消去で決定する。計算量は`O(d^3 log period)`、領域は`O(d^2)`。
法上の期待値では境界方程式が一意解を持つ必要がある。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `cyclic_hitting_expectation`

```cpp
template<class T> T cyclic_hitting_expectation( long long period, long long remaining, std::vector<T> step_probability )
```

O(d^3 log period)。正のstep和がremaining mod periodへ初到達するまでの期待step数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC299 Ex - Dice Sum Infinity](https://atcoder.jp/contests/abc299/tasks/abc299_h)では、
`period=10^9`、`remaining=R`、step 1から6を各確率`1/6`として渡す。
`verify/atcoder_abc299_h.cpp`で公式sample 2件を確認済み。judge ACは未確認。

## 検証

- `tests/api/math/randomwalk.cpp`: 一歩walkと公平サイコロの基本例
- `tests/property/math/randomwalk.cpp`: 小周期の全剰余連立方程式との比較
