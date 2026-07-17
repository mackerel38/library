---
title: "動的順序統計multiset"
documentation_of: //cp/structure/orderstatistic.hpp
---

# 動的順序統計multiset

- Header: `cp/structure/orderstatistic.hpp`
- Symbol: `poe::orderstatisticmultiset`
- Status: experimental

## どんな問題に使えるか

更新で現れ得る値を先に列挙できるとき、重複を許す値集合へ追加・削除しながら、
順位、個数、順位範囲の総和を求める。

```cpp
#include "structure/orderstatistic.hpp"

poe::orderstatisticmultiset<long long> data(all_possible_values);
data.add(value);
data.erase(old_value);
long long median = data.kth((data.size() - 1) / 2);
long long low_sum = data.sum_smallest(k);
long long bounded_sum = data.clamp_sum(lower, upper);
```

構築は座標種類数を`u`として`O(u log u)`。次の操作はすべて`O(log u)`。

- `add(value, count)`、`erase(value, count)`
- `count(value)`、`count_less(value)`、`sum_less(value)`
- 全要素へ`max(lower,min(upper,x))`を適用した総和`clamp_sum(lower, upper)`
- 0-indexedの`kth(index)`
- `sum_smallest(count)`、`sum_largest(count)`

`size`、`empty`、全体の`sum`は`O(1)`。
構築時の`universe`にない値は追加できない。値と個数の積・総和が`T`に収まる必要がある。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `orderstatisticmultiset`

```cpp
template <class T> struct orderstatisticmultiset
```

座標圧縮済み動的順序統計multiset: orderstatisticmultiset<long long> data(universe)。

### `orderstatisticmultiset`

```cpp
explicit orderstatisticmultiset(std::vector<T> universe) : values_(std::move(universe)), counts_(prepare(values_)), sums_(values_.size())
```

O(u log u)。追加され得る全値universeから空のmultisetを構築する。

### `add`

```cpp
void add(const T& value, long long count = 1)
```

O(log u)。valueをcount個追加する。

### `erase`

```cpp
void erase(const T& value, long long count = 1)
```

O(log u)。valueをcount個削除する。十分な個数が存在すること。

### `count`

```cpp
long long count(const T& value) const
```

O(log u)。valueの個数を返す。

### `count_less`

```cpp
long long count_less(const T& value) const
```

O(log u)。value未満の要素数を返す。

### `sum_less`

```cpp
T sum_less(const T& value) const
```

O(log u)。value未満の要素の総和を返す。

### `clamp_sum`

```cpp
T clamp_sum(const T& lower, const T& upper) const
```

O(log u)。全要素へmax(lower,min(upper,x))を適用した総和を返す。lower>upperならlower*size。

### `kth`

```cpp
T kth(long long index) const
```

O(log u)。0-indexedでindex番目に小さい値を返す。

### `sum_smallest`

```cpp
T sum_smallest(long long count) const
```

O(log u)。小さい方からcount個の総和を返す。

### `sum_largest`

```cpp
T sum_largest(long long count) const
```

O(log u)。大きい方からcount個の総和を返す。

### `size`

```cpp
long long size() const noexcept
```

O(1)。要素数を返す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。空ならtrueを返す。

### `sum`

```cpp
T sum() const
```

O(1)。全要素の総和を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC440 F - Egoism](https://atcoder.jp/contests/abc440/tasks/abc440_f)では、
全体と丁寧さ2の馬の二つのmultisetを管理する。必要個数の最小和と、条件を満たさない場合の
境界要素交換を`sum_smallest`と`kth`で計算できる。
`verify/atcoder_abc440_f.cpp`で公式sampleを確認済み。judge ACは未確認。

[AtCoder ABC432 E - Clamp](https://atcoder.jp/contests/abc432/tasks/abc432_e)では、
一点更新を`erase`と`add`で処理し、各queryを`clamp_sum(l, r)`で直接求める。
`verify/atcoder_abc432_e.cpp`で公式sampleを確認済み。judge ACは未確認。

property testでは`std::multiset`をソートした愚直列と各操作を照合している。
