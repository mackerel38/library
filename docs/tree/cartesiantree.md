---
title: "Cartesian Tree"
documentation_of: //cp/tree/cartesiantree.hpp
---

# Cartesian Tree

- Header: `cp/tree/cartesiantree.hpp`
- Symbol: `poe::cartesian_tree`, `poe::cartesiantree_result`
- Status: experimental

## どんな問題に使えるか

列の各区間を最小値または最大値の位置で再帰分割する問題を木DPへ変換するときに使う。
inorderが元配列順なので、区間の包含関係を木の祖先関係として扱える。単に区間最小値queryが必要なだけなら
Segment Treeの方が直接的であり、同値要素の親子規則が解法へ影響する場合はtie規則を確認する。

## 使い方

```cpp
#include "tree/cartesiantree.hpp"

auto min_tree = poe::cartesian_tree(values);
auto max_tree = poe::cartesian_tree(values, std::greater{});
int root = tree.root;
```

inorder順が元配列の順序と一致する。省略時は親の値が子以下となるmin Cartesian Tree、
`std::greater{}`を渡すと親の値が子以上となるmax Cartesian Treeを返す。
同値では左側の要素を祖先にする。空列の根は`-1`。

## 計算量

`O(n)`時間、`O(n)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `cartesiantree_result`

```cpp
struct cartesiantree_result
```

cartesiantree_resultはmin Cartesian Treeの親・左右の子・根を保持する。

### `cartesian_tree`

```cpp
template <class T, class Compare = std::less<T>> cartesiantree_result cartesian_tree(const std::vector<T>& values, Compare compare =
```

O(n)。compare(child,parent)が偽となるCartesian Treeを返す。std::greater{}ならmax木。
compare上で同値なら左側の要素を祖先にする。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[NDPC F - 集合](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_f)では列をCartesian Treeへ変換して木DPを行う。

```cpp
auto tree = poe::cartesian_tree(a);
dfs(tree.root); // 問題固有の二乗木DPを行う
```

このヘッダは木の構築だけを担当し、問題固有の二乗木DPは含めない。
`verify/atcoder_ndpc2026_f.cpp`で2026-07-15にjudge AC確認済み。

[AtCoder ABC435 F - Cat exercise](https://atcoder.jp/contests/abc435/tasks/abc435_f)ではmax Cartesian Treeを作り、
各頂点から左右の子へ降りるpathの添字距離和の最大値を木DPする。
`verify/atcoder_abc435_f.cpp`で公式sampleを確認済み。judge ACは未確認。
