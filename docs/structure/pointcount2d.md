---
title: "二次元静的点数え上げ"
documentation_of: //cp/structure/pointcount2d.hpp
---

# 二次元静的点数え上げ

## 概要

更新のない二次元点集合について、軸に平行な長方形に入る点の個数を数える。
同じ座標の点も別々に数える。

## 厳密な定義

点の多重集合$P=\{(x_i,y_i)\}_{i=0}^{n-1}$を前処理する。
`count(x_lower, y_lower, x_upper, y_upper)`は

$$
\left|\{i\mid x_{lower}\le x_i<x_{upper},\ y_{lower}\le y_i<y_{upper}\}\right|
$$

を返す。`count_closed`は二つの上端も含める。

## Include

```cpp
#include "structure/pointcount2d.hpp"
```

## API

```cpp
template <class T>
struct pointcount2d;
```

### コンストラクタ

```cpp
explicit pointcount2d(const std::vector<std::pair<T, T>>& points);
```

`points[i] = {x_i, y_i}`から構築する。入力順と重複を保存する。
$O(n\log n)$時間・$O(n\log n)$領域。

### `size`, `operator[]`

```cpp
int size() const;
const std::pair<T, T>& operator[](int index) const;
```

点数と、入力順の`index`番目の点へのconst参照を返す。どちらも$O(1)$。
`operator[]`は$0\le index<n$を要求する。

### `count`

```cpp
int count(const T& x_lower, const T& y_lower,
          const T& x_upper, const T& y_upper) const;
```

半開長方形$[x_{lower},x_{upper})\times[y_{lower},y_{upper})$内の点数を返す。
各下端は上端以下でなければならない。$O(\log n)$時間。

### `count_closed`

```cpp
int count_closed(const T& x_lower, const T& y_lower,
                 const T& x_upper, const T& y_upper) const;
```

閉長方形$[x_{lower},x_{upper}]\times[y_{lower},y_{upper}]$内の点数を返す。
各下端は上端以下でなければならない。$O(\log n)$時間。

## 使用例

```cpp
poe::pointcount2d<long long> points({{0, 0}, {2, 3}, {2, 3}});
int count = points.count(0, 0, 3, 4); // 3
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `pointcount2d`

```cpp
template <class T> struct pointcount2d
```

静的点集合の長方形内点数: pointcount2d<long long> points(data); 構築O(n log n)、取得O(log n)。

### `pointcount2d`

```cpp
explicit pointcount2d(const std::vector<point>& points) : points_(points), sorted_(points)
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
int count(const T& x_lower, const T& y_lower, const T& x_upper, const T& y_upper) const
```

O(log n)。[x_lower,x_upper)x[y_lower,y_upper)にある点数を返す。

### `count_closed`

```cpp
int count_closed( const T& x_lower, const T& y_lower, const T& x_upper, const T& y_upper ) const
```

O(log n)。[x_lower,x_upper]x[y_lower,y_upper]にある点数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC233 H - Manhattan Christmas Tree](https://atcoder.jp/contests/abc233/tasks/abc233_h)では、
座標を$(x+y,x-y)$へ変換した後の正方形内点数を判定に使う。
