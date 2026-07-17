---
title: "2次元点と線分"
documentation_of: //cp/geometry/point.hpp
---

# 2次元点と線分

- Header: `cp/geometry/point.hpp`
- Symbol: `poe::point`, `poe::dot`, `poe::cross`, `poe::orientation`, `poe::on_segment`, `poe::segments_intersect`, `poe::distance`, `poe::distance_to_segment`, `poe::minimum_distance_segment_walks`, `poe::argument_less`, `poe::same_direction`
- Status: experimental

```cpp
#include "geometry/point.hpp"
```

整数座標と浮動小数座標で共通の点・ベクトル演算を提供する。整数の内積・外積は`__int128_t`へ
拡張するため、`long long`座標の積をそのまま`long long`へ入れる場合よりオーバーフローしにくい。
浮動小数の`orientation`は相対誤差`1e-12`を使う。

線分交差は端点で触れる場合と重なる場合も`true`。計算量はすべて`O(1)`。

`distance_to_segment(target, first, second)`は点と線分の最短距離を返し、退化した線分にも対応する。
`minimum_distance_segment_walks(a,b,c,d)`は二点が同時に出発し、それぞれ線分`ab`、`cd`を
速さ1で進んで到着後停止するときの最接近距離を返す。相対位置の軌跡を、先に片方が到着する前後の
高々二本の線分へ分けるため、どちらも`O(1)`である。各線分は長さ正でなければならない。

`argument_less(a,b)`は浮動小数点角度を使わず、非zeroベクトルを正のx軸から反時計回りの
偏角順に比較する。同じrayの点は同値となり、`same_direction`でまとめられる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `point`

```cpp
template<class T> struct point
```

point<T>{x,y}: 2次元点・ベクトル。+,-,scalar積と辞書順比較が使える。

### `dot`

```cpp
template<class T> geometrywide<T> dot(const point<T>& left, const point<T>& right)
```

O(1)。dot(a,b): ベクトルa,bの内積を返す。整数型では__int128_tへ拡張する。

### `cross`

```cpp
template<class T> geometrywide<T> cross(const point<T>& left, const point<T>& right)
```

O(1)。cross(a,b): ベクトルa,bの外積を返す。整数型では__int128_tへ拡張する。

### `cross`

```cpp
template<class T> geometrywide<T> cross(const point<T>& first, const point<T>& second, const point<T>& third)
```

O(1)。cross(a,b,c): (b-a)x(c-a)を返す。

### `orientation`

```cpp
template<class T> int orientation(const point<T>& first, const point<T>& second, const point<T>& third)
```

O(1)。orientation(a,b,c): 反時計回りなら1、時計回りなら-1、一直線なら0。

### `on_segment`

```cpp
template<class T> bool on_segment(const point<T>& first, const point<T>& second, const point<T>& target)
```

O(1)。on_segment(a,b,p): 線分ab上にpがあるか返す。

### `segments_intersect`

```cpp
template<class T> bool segments_intersect( const point<T>& first, const point<T>& second, const point<T>& third, const point<T>& fourth )
```

O(1)。segments_intersect(a,b,c,d): 端点接触を含め、二線分が交わるか返す。

### `distance`

```cpp
template<class T> long double distance(const point<T>& first, const point<T>& second)
```

O(1)。distance(a,b): 二点間のEuclid距離をlong doubleで返す。

### `distance_to_segment`

```cpp
template<class T> long double distance_to_segment( const point<T>& target, const point<T>& first, const point<T>& second )
```

O(1)。distance_to_segment(p,a,b): 点pと線分abの最短距離を返す。

### `minimum_distance_segment_walks`

```cpp
template<class T> long double minimum_distance_segment_walks( const point<T>& first_start, const point<T>& first_goal, const point<T>& second_start, const point<T>& second_goal )
```

O(1)。二点が各線分を速さ1で同時に進み、到着後停止するときの最接近距離を返す。

### `argument_half`

```cpp
template<class T> int argument_half(const point<T>& value)
```

O(1)。偏角を[0,pi)と[pi,2pi)へ分ける。原点ベクトルには使わない。

### `argument_less`

```cpp
template<class T> bool argument_less(const point<T>& left, const point<T>& right)
```

O(1)。偏角の昇順比較を返す。同じ方向は同値として扱う。

### `same_direction`

```cpp
template<class T> bool same_direction(const point<T>& left, const point<T>& right)
```

O(1)。原点から見て同じ向きのray上にあるか返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC266 C - Convex Quadrilateral](https://atcoder.jp/contests/abc266/tasks/abc266_c)で各連続3点の
`orientation`を調べられる。verifyコードはサンプル確認のみで、judge ACは未確認。

[ABC442 E - Laser Takahashi](https://atcoder.jp/contests/abc442/tasks/abc442_e)では偏角順に並べ、
同じrayをgroup化する。`verify/atcoder_abc442_e.cpp`で公式sampleを確認済み。judge ACは未確認。

[ABC426 E - Closest Moment](https://atcoder.jp/contests/abc426/tasks/abc426_e)では
`minimum_distance_segment_walks`へ二人の始点・終点を渡すだけで求められる。
`verify/atcoder_abc426_e.cpp`で公式sample一致、judge未提出。
