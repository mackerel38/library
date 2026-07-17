---
title: "木距離行列の判定・復元"
documentation_of: //cp/tree/treemetric.hpp
---

# 木距離行列の判定・復元


## 概要

対称行列が、同じ頂点集合上の正重み無向木の全点間距離になっているか判定する。
成立する場合は、その距離を実現する`undirected_graph<Cost>`も復元できる。
入力頂点以外にSteiner頂点を追加してよい木距離や、0・負辺を許す場合には使えない。

## 厳密な定義

- `restore_tree_metric`: O(n^2)。distanceが正重み木の全点間距離ならその木を返し、そうでなければnullopt。
- `is_tree_metric`: O(n^2)。distanceが正重み木の全点間距離ならtrueを返す。

## Include

```cpp
#include "tree/treemetric.hpp"

auto tree = restore_tree_metric(distance);
if (!tree) cout << "No\n";
```

根からの距離の加法性で各頂点の直近祖先を選び、復元木の全距離を再検査する。
`O(n^2)`時間、入力と返却値を除いて`O(n)`追加領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `restore_tree_metric`

```cpp
template <class Cost> std::optional<undirected_graph<Cost>> restore_tree_metric( const std::vector<std::vector<Cost>>& distance )
```

O(n^2)。distanceが正重み木の全点間距離ならその木を返し、そうでなければnullopt。

### `is_tree_metric`

```cpp
template <class Cost> bool is_tree_metric(const std::vector<std::vector<Cost>>& distance)
```

O(n^2)。distanceが正重み木の全点間距離ならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC451 E - Tree Distance](https://atcoder.jp/contests/abc451/tasks/abc451_e)へ
`is_tree_metric(distance)`を直接使える。`verify/atcoder_abc451_e.cpp`に提出用コードを用意し、
公式sampleを確認済み。
