---
title: "線形漸化式"
documentation_of: //cp/math/linearecurrence.hpp
---

# 線形漸化式

- Header: `cp/math/linearecurrence.hpp`
- Symbol: `poe::semiring_recurrence`, `poe::linear_recurrence`, `poe::maxplus_recurrence`
- Status: experimental

## どんな問題に使えるか

固定係数`k`項線形漸化式の第`n`項だけを高速に求める。`n`が巨大で状態数`k`も行列累乗には
やや大きい場合に向く。係数や漸化式の次数が途中で変わる問題には使えない。
通常の加算・乗算だけでなく、結合的で分配法則を満たす半環も扱える。

## API・計算量

```cpp
#include "math/linearecurrence.hpp"

mint answer = poe::linear_recurrence(initial, coefficient, n);
long long best = poe::maxplus_recurrence(initial, reward, n);
```

`a[i]=sum coefficient[j-1]*a[i-j]`。`O(k^2 log n)`時間、`O(k)`領域。

`maxplus_recurrence`は
`a[i]=max_{1<=j<=k}(a[i-j]+coefficient[j-1])`を計算する。
存在しない遷移の係数には`numeric_limits<T>::lowest()`を置く。
より一般の半環では`semiring_recurrence`へ加法単位元、乗法単位元、二演算を渡す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `semiring_recurrence`

```cpp
template <class T, class Add, class Multiply> T semiring_recurrence( const std::vector<T>& initial, const std::vector<T>& coefficient, long long index, const T& zero, const T& one, Add add, Multiply multiply )
```

O(k^2 log n)。任意半環上のk項漸化式a[i]=add_j multiply(c[j-1],a[i-j])のa[n]。

### `linear_recurrence`

```cpp
template <class T> T linear_recurrence( const std::vector<T>& initial, const std::vector<T>& coefficient, long long index )
```

O(k^2 log n)。linear_recurrence(initial, coefficient, n)でa[n]を返す。
a[i]=sum_{j=1..k} coefficient[j-1]*a[i-j]、initial.size()==k。

### `maxplus_recurrence`

```cpp
template <class T> T maxplus_recurrence( const std::vector<T>& initial, const std::vector<T>& coefficient, long long index )
```

O(k^2 log n)。a[i]=max_j(a[i-j]+coefficient[j-1])で定まるmax-plus漸化式のa[n]。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[TDPC T - フィボナッチ](https://atcoder.jp/contests/tdpc/tasks/tdpc_fibonacci)と同じ形の
固定係数漸化式に使える。ただし同問題の最大制約には`fps/bostanmori.hpp`を使う。
Fibonacciで行列累乗と一致することを検査済み。

[AtCoder ABC415 G - Get Many Cola](https://atcoder.jp/contests/abc415/tasks/abc415_g)では、
小さい状態だけ交換条件付きで直接DPし、全交換が使用可能になる以降をmax-plus漸化式として飛ばす。
`verify/atcoder_abc415_g.cpp`で公式sampleを確認済み。judge ACは未確認。
小さい乱択漸化式を逐次DPと比較するproperty testも実施済み。
