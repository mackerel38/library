---
title: "子を昇順に辿る行きがけ順"
documentation_of: //cp/tree/preorder.hpp
---

# 子を昇順に辿る行きがけ順

## 概要

頂点labelの列がDFSの行きがけ順として与えられる。各頂点の子をlabel昇順に訪れるという条件のもとで、
その行きがけ順を作る根付き木が何通りあるかを数える。

## 厳密な定義

相異なるlabel $A_0,A_1,\ldots,A_{n-1}$を持つ$n$頂点を考え、$A_0$の頂点を根とする。
各頂点の子をlabelの小さい順に辿るDFSを行ったとき、行きがけ順がちょうど
$A_0,A_1,\ldots,A_{n-1}$となる根付き木の数を返す。
二つの木は、少なくとも一頂点の親が異なるとき別の木として数える。

## Include

```cpp
#include "tree/preorder.hpp"
```

```cpp
using mint = staticmodint<998244353>;
mint count = count_sorted_child_preorder_trees<mint>(labels);
```

`T`は`T{}`、`T{1}`、加算、乗算を持つ必要がある。`Label`は`operator<`で比較でき、labelは
互いに異なる必要がある。時間計算量は$O(n^3)$、領域計算量は$O(n^2)$である。
空列には`T{}`を返す。

## 頻出事故

- 入力列は単なる頂点番号順ではなく、実現させたい行きがけ順で渡す。
- 子の訪問順は入力列内の位置順ではなく、`Label`の昇順である。
- 根は入力列の先頭要素で固定される。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_sorted_child_preorder_trees`

```cpp
template<class T, class Label> T count_sorted_child_preorder_trees(const std::vector<Label>& labels)
```

O(n^3)時間・O(n^2)領域。子をlabel昇順に辿るDFSの行きがけ順がlabelsとなる根付き木数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC252 G - Pre-Order](https://atcoder.jp/contests/abc252/tasks/abc252_g)では、
与えられた順列をそのまま渡して条件を満たす根付き木を数える。
`verify/atcoder_abc252_g.cpp`に提出用コードを用意し、公式sampleを確認している。
