---
title: "区間0/1ナップサック"
documentation_of: //cp/dp/rangeknapsack.hpp
---

# 区間0/1ナップサック


## 概要

固定された品物列に対し、多数の半開区間`[left,right)`と小さい容量が与えられ、区間内だけから
品物を選ぶ0/1ナップサックの最大価値をまとめて求める。各queryで容量DPを作り直す代わりに、
列の分割統治で中央をまたぐ全queryへ左suffix DPと右prefix DPを共有する。

## 厳密な定義

- `rangeknapsackquery`: range_knapsack01へ渡す半開区間[left,right)と容量capacity。
- `range_knapsack01`: O(CN log N + CQ + Q log N)時間・O(NC+Q log N)領域。各区間の0/1ナップサック最大価値を返す。

## Include

```cpp
#include "dp/rangeknapsack.hpp"

vector<poe::rangeknapsackquery> queries{{left, right, capacity}};
vector<long long> answer = poe::range_knapsack01(weights, values, queries);
```

何も選ばない価値`0`を許す。重さと容量は非負で、query区間は空でない半開区間とする。
`C=max capacity`として`O(CN log N + CQ + Q log N)`時間、`O(NC + Q log N)`領域。
容量が数百程度でquery数が多い場合に向き、容量自体が大きい場合には向かない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rangeknapsackquery`

```cpp
struct rangeknapsackquery
```

range_knapsack01へ渡す半開区間[left,right)と容量capacity。

### `range_knapsack01`

```cpp
template<class T> std::vector<T> range_knapsack01( const std::vector<int>& weights, const std::vector<T>& values, const std::vector<rangeknapsackquery>& queries )
```

O(CN log N + CQ + Q log N)時間・O(NC+Q log N)領域。各区間の0/1ナップサック最大価値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC426 G - Range Knapsack Query](https://atcoder.jp/contests/abc426/tasks/abc426_g)で、
1-indexed閉区間を0-indexed半開区間へ直してそのまま渡す。公式sample確認済み。

## 検証

- `tests/api/dp/rangeknapsack.cpp`: 基本呼び出しとsample相当
- `tests/property/dp/rangeknapsack.cpp`: 小さい全区間・全容量を部分集合全列挙と比較
