---
title: "2次元アフィン変換"
documentation_of: //cp/geometry/affine.hpp
---

# 2次元アフィン変換

- Header: `cp/geometry/affine.hpp`
- Symbol: `poe::affine`
- Status: experimental

```cpp
#include "geometry/affine.hpp"
```

平行移動、90度回転、軸に平行な直線についての鏡映と、その合成を持つ。
`left * right`は関数合成と同じく、点へ`right`を適用してから`left`を適用する。

```cpp
poe::affine<long long> transform;
transform = poe::affine<long long>::rotate_clockwise() * transform;
auto moved = transform({x, y});
```

各操作は`O(1)`、変換列のprefixを保存すれば各時点の点クエリも`O(1)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `affine`

```cpp
template<class T> struct affine
```

affine<T>: 2次元アフィン変換。left*rightはrightを適用してからleftを適用する。

### `operator()`

```cpp
point<T> operator()(const point<T>& value) const
```

O(1)。transform(p): 点pを変換する。

### `identity`

```cpp
static affine identity()
```

O(1)。identity(): 恒等変換を返す。

### `translate`

```cpp
static affine translate(T dx, T dy)
```

O(1)。translate(dx,dy): 平行移動を返す。

### `rotate_clockwise`

```cpp
static affine rotate_clockwise()
```

O(1)。rotate_clockwise(): 原点中心の時計回り90度回転を返す。

### `rotate_counterclockwise`

```cpp
static affine rotate_counterclockwise()
```

O(1)。rotate_counterclockwise(): 原点中心の反時計回り90度回転を返す。

### `reflect_x`

```cpp
static affine reflect_x(T x)
```

O(1)。reflect_x(x): 直線X=xに関する鏡映を返す。

### `reflect_y`

```cpp
static affine reflect_y(T y)
```

O(1)。reflect_y(y): 直線Y=yに関する鏡映を返す。

### `operator*`

```cpp
friend affine operator*(const affine& left, const affine& right)
```

O(1)。left*right: rightの後にleftを適用する合成変換を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC189 E - Rotate and Flip](https://atcoder.jp/contests/abc189/tasks/abc189_e)の4種類の操作を直接表せる。
verifyコードを用意し、サンプル確認済み・。
