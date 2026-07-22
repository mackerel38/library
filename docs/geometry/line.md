---
title: "直線と直線交点の距離順位"
documentation_of: //cp/geometry/line.hpp
---

# 直線と直線交点の距離順位

## 概要

$ax+by+c=0$形式の直線、二直線の交点、点と直線の距離を扱う。
さらに、多数の直線対の交点を列挙せず、円内にある交点数や原点から$k$番目に近い交点距離を求める。

## 厳密な定義

- `line<T>{a,b,c}`は$a x+b y+c=0$を表す。
- `line_intersection`は平行でない二直線の唯一の交点を返す。平行なら`nullopt`を返す。
- `distance_to_line(p,l)`は点$p$と直線$l$のEuclid距離を返す。
- `count_line_intersections_in_circle`は、二直線の交点が指定したopen円
  $\{p\mid\lVert p-c\rVert<r\}$内にある直線対数を返す。
- `kth_line_intersection_distance`は、重複を含む全直線対の交点距離を昇順に並べた$k$番目を返す。

## Include

```cpp
#include "geometry/line.hpp"
```

## 基本操作

```cpp
poe::line<long long> first{1, 1, -3};
poe::line<long long> second{1, -1, -1};

auto p = poe::line_intersection(first, second); // (2, 1)
long double d = poe::distance_to_line(poe::point<int>{0, 0}, first);
```

`a`と`b`を同時に0にはできない。交点と距離は`long double`で返す。

## 円内交点数

各直線が円を横切る二点を円周上の弦の端点とみなす。二直線の交点がopen円内にあることと、
二本の弦の端点が円周上で交互に現れることは同値である。

```cpp
long long inside = poe::count_line_intersections_in_circle(
    lines, poe::point<long double>{0, 0}, radius
);
```

接点や円周上の直線交点は数えない。時間計算量は$O(n\log n)$、領域計算量は$O(n)$。

## k番目の交点距離

```cpp
long double answer = poe::kth_line_intersection_distance(lines, k);
```

全直線が互いに非平行で、$1\le k\le n(n-1)/2$であることが必要。
既定では50回二分探索する。答えの安全な上界が分かる場合は
`kth_line_intersection_distance(lines, k, iterations, initial_high)`として渡すと、上界を探す反復を省ける。
計算量は$O((iterations+\log answer)n\log n)$。

## 使用例

[AtCoder ABC263 Ex - Intersection 2](https://atcoder.jp/contests/abc263/tasks/abc263_h)では、
係数制約から全交点距離が$3\times10^6$未満と分かるため、`initial_high=3'000'000`を渡せる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `line`

```cpp
template <class T> struct line
```

line<T>{a,b,c}: ax+by+c=0で表す直線。a,bの少なくとも一方は非zero。

### `line_intersection`

```cpp
template <class T, class U> std::optional<point<long double>> line_intersection( const line<T>& first, const line<U>& second )
```

O(1)。平行でない二直線の交点を返す。平行ならnullopt。

### `distance_to_line`

```cpp
template <class T, class U> long double distance_to_line(const point<T>& target, const line<U>& value)
```

O(1)。点targetと直線valueの距離を返す。

### `count_line_intersections_in_circle`

```cpp
template <class T> long long count_line_intersections_in_circle( const std::vector<line<T>>& lines, const point<long double>& center, long double radius )
```

O(n log n)時間・O(n)領域。二直線の交点のうちopen円内にある組数を返す。

### `kth_line_intersection_distance`

```cpp
template <class T> long double kth_line_intersection_distance( const std::vector<line<T>>& lines, long long k, int iterations = 50, long double initial_high = 1 )
```

O(iterations n log n)時間。原点からk番目に近い二直線交点の距離を返す。
全直線は互いに非平行、1<=k<=n(n-1)/2であること。

<!-- END AUTO-GENERATED API REFERENCE -->
