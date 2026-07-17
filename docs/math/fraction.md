---
title: "分数区間"
documentation_of: //cp/math/fraction.hpp
---

# 分数区間

- Header: `cp/math/fraction.hpp`
- Symbol: `poe::fraction`, `poe::simplest_fraction_between`
- Status: experimental

## どんな問題に使えるか

二つの有理数の開区間に入る分数のうち、分母が最小のものを求める。
連分数またはStern--Brocot treeで同じ方向へ進む操作をまとめ、値の大きさに対して対数時間で処理する。
分母が同じ候補が複数ある場合は分子が最小のものを返す。

```cpp
#include "math/fraction.hpp"

auto [p, q] = poe::simplest_fraction_between(a, b, c, d);
```

入力は`0 <= a/b < c/d`、`b,d > 0`を満たす64 bit整数とする。大小比較には128 bit整数を使う。
戻り値も符号付き64 bit整数へ収まる必要がある。

## 計算量

`O(log max(a,b,c,d))`時間、再帰stack `O(log max(a,b,c,d))`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `fraction`

```cpp
struct fraction
```

fraction{numerator, denominator}: 正の分母を持つ64 bit有理数を表す。

### `simplest_fraction_between`

```cpp
inline fraction simplest_fraction_between( long long a, long long b, long long c, long long d )
```

O(log max(a,b,c,d))。simplest_fraction_between(a,b,c,d): a/b<p/q<c/dで分母最小のp/qを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC408 G - A/B &lt; p/q &lt; C/D](https://atcoder.jp/contests/abc408/tasks/abc408_g)は、
戻り値の`denominator`を出力すればよい。
`verify/atcoder_abc408_g.cpp`で公式sampleを確認済み。
小さい正有理数区間について、分母を昇順に全探索する愚直解とも照合済み。
