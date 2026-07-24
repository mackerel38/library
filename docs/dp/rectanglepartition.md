---
title: "矩形分割DP"
documentation_of: //cp/dp/rectanglepartition.hpp
---

# 矩形分割DP

## 概要

長方形をそのまま処理するか、水平線または垂直線で二つに切って別々に処理するかを選び、
全体の最小費用を求める。各長方形の直接費用だけをラムダ式で指定する。

## 厳密な定義

半開長方形$R=[t,b)\times[l,r)$の直接費用を$c(R)$とする。次の漸化式で定まる$dp(R)$を返す。

$$
dp(R)=\min\left(
c(R),
\min_{t<m<b}\{dp([t,m)\times[l,r))+dp([m,b)\times[l,r))\},
\min_{l<m<r}\{dp([t,b)\times[l,m))+dp([t,b)\times[m,r))\}
\right).
$$

したがって、斜めの切断や、一度切った部分をまたぐ処理は表現しない。

## Include

```cpp
#include "dp/rectanglepartition.hpp"
```

## API

```cpp
template <class T, class Cost>
T minimum_rectangle_partition_cost(int height, int width, Cost cost);
```

- `height`, `width`: 全体の行数と列数。$0$以上。
- `cost(top, left, bottom, right)`: 半開長方形$[top,bottom)\times[left,right)$の直接費用を
  `T`で返す関数。非空な長方形に対して呼ばれる。
- 返り値: 全体$[0,height)\times[0,width)$の最小費用。どちらかの長さが$0$なら`T{}`。
- `T`には加算、`std::min`による比較、`cost`の返り値からの構築が必要。
- $O(h^3w^2+h^2w^3)$時間・$O(h^2w^2)$領域。

```cpp
int answer = poe::minimum_rectangle_partition_cost<int>(h, w,
    [&](int top, int left, int bottom, int right) {
        return direct_cost(top, left, bottom, right);
    });
```

## 頻出事故

分割後の費用は足し合わせる。左右または上下のどちらかだけを選ぶ問題には使えない。
また、直接費用の計算量は全体の計算量へ乗るため、長方形和などは二次元累積和で$O(1)$にする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimum_rectangle_partition_cost`

```cpp
template <class T, class Cost> T minimum_rectangle_partition_cost(int height, int width, Cost cost)
```

O(h^3w^2+h^2w^3)時間・O(h^2w^2)領域。矩形の直接費用と直線分割から最小費用を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC233 G - Strongest Takahashi](https://atcoder.jp/contests/abc233/tasks/abc233_g)では、
印のない長方形の直接費用を$0$、それ以外を長辺の長さとして渡す。
