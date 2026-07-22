---
title: "点追加可能な凸包support query"
documentation_of: //cp/geometry/incrementalhull.hpp
---

# 点追加可能な凸包support query

## 概要

二次元平面へ点を追加しながら、指定方向へ最も遠い点との内積を求める。
動的な平衡木を使わず、二進マージした複数の静的凸包を内部で管理する。

## 厳密な定義

追加済みの点集合を$S$とする。query vector $(a,b)$に対し、support function

$$
h_S(a,b)=\max_{(x,y)\in S}(ax+by)
$$

を返す。点の追加だけを許し、削除は行わない。

## Include

```cpp
#include "geometry/incrementalhull.hpp"
```

```cpp
incrementalconvexhull<long long> hull;
hull.add({x, y});
__int128_t answer = hull.max_dot(a, b);
```

## API

| 操作 | 意味 | 計算量 |
|---|---|---|
| `add(point)` | 点を追加する | 償却$O(\log^2 n)$ |
| `max_dot(a, b)` | $\max(ax+by)$を返す | $O(\log^2 n)$ |
| `size()` | 重複を含む追加点数 | $O(1)$ |
| `empty()` | 空集合か判定 | $O(1)$ |

追加点を二進数の桁のように大きさ$1,2,4,\ldots$のblockへまとめる。blockを併合するときに上下の
凸包chainを構築し、queryでは各chain上の単峰な内積列を二分探索する。領域計算量は$O(n)$である。

整数型`T`では返り値が`geometrywide<T>`となり、`long long`なら`__int128_t`へ拡張される。

## 頻出事故

- 空集合に対して`max_dot`は呼べない。
- 点の削除には対応しない。
- 同じ点を複数回追加できるが、`size()`では別々に数える。
- $(a,b)=(0,0)$では、空でなければ$0$を返す。
- 出力時に`__int128_t`を`long long`へ戻すなら、問題の答えが範囲内であることを確認する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `incrementalconvexhull`

```cpp
template<class T> struct incrementalconvexhull
```

点を追加し、任意方向の内積最大値を管理する。add・max_dotは償却O(log^2 n)、領域O(n)。

### `incrementalconvexhull`

```cpp
incrementalconvexhull() = default
```

O(1)。空集合を作る。

### `add`

```cpp
void add(point<T> value)
```

償却O(log^2 n)。点を追加する。重複点も追加できる。

### `max_dot`

```cpp
value_type max_dot(T a, T b) const
```

O(log^2 n)。追加済み点(x,y)に対するa*x+b*yの最大値を返す。空集合では呼べない。

### `size`

```cpp
int size() const noexcept
```

O(1)。追加した点の個数を返す。重複も別々に数える。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。点が一つもないならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC244 Ex - Linear Maximization](https://atcoder.jp/contests/abc244/tasks/abc244_h)では、
各queryで$(X_i,Y_i)$を`add`した直後に`max_dot(A_i,B_i)`を呼ぶ。
`verify/atcoder_abc244_h.cpp`に提出用コードを用意している。
