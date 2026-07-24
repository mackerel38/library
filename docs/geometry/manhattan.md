---
title: "Manhattan距離の静的近傍検索"
documentation_of: //cp/geometry/manhattan.hpp
---

# Manhattan距離の静的近傍検索

## 概要

更新のない点集合について、指定点から一定のManhattan距離以内にある点数と、
$k$番目に近い点までの距離を求める。

## 厳密な定義

点の多重集合$P=\{(x_i,y_i)\}_{i=0}^{n-1}$とquery点$(x,y)$に対し、距離を

$$
d_i=|x_i-x|+|y_i-y|
$$

とする。`count(x, y, distance)`は$d_i\le distance$となる$i$の個数、
`kth(x, y, k)`は$d_i$を昇順に並べたときの$k$番目を返す。同距離の点も別々に数える。

## Include

```cpp
#include "geometry/manhattan.hpp"
```

## API

```cpp
template <std::signed_integral T = long long>
struct manhattannearest;
```

座標型`T`は符号付き整数型とする。内部で$x+y$、$x-y$と距離計算を行っても`T`の範囲を
超えないことが必要。

### コンストラクタ

```cpp
explicit manhattannearest(const std::vector<std::pair<T, T>>& points);
```

重複を含む点集合を構築する。$O(n\log n)$時間・$O(n\log n)$領域。

### `size`, `operator[]`

```cpp
int size() const;
const std::pair<T, T>& operator[](int index) const;
```

点数と入力順の点へのconst参照を返す。どちらも$O(1)$。

### `count`

```cpp
int count(T x, T y, T distance) const;
```

$(x,y)$からManhattan距離`distance`以下にある点数を返す。`distance >= 0`を要求する。
$O(\log n)$時間。

### `kth`

```cpp
T kth(T x, T y, int k) const;
```

$(x,y)$から`k`番目に近い点までのManhattan距離を返す。`k`は1-indexedで、
$1\le k\le n$を要求する。最大距離を$C$として$O(\log C\log n)$時間。

```cpp
poe::manhattannearest near(points);
long long distance = near.kth(x, y, k);
```

## 頻出事故

点集合は構築後に更新できない。多数のqueryを処理する前提の構造であり、queryが一件だけなら
全点との距離を列挙する方が簡単である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `manhattannearest`

```cpp
template <std::signed_integral T = long long> struct manhattannearest
```

静的点集合のManhattan近傍: manhattannearest<long long> near(points); kth(x,y,k)はO(log C log n)。

### `manhattannearest`

```cpp
explicit manhattannearest(const std::vector<point>& points) : points_(points), transformed_(transform(points))
```

O(n log n)。重複を含め、pointsの各点を一個として構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。点の個数を返す。

### `operator[]`

```cpp
const point& operator[](int index) const
```

O(1)。入力順でindex番目の点を返す。

### `count`

```cpp
int count(T x, T y, T distance) const
```

O(log n)。(x,y)からManhattan距離distance以下にある点数を返す。

### `kth`

```cpp
T kth(T x, T y, int k) const
```

O(log C log n)。(x,y)から1-indexedでk番目に近い点までのManhattan距離を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC233 H - Manhattan Christmas Tree](https://atcoder.jp/contests/abc233/tasks/abc233_h)では、
全ての木をコンストラクタへ渡し、各queryを`kth(a, b, k)`一回で処理する。
