---
title: "多角形"
documentation_of: //cp/geometry/polygon.hpp
---

# 多角形

## 概要

面積・凸包・点包含・凸性判定を扱う。同じ凸多角形を複数の位置へ平行移動したとき、その全てに
含まれる点も、辺ごとの半平面制約を前計算して判定できる。

## 厳密な定義

- `area2`: area2(polygon): 符号付き面積の2倍を返す。反時計回りなら正。O(n)。
- `convex_hull`: O(n log n)。convex_hull(points,keep_collinear): 反時計回りの凸包を返す。重複点は除く。
- `containment`: containment: 多角形に対する点の位置。
- `polygon_contains`: O(n)。polygon_contains(polygon,p): 単純多角形に対するpの位置を返す。
- `is_convex`: O(n)。is_convex(polygon,strict): 頂点順の多角形が凸か返す。strictなら一直線の辺を許さない。
- `translatedpolygonintersection`: 反時計回りの狭義凸多角形$P$と平行移動量$t_j$に対し、
  $\bigcap_j(P+t_j)$への点包含を判定する。



## Include

```cpp
#include "geometry/polygon.hpp"
```

符号付き面積、凸包、単純多角形の点包含、凸性判定をまとめる。凸包は重複点を除き、通常は辺上の
中間点を除いた反時計回りの頂点列を返す。`keep_collinear=true`で境界上の点を残す。

凸包は`O(n log n)`、他は`O(n)`。空多角形に`area2`や包含判定を呼ばないこと。

```cpp
translatedpolygonintersection region(polygon, shifts);
bool included = region.contains(target);
```

`polygon`の各辺を反時計回りに向けたベクトル$d_i$、辺の始点を$p_i$とする。
`region.contains(q)`は全ての$i$について

$$
d_i\times q\ge \max_j d_i\times(p_i+t_j)
$$

が成り立つかを返す。境界も含む。`polygon`は3頂点以上の反時計回り狭義凸多角形、`shifts`は
空でない必要がある。頂点数$n$、平行移動数$m$に対し構築は$O(nm)$、一回の判定は$O(n)$、
領域は$O(n)$である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `area2`

```cpp
template<class T> geometrywide<T> area2(const std::vector<point<T>>& polygon)
```

area2(polygon): 符号付き面積の2倍を返す。反時計回りなら正。O(n)。

### `convex_hull`

```cpp
template<class T> std::vector<point<T>> convex_hull(std::vector<point<T>> points, bool keep_collinear = false)
```

O(n log n)。convex_hull(points,keep_collinear): 反時計回りの凸包を返す。重複点は除く。

### `containment`

```cpp
enum class containment
```

containment: 多角形に対する点の位置。

### `polygon_contains`

```cpp
template<class T> containment polygon_contains(const std::vector<point<T>>& polygon, const point<T>& target)
```

O(n)。polygon_contains(polygon,p): 単純多角形に対するpの位置を返す。

### `is_convex`

```cpp
template<class T> bool is_convex(const std::vector<point<T>>& polygon, bool strict = true)
```

O(n)。is_convex(polygon,strict): 頂点順の多角形が凸か返す。strictなら一直線の辺を許さない。

### `translatedpolygonintersection`

```cpp
template<class T> struct translatedpolygonintersection
```

平行移動した同一凸多角形すべての共通部分: translatedpolygonintersection<T> region(polygon,shifts); 構築O(nm)、contains O(n)。

### `translatedpolygonintersection`

```cpp
translatedpolygonintersection( const std::vector<point<T>>& polygon, const std::vector<point<T>>& shifts )
```

O(nm)。反時計回りの狭義凸polygonと、一つ以上の平行移動量から共通部分の半平面制約を作る。

### `contains`

```cpp
bool contains(const point<T>& target) const
```

O(n)。targetが全ての平行移動後polygonの内部または境界にあればtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC266 C - Convex Quadrilateral](https://atcoder.jp/contests/abc266/tasks/abc266_c)は`is_convex`で判定できる。
サンプル確認済み。

[AtCoder ABC251 G - Intersection of Polygons](https://atcoder.jp/contests/abc251/tasks/abc251_g)では、
元の凸多角形と全平行移動量から`translatedpolygonintersection`を構築し、各query点を判定する。
`verify/atcoder_abc251_g.cpp`に提出用コードを用意し、公式sampleを確認している。
