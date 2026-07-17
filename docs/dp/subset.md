---
title: "集合分割DP"
documentation_of: //cp/dp/subset.hpp
---

# 集合分割DP

## 概要

重複を除いた全削除順・最小費用集合分割・零和最大分割のbit DPを扱う。

## 厳密な定義

- `count_distinct_deletion_orders`: O(n 2^n)時間・O(2^n)空間。同じ残存列への削除を重複させず、全削除順の個数を返す。
- `subsetpartitionresult`: 集合分割DPの結果。costが最小費用、groupsが選んだ非空部分集合のbit mask列。
- `zerosumpartitionresult`: 零和集合分割の結果。groupsは和が0の非空部分集合bit mask列で、個数が最大。
- `max_zero_sum_partition`: O(n 2^n)時間・O(2^n)空間。全体和が0なら零和部分集合への最大個数分割を復元する。
- `subset_partition_min`: O(3^n)時間・O(2^n)空間。各非空部分集合の費用から、全要素を分割する最小費用と分割を返す。


## Include

```cpp
#include "dp/subset.hpp"
```

`subset_partition_min(n, group_cost, inf)`は、`n`個の要素を重なりのない非空部分集合へ分割し、
各グループの費用の総和を最小化する。`group_cost[mask]`には`mask`を一グループにした費用を入れ、
許されないグループには`inf`を入れる。

結果の`cost`が最小費用、`groups`が実際に選んだグループのbit mask列である。
分割不能なら`cost == inf`かつ`groups.empty()`になる。計算量は`O(3^n)`、空間は`O(2^n)`。

各遷移では、未分割集合の最下位要素を含むグループだけを調べる。同じ集合分割をグループの
並び順だけ変えて重複して調べず、復元用の選択も内部で保存する。

## 向いている問題

- 各グループが条件を満たすために必要な費用を、部分集合ごとに前計算できる。
- 全要素をちょうど一度ずつ、任意個のグループへ分ける。
- グループ間に順序や相互作用がなく、全体費用が各グループ費用の和になる。

グループを作る順序が答えに影響するDPや、あるグループの選択で別グループの費用が変わる問題には使えない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_distinct_deletion_orders`

```cpp
template <class T, std::ranges::input_range Range> T count_distinct_deletion_orders(const Range& sequence)
```

O(n 2^n)時間・O(2^n)空間。同じ残存列への削除を重複させず、全削除順の個数を返す。

### `subsetpartitionresult`

```cpp
template<class T> struct subsetpartitionresult
```

集合分割DPの結果。costが最小費用、groupsが選んだ非空部分集合のbit mask列。

### `zerosumpartitionresult`

```cpp
struct zerosumpartitionresult
```

零和集合分割の結果。groupsは和が0の非空部分集合bit mask列で、個数が最大。

### `max_zero_sum_partition`

```cpp
template <class T> zerosumpartitionresult max_zero_sum_partition(const std::vector<T>& values)
```

O(n 2^n)時間・O(2^n)空間。全体和が0なら零和部分集合への最大個数分割を復元する。

### `subset_partition_min`

```cpp
template<class T> subsetpartitionresult<T> subset_partition_min( int n, const std::vector<T>& group_cost, const T& inf )
```

O(3^n)時間・O(2^n)空間。各非空部分集合の費用から、全要素を分割する最小費用と分割を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder Beginner Contest 187 F - Close Group](https://atcoder.jp/contests/abc187/tasks/abc187_f)。
各部分集合がcliqueなら費用`1`、そうでなければ`inf`とすることで、最小clique分割を求められる。

```cpp
vector<int> cost(1 << n, inf);
for (int mask = 1; mask < (1 << n); ++mask) {
    if (is_clique[mask]) cost[mask] = 1;
}
auto result = subset_partition_min(n, cost, inf);
cout << result.cost << '\n';
```

`verify/atcoder_abc187_f.cpp`に提出用コードを用意し、公式サンプル4件との一致を確認済み。


## 重複を除いた全削除順

`count_distinct_deletion_orders<T>(sequence)`は、現在の列から一要素ずつ削除して空列にする過程を数える。
同じ現在列から異なる位置を削除して同じ次の列になる場合、その遷移は一度だけ数える。

```cpp
using mint = poe::modint998244353;
mint ways = poe::count_distinct_deletion_orders<mint>(sequence);
```

残っている元添字集合をbit maskにし、現在の残存列で同じ値が連続する部分からは最初の一つだけを
削除候補にする。計算量は`O(n 2^n)`時間・`O(2^n)`領域。

[AtCoder ABC425 F - Inserting Process](https://atcoder.jp/contests/abc425/tasks/abc425_f)は
挿入過程を逆向きの削除過程として数えられる。
`verify/atcoder_abc425_f.cpp`で公式sample 3件一致。

## 零和部分集合への最大分割

`max_zero_sum_partition(values)`は、全体和が0の列を和が0の非空部分集合へ分け、
グループ数が最大の分割をbit mask列として復元する。全体和が0でなければ空列を返す。

```cpp
vector<long long> deviation(n);
auto result = max_zero_sum_partition(deviation);
for (uint64_t group : result.groups) {
    // groupに属する値の和は0
}
```

要素を並べるbit DPで、prefix和が0になる回数を最大化する。`O(n 2^n)`時間、`O(2^n)`領域。
空列と分割不能の両方が空の`groups`になるため、必要なら呼び出し側で全体和を確認する。

[AtCoder ABC432 F - Candy Redistribution](https://atcoder.jp/contests/abc432/tasks/abc432_f)では、
各所持数から平均を引いた偏差を最大個数の零和groupへ分けると、最小操作数は
`n - groups.size()`になる。`verify/atcoder_abc432_f.cpp`で公式sampleの有効な最小解を確認済み。
