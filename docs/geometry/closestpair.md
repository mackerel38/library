---
title: "最近点対"
documentation_of: //cp/geometry/closestpair.hpp
---

# 最近点対

## Include

```cpp
#include "geometry/closestpair.hpp"
```

`closest_pair(points)`は二次元点列からEuclid距離が最小の二点を`O(n log n)`で求める。
結果の`first`と`second`は入力列での点番号、`squared_distance`は距離の二乗である。
`distance()`で`long double`の距離を得られる。

整数座標では差と距離の二乗を`__int128_t`へ拡張するため、64 bit整数の積によるoverflowを避けられる。
浮動小数座標では`long double`を使う。同一点が複数あれば距離0のいずれかの組を返す。
点が二個未満なら`exists()`がfalseになる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `closestpairresult`

```cpp
template<class T> struct closestpairresult
```

最近点対の結果。first,secondは元の点番号、squared_distanceは距離の二乗。

### `exists`

```cpp
bool exists() const noexcept
```

O(1)。二点以上から有効な点対が得られたか返す。

### `distance`

```cpp
long double distance() const
```

O(1)。最近点対のEuclid距離を返す。有効な点対があること。

### `closest_pair`

```cpp
template<class T> closestpairresult<T> closest_pair(const std::vector<point<T>>& points)
```

O(n log n)。二次元点列の最近点対を元の点番号と距離の二乗付きで返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Closest Pair of Points](https://judge.yosupo.jp/problem/closest_pair)。

```cpp
auto result = closest_pair(points);
cout << result.first << ' ' << result.second << '\n';
```

`verify/closest_pair.test.cpp`に提出用コードを用意している。property testでは愚直全点対との
乱択照合を行っているが、judgeへの提出確認は未実施。
