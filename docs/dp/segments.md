---
title: "高々K個の互いに素な部分配列"
documentation_of: //cp/dp/segments.hpp
---

# 高々K個の互いに素な部分配列


## 概要

数列から互いに位置が重ならない非空部分配列を高々`k`個選び、選んだ要素の総和を最大化する。
区間を選ばない場合も許すため、全要素が負なら0を返す。

## 厳密な定義

- `maximum_k_disjoint_subarray_sum`: O(nk)。互いに交わらない高々k個の非空部分配列を選んだ総和の最大値を返す。

## Include

```cpp
#include "dp/segments.hpp"

long long gain = poe::maximum_k_disjoint_subarray_sum(values, k);
```

各選択個数について「区間外」と「現在の区間内」の2状態を持つ。
計算量は`O(nk)`、領域は`O(k)`。値と答えが`T`に収まること。

ちょうど`k`区間を選ぶ問題、区間ごとの固定費、区間長制約にはそのまま使えない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `maximum_k_disjoint_subarray_sum`

```cpp
template <class T> T maximum_k_disjoint_subarray_sum(const std::vector<T>& values, int k)
```

O(nk)。互いに交わらない高々k個の非空部分配列を選んだ総和の最大値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC466 E - Range Flip](https://atcoder.jp/contests/abc466/tasks/abc466_e)では、
表面の総和へ`B_i-A_i`の高々`K`個の互いに素な部分配列和を加える。
`verify/atcoder_abc466_e.cpp`で公式sample一致を確認している。
