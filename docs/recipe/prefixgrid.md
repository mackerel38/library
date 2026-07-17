---
title: "行・列prefixのORで作るグリッド"
documentation_of: //cp/recipe/prefixgrid.hpp
---

# 行・列prefixのORで作るグリッド


## 概要

最初は全マス0のグリッドについて、各行の左から任意個と各列の上から任意個を1にする。
この操作で得られる相異なる0/1グリッドのうち、`0`・`1`・`?`からなるpatternに一致する個数を数える。

## 厳密な定義

- `count_row_column_prefix_union`: O(hw2^min(h,w))。行左prefixと列上prefixのORで作れpatternに一致するgrid数を返す。

## Include

```cpp
#include "recipe/prefixgrid.hpp"

auto answer = poe::count_row_column_prefix_union<998244353>(pattern);
```

同じ完成グリッドを作るprefix長の組が複数あっても一度だけ数える。
縦横を自動で転置し、小さい方をbitmask幅にする。高さ`h`、幅`w`として
`O(hw 2^min(h,w))`時間、`O(2^min(h,w))`領域。

各行を上から処理し、bitmaskは各列がまだ上prefixを伸ばせるかを表す。
行prefixの終端候補を左から一つずつ確定し、残りbitのsubset遷移を一回の走査へまとめる。

## recipeへ置く理由

row-prefixとcolumn-prefixのORという生成規則まで固定した完成形であり、一般のsubset DPや
grid DPより適用範囲が狭い。一方で提出側の重複除去を完全に隠せるため、recipeとして保持する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_row_column_prefix_union`

```cpp
template<int mod> staticmodint<mod> count_row_column_prefix_union(std::vector<std::string> pattern)
```

O(hw2^min(h,w))。行左prefixと列上prefixのORで作れpatternに一致するgrid数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC295 Ex - E or m](https://atcoder.jp/contests/abc295/tasks/abc295_h)へ直接使える。
`verify/atcoder_abc295_h.cpp`で公式sample 3件を確認済み。

## 検証

- `tests/api/recipe/prefixgrid.cpp`: 固定値・全wildcardの基本例
- `tests/property/recipe/prefixgrid.cpp`: 全row/column prefix長を列挙し完成gridを重複除去して比較
