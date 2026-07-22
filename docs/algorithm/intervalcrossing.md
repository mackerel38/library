---
title: "端点が交互に現れる区間対"
documentation_of: //cp/algorithm/intervalcrossing.hpp
---

# 端点が交互に現れる区間対

## 概要

区間を二本選んだとき、一方の左端、他方の左端、一方の右端、他方の右端の順に並ぶ組を数える。
円周上の弦が内部で交差する組数や、区間端点の交互配置を数える問題に使える。

## 厳密な定義

区間$[l_i,r_i]$を向きなしで受け取り、端点を$l_i<r_i$へ正規化する。異なる二区間$i,j$のうち

$$
l_i<l_j<r_i<r_j
$$

または添字$i,j$を交換した不等式を満たす組数を返す。すべて厳密不等号なので、端点が等しい区間対は
その端点を使って交互とは数えない。

## Include

```cpp
#include "algorithm/intervalcrossing.hpp"
```

## 使い方

```cpp
vector<pair<int, int>> intervals = {{0, 3}, {1, 4}, {2, 5}};
cout << poe::count_alternating_interval_pairs(intervals) << '\n'; // 3
```

端点型は大小比較と等値比較ができればよい。各区間は長さ正でなければならない。

## 計算量

区間数を$n$として、時間計算量は$O(n\log n)$、領域計算量は$O(n)$。
右端を通過するとき、内側で開いたままの区間数をFenwick Treeで数える。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_alternating_interval_pairs`

```cpp
template <class T> long long count_alternating_interval_pairs( const std::vector<std::pair<T, T>>& intervals )
```

O(n log n)時間・O(n)領域。端点がl_i<l_j<r_i<r_jと厳密に交互に現れる区間対数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
