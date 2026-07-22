---
title: "等差数列のpopcount総和"
documentation_of: //cp/math/bitcountsum.hpp
---

# 等差数列のpopcount総和

## 概要

等差数列に含まれるすべての非負整数について、二進表記の1の個数を合計します。
剰余が指定された区間内の整数や、一定間隔で現れる番号のbit統計に使えます。

## 厳密な定義

非負整数`count`, `first`, `difference`に対して

$$
\sum_{i=0}^{\mathrm{count}-1}
\operatorname{popcount}(\mathrm{first}+i\cdot\mathrm{difference})
$$

を返します。

$2^k$のbitが1であることを

$$
\left\lfloor\frac{x+2^k}{2^{k+1}}\right\rfloor
-\left\lfloor\frac{x}{2^{k+1}}\right\rfloor=1
$$

と表し、各bitについて二つの`floor_sum`の差を求めます。

## Include

```cpp
#include "math/bitcountsum.hpp"
```

## API

```cpp
long long sum_popcount_arithmetic_progression(
    long long count,
    long long first,
    long long difference
);
```

- 数列の最大項は $2^{31}$ 未満である必要があります。
- `count`は $2^{32}$ 未満である必要があります。
- $V$を最大項として、計算量は $O((\log V)^2)$、追加メモリは $O(1)$ です。
- `count=0`では`0`を返します。

## 使用例

```cpp
// 1, 6, 11のpopcount和
long long answer = sum_popcount_arithmetic_progression(3, 1, 5);
// answer == 6
```

[ABC283 Ex - Popcount Sum](https://atcoder.jp/contests/abc283/tasks/abc283_h)では、$R=0$なら初項を$M$、
それ以外では初項を$R$として、$N$以下の項数を渡します。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sum_popcount_arithmetic_progression`

```cpp
inline long long sum_popcount_arithmetic_progression( long long count, long long first, long long difference )
```

O(log V・log V)。first+i*difference (0<=i<count) のpopcount総和を返す。最大項は2^31未満。

<!-- END AUTO-GENERATED API REFERENCE -->
