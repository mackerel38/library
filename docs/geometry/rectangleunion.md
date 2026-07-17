---
title: "軸平行長方形の和集合面積"
documentation_of: //cp/geometry/rectangleunion.hpp
---

# 軸平行長方形の和集合面積

- Header: `cp/geometry/rectangleunion.hpp`
- Symbol: `poe::axisalignedrectangle`, `poe::rectangle_area`, `poe::split_rectangle_x`,
  `poe::split_rectangle_y`, `poe::translate_rectangle`, `poe::rectangles_edge_adjacent`,
  `poe::rectangle_union_area`
- Status: experimental

## どんな問題に使えるか

重なりを許す多数の軸平行長方形が覆う面積を数える。長方形は
`[left,right) x [bottom,top)`の半開区間で表し、退化長方形は無視する。

```cpp
#include "geometry/rectangleunion.hpp"

vector<axisalignedrectangle<long long>> rectangles;
long long area = rectangle_union_area(rectangles);
```

x座標でevent sweepし、圧縮したy区間の被覆長をsegment treeで管理する。
`O(n log n)`時間、`O(n)`領域。座標差の積と総面積が型`T`に収まること。

一枚の半開長方形には、いずれも`O(1)`の次の操作も使える。

- `rectangle_area(rectangle)`: 面積
- `split_rectangle_x(rectangle, x)`、`split_rectangle_y(rectangle, y)`: 指定座標で空でない高々二枚へ分割
- `translate_rectangle(rectangle, dx, dy)`: 平行移動
- `rectangles_edge_adjacent(left, right)`: 正の長さの辺で接するか。点接触と面積を持つ重なりはfalse

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `axisalignedrectangle`

```cpp
template <class T> struct axisalignedrectangle
```

半開軸平行長方形[left,right)x[bottom,top)を表す。

### `rectangle_area`

```cpp
template <class T> T rectangle_area(const axisalignedrectangle<T>& rectangle)
```

O(1)。半開長方形の面積を返す。

### `split_rectangle_x`

```cpp
template <class T> std::vector<axisalignedrectangle<T>> split_rectangle_x( const axisalignedrectangle<T>& rectangle, const T& coordinate )
```

O(1)。半開長方形をx=coordinateで空でない高々二つへ分割する。

### `split_rectangle_y`

```cpp
template <class T> std::vector<axisalignedrectangle<T>> split_rectangle_y( const axisalignedrectangle<T>& rectangle, const T& coordinate )
```

O(1)。半開長方形をy=coordinateで空でない高々二つへ分割する。

### `translate_rectangle`

```cpp
template <class T> axisalignedrectangle<T> translate_rectangle( axisalignedrectangle<T> rectangle, const T& dx, const T& dy )
```

O(1)。半開長方形を(dx,dy)だけ平行移動する。

### `rectangles_edge_adjacent`

```cpp
template <class T> bool rectangles_edge_adjacent( const axisalignedrectangle<T>& left, const axisalignedrectangle<T>& right )
```

O(1)。面積正の半開長方形が正の長さの辺で接するならtrueを返す。点接触・重なりはfalse。

### `rectangle_union_area`

```cpp
template <std::integral T> T rectangle_union_area(const std::vector<axisalignedrectangle<T>>& rectangles)
```

O(n log n)。半開軸平行長方形群の和集合面積を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC449 F - Grid Clipping](https://atcoder.jp/contests/abc449/tasks/abc449_f)では、
各黒マスを含む配置左上座標の長方形を作り、全配置数からそのunion areaを引く。
`verify/atcoder_abc449_f.cpp`に提出用コードを用意し、公式sampleを確認済み。judge ACは未確認。

[AtCoder ABC432 D - Suddenly, A Tempest](https://atcoder.jp/contests/abc432/tasks/abc432_d)では、
各変換で長方形を境界分割して平行移動し、最後に辺接触する長方形をDSUで併合する。
`verify/atcoder_abc432_d.cpp`で公式sampleを確認済み。judge ACは未確認。
