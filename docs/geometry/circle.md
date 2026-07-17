---
title: "円と最小包含円"
documentation_of: //cp/geometry/circle.hpp
---

# 円と最小包含円

- Header: `cp/geometry/circle.hpp`
- Symbol: `poe::circle`, `poe::minimum_enclosing_circle`
- Status: experimental

```cpp
#include "geometry/circle.hpp"
```

`circle`は中心と半径を`long double`で保持し、`contains`で点包含を判定する。
`minimum_enclosing_circle(points)`は点を固定seedでshuffleし、境界点が増えたときだけ円を作り直す。
期待計算量`O(n)`、最悪`O(n^3)`、領域`O(n)`。

ほぼ一直線の3点では、3組の直径円から全点を含むものを選ぶ。浮動小数誤差を含むため、出力許容誤差を
持つ問題向けであり、円周上かどうかの厳密な組合せ判定には使わない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `circle`

```cpp
struct circle
```

circle{center,radius}: long doubleの円。

### `contains`

```cpp
template<class T> bool contains(const point<T>& value) const
```

O(1)。c.contains(p): 誤差を考慮してpが円内または円周上か返す。

### `minimum_enclosing_circle`

```cpp
template<class T> circle minimum_enclosing_circle(std::vector<point<T>> points)
```

期待O(n)。minimum_enclosing_circle(points): 全点を含む最小円を返す。空集合は半径0。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC151 F - Enclose All](https://atcoder.jp/contests/abc151/tasks/abc151_f)は返された半径を出力すればよい。
verifyコードを用意し、サンプル確認済み・judge ACは未確認。
