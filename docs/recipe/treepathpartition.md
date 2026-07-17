---
title: "木のpath分割幅"
documentation_of: //cp/recipe/treepathpartition.hpp
---

# 木のpath分割幅

- Header: `cp/recipe/treepathpartition.hpp`
- Symbol: `poe::minimum_path_partition_width`
- Status: experimental recipe

## どんな問題に使えるか

木の頂点を複数色へ分割し、各色の頂点集合が連結な単純pathになるようにする。
そのうえで、任意の木上pathに現れる色数の最大値を最小化する。

```cpp
#include "recipe/treepathpartition.hpp"

int answer = poe::minimum_path_partition_width(graph);
```

上限を固定した判定では、各部分木から「頂点から子孫への最大色数」と
「親まで同色pathを延長できるか」だけを返す。同色にする子は高々2本なので、
子の値は上位定数個だけ保持する。答えを二分探索し`O(n log n)`時間、`O(n)`領域。

## recipeへ置く理由

木DPの状態圧縮は再利用できるが、目的関数がpath成分への彩色とpath上の色数に固定されている。
一般のrerootingやpath coverへ混ぜず、完成形のrecipeとして保持する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimum_path_partition_width`

```cpp
template <undirected_graph_type Graph> int minimum_path_partition_width(const Graph& graph)
```

O(n log n)。各色がpathとなる頂点分割で、任意path上の最大色数の最小値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC293 Ex - Optimal Path Decomposition](https://atcoder.jp/contests/abc293/tasks/abc293_h)
へ直接使える。`verify/atcoder_abc293_h.cpp`で公式sample 3件を確認済み。judge ACは未確認。

## 検証

- `tests/api/recipe/treepathpartition.cpp`: path・star・公式例
- `tests/property/recipe/treepathpartition.cpp`: 小さい木の全頂点色分割との比較
