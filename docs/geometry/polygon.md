---
title: "多角形"
documentation_of: //cp/geometry/polygon.hpp
---

# 多角形

- Header: `cp/geometry/polygon.hpp`
- Symbol: `poe::area2`, `poe::convex_hull`, `poe::containment`, `poe::polygon_contains`, `poe::is_convex`
- Status: experimental

```cpp
#include "geometry/polygon.hpp"
```

符号付き面積、凸包、単純多角形の点包含、凸性判定をまとめる。凸包は重複点を除き、通常は辺上の
中間点を除いた反時計回りの頂点列を返す。`keep_collinear=true`で境界上の点を残す。

凸包は`O(n log n)`、他は`O(n)`。空多角形に`area2`や包含判定を呼ばないこと。

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

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC266 C - Convex Quadrilateral](https://atcoder.jp/contests/abc266/tasks/abc266_c)は`is_convex`で判定できる。
verifyコードを用意し、サンプル確認済み・。
