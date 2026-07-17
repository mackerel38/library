---
title: "区間内の同値組数"
documentation_of: //cp/algorithm/rangefrequency.hpp
---

# 区間内の同値組数


## 概要

静的な列と複数の半開区間`[l,r)`について、同じ値を持つ相異なる`k`添字の組数
`sum_x C(freq_x, k)`を一括で求める。

## 厳密な定義

- `small_binomial`: O(k)。nCkをlong longで返す。結果がlong longに収まること。
- `equal_tuple_range_counts`: O((n+q)sqrt(q) k+n log n)。各区間で値が等しいk添字組の個数を返す。

## Include

```cpp
#include "algorithm/rangefrequency.hpp"

auto answer = poe::equal_tuple_range_counts(values, ranges, 3);
```

内部で値を座標圧縮し、`Mo`の端点追加時に`C(freq,k-1)`を足す。
利用側では頻度配列や追加・削除callbackを書かなくてよい。`k=0`では各区間に1を返す。
答えと途中の二項係数が`long long`に収まること。

端点更新一回あたり`O(k)`、全体で
`O(n log n + (n+q)sqrt(q) k)`程度、領域`O(n+q)`。

## インターフェイス

```cpp
long long small_binomial(long long n, int k);
```

`n choose k`を返す。`k < 0`または`n < k`なら`0`。返り値が`long long`に
収まる必要がある。計算量は`O(min(k, n-k))`。

```cpp
template <class T>
vector<long long> equal_tuple_range_counts(
    const vector<T>& values,
    const vector<pair<int, int>>& ranges,
    int tuple_size);
```

各`ranges[q] = {left, right}`について、半開区間`[left, right)`から選ぶ
`tuple_size`個の相異なる添字がすべて同じ値を持つ組の数を、query順に返す。
添字の選択順は区別しない。`0 <= left <= right <= values.size()`かつ
`tuple_size >= 0`が必要で、`tuple_size == 0`の答えは常に`1`。
`T`にはソートと同値判定に使える全順序が必要。答えと途中の二項係数は
`long long`に収まらなければならない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `small_binomial`

```cpp
inline long long small_binomial(long long n, int k)
```

O(k)。nCkをlong longで返す。結果がlong longに収まること。

### `equal_tuple_range_counts`

```cpp
template <class T> std::vector<long long> equal_tuple_range_counts( const std::vector<T>& values, const std::vector<std::pair<int, int>>& ranges, int tuple_size)
```

O((n+q)sqrt(q) k+n log n)。各区間で値が等しいk添字組の個数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC293 G - Triple Index](https://atcoder.jp/contests/abc293/tasks/abc293_g)は
`k=3`として直接使える。`verify/atcoder_abc293_g.cpp`で公式sample 1を確認済み。


## 検証

- `tests/api/algorithm/rangefrequency.cpp`: `k=0,1,2,3`の既知値
- `tests/property/algorithm/rangefrequency.cpp`: 各区間の頻度を直接数える方法との比較
