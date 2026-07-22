---
title: "distancepairs"
documentation_of: //cp/geometry/distancepairs.hpp
---

# `distancepairs`

## 概要

二次元整数点列から、Euclid距離が指定値以下である点番号の組をすべて列挙する。
答えが少ないときは全点対を調べず、出力に近い時間で動作する。

## 厳密な定義

点列 $P_0,\ldots,P_{n-1}$ と正の距離 $K$ に対し、

$$
0\le i<j<n,\qquad
(P_i.x-P_j.x)^2+(P_i.y-P_j.y)^2\le K^2
$$

を満たすすべての組 $(i,j)$ を辞書順で返す。

## Include

```cpp
#include "geometry/distancepairs.hpp"
```

## API

```cpp
vector<pair<int, int>> pairs_within_distance(points, radius);
```

- `points`: `point<T>` の列。`T` は64bit以下の整数型。
- `radius`: 正の整数。
- 返り値: 条件を満たす0-indexed点番号対の辞書順列。重複座標も別の点として扱う。

平面を一辺 `radius` の正方形bucketへ分ける。新しい点と同じbucketまたは周囲8bucketにある既出点だけを調べる。
距離の二乗は `__int128_t` で計算し、64bit乗算のoverflowを避ける。負座標でも数学的なfloor除算でbucketを決める。

返す点対数を $z$ とすると期待計算量は $O(n+z)$、使用メモリは $O(n+z)$。
最後に辞書順へ並べるため、厳密には $O(n+z\log z)$ 時間を要する。

## 使用例

[AtCoder ABC234 Ex - Enumerate Pairs](https://atcoder.jp/contests/abc234/tasks/abc234_h)では、入力点と $K$ をそのまま渡す。

```cpp
auto pairs = pairs_within_distance(points, K);
cout << pairs.size() << '\n';
for (auto [i, j] : pairs) cout << i + 1 << ' ' << j + 1 << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `pairs_within_distance`

```cpp
template<std::signed_integral T> std::vector<std::pair<int, int>> pairs_within_distance( const std::vector<point<T>>& points, T radius )
```

期待O(n+z)時間・O(n+z)領域。Euclid距離がradius以下の全点番号対を辞書順で返す。

<!-- END AUTO-GENERATED API REFERENCE -->
