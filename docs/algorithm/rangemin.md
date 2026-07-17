---
title: "オフライン区間上限制約和"
documentation_of: //cp/algorithm/rangemin.hpp
---

# オフライン区間上限制約和

- Symbols: `poe::rangeminsumquery`, `poe::offline_range_min_sum`

## 概要

固定列に対する多数のクエリ
`sum(values[i] と cap の小さい方, left <= i < right)`をまとめて求める。
閾値以下の要素数と和をFenwick Treeで管理するため、値が更新される問題やオンラインで直前の答えに依存する
クエリには使えない。

## 厳密な定義

- `rangeminsumquery`: offline_range_min_sumへ渡す半開区間と上限値。
- `offline_range_min_sum`: 各queryのsum(min(values[i], cap))を一括計算する: offline_range_min_sum(values, queries); O((n+q)log n)。

## Include

```cpp
#include "algorithm/rangemin.hpp"

using query = poe::rangeminsumquery<long long>;
auto answers = poe::offline_range_min_sum(values, std::vector<query>{{1, 5, 10}});
```

列長を`n`、クエリ数を`q`として`O((n+q) log n)`時間・`O(n+q)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rangeminsumquery`

```cpp
template <class T> struct rangeminsumquery
```

offline_range_min_sumへ渡す半開区間と上限値。

### `offline_range_min_sum`

```cpp
template <class T> std::vector<T> offline_range_min_sum(const std::vector<T>& values, const std::vector<rangeminsumquery<T>>& queries)
```

各queryのsum(min(values[i], cap))を一括計算する: offline_range_min_sum(values, queries); O((n+q)log n)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC438 G - Sum of Min](https://atcoder.jp/contests/abc438/tasks/abc438_g)では、互いに素な
周期列の組を固定した添字ごとの巡回区間へ変換し、このAPIで全区間を一括処理する。
`verify/atcoder_abc438_g.cpp`は公式サンプル一致を確認済み。
