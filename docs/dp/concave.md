---
title: "区分線形な上に凸列"
documentation_of: //cp/dp/concave.hpp
---

# 区分線形な上に凸列


## 概要

有限な整数区間上の上に凸なDP列を折れ点だけで持ち、次の遷移を高速化する。

## 厳密な定義

- `concavesequence`: 整数区間上の区分線形な上に凸列。点から作り、一次関数加算・level未満切除・suffix max変換を行う。

- 全状態への一次関数加算
- 値が閾値未満になる両端状態の削除
- `g(x) = max_{y >= x}(f(y) - c(y-x))`というsuffix max-plus変換

## Include

```cpp
#include "dp/concave.hpp"

poe::concavesequence dp(initial_x, initial_value);
dp.add_affine(slope, constant);
dp.trim_below(0);
dp.suffix_max(cost, 0);
```

各折れ点は端から高々一度削除されるため、一連の操作は追加された折れ点数に対してならし線形。
`get(x)`は`O(log pieces)`、端点取得は`O(1)`。座標・値・傾きは整数で、
各線分の両端の差が長さで割り切れることを仮定する。

一般のSlope Trick全体ではなく、整数上のconcave max-DPに必要な操作を表す。
凸な最小化関数へ`add_abs`などを行う場合は`dp/slopetrick.hpp`を使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `concavesequence`

```cpp
struct concavesequence
```

整数区間上の区分線形な上に凸列。点から作り、一次関数加算・level未満切除・suffix max変換を行う。

### `concavesequence`

```cpp
concavesequence(long long x, long long value) : points_
```

O(1)。座標xだけでvalueを取る列を作る。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。定義域が空かを返す。

### `left`

```cpp
long long left() const
```

O(1)。定義域の左端を返す。空でないこと。

### `right`

```cpp
long long right() const
```

O(1)。定義域の右端を返す。空でないこと。

### `get`

```cpp
std::optional<long long> get(long long x) const
```

O(log pieces)。xが定義域内なら値を返す。

### `add_affine`

```cpp
void add_affine(long long slope, long long constant)
```

O(1)。全xの値へslope*x+constantを加える。

### `trim_below`

```cpp
void trim_below(long long level)
```

ならしO(pieces)。値がlevel未満の座標を定義域から除く。

### `suffix_max`

```cpp
void suffix_max(long long slope, long long new_left)
```

ならしO(pieces)。g(x)=max_{y>=x}(f(y)-slope*(y-x))へ変換し左端をnew_leftまで拡張する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC458 G - Children Yearn for the Evil Kindergarten](https://atcoder.jp/contests/abc458/tasks/abc458_g)の
人数別最大メダルDPをこの列として持てる。`verify/atcoder_abc458_g.cpp`で公式sample一致を確認している。
