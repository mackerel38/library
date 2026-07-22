---
title: "凸多角形を避ける最短路"
documentation_of: //cp/geometry/convexpath.hpp
---

# 凸多角形を避ける最短路

## 概要

凸多角形を障害物として、その内部へ入らずに二点間を移動する最短距離を求めます。
直線で通れる場合は二点間距離を返し、通れない場合は障害物の外周を左右のどちらかへ回る距離の短い方を返します。

## 厳密な定義

反時計回りの凸多角形 $C$ と、$C$ の内部にない二点 $S,T$ に対して、
$C$ の内部と交わらない連続曲線 $P$ の長さの下限

$$
\inf_{P:S\to T,\;P\cap\operator{int}(C)=\varnothing}\operator{length}(P)
$$

を返します。多角形の辺上と頂点上は通れます。

`C`、`S`、`T` の凸包上に両方の端点が現れる場合、最短路はその凸包の二つの外周経路のどちらかです。
片方が凸包上に現れない場合、線分 $ST$ は $C$ の内部を横切らないため、答えは $|ST|$ です。

## Include

```cpp
#include "geometry/convexpath.hpp"
```

## API

```cpp
long double shortest_path_avoiding_convex_polygon(
    const vector<point<T>>& polygon,
    const point<T>& start,
    const point<T>& goal
);
```

- `polygon`は3頂点以上で、反時計回りの凸多角形にします。一直線上の頂点が含まれていても構いません。
- `start`と`goal`は多角形の内部に置けません。境界上には置けます。
- 計算量は $O(n\log n)$、追加メモリは $O(n)$ です。
- 距離は`long double`で返します。

## 使用例

```cpp
vector<point<long long>> polygon{{1, 1}, {3, 1}, {3, 3}, {1, 3}};
point<long long> start{0, 2};
point<long long> goal{5, 2};
long double answer = shortest_path_avoiding_convex_polygon(polygon, start, goal);
```

[ABC286 Ex - Don't Swim](https://atcoder.jp/contests/abc286/tasks/abc286_h)では、入力された凸多角形と二点を
そのまま渡せます。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `shortest_path_avoiding_convex_polygon`

```cpp
template<class T> long double shortest_path_avoiding_convex_polygon( const std::vector<point<T>>& polygon, const point<T>& start, const point<T>& goal )
```

O(n log n)。反時計回りの凸多角形の内部を避けてstartからgoalへ進む最短距離を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
