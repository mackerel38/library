---
title: "連続分割DP"
documentation_of: //cp/dp/partition.hpp
---

# 連続分割DP


## 概要

列を連続区間へ分けるDPのうち、フェーズごとの得点最大化、各区間の値域を掛ける分割の総和、
任意の括弧付けによる二項演算を扱う。

## 厳密な定義

- `max_phase_partition`: 列を各フェーズ一個以上の連続区間へ分け最大得点を返す: max_phase_partition(score); O(pn)。
- `sum_partition_range_products<T>`: 長さ$n$の列$A$の全ての連続分割$B=(B_1,\ldots,B_k)$について、
  $$\sum_B\prod_{i=1}^{k}(\max B_i-\min B_i)$$
  を返す。空列の分割は一通りとし、値は$1$。$O(n)$時間・$O(n)$領域。
- `parenthesized_results`: O(n^3 s^2)。二項演算を全ての括弧付けで適用して到達できる状態をbit集合で返す。s<=64。

## Include

```cpp
#include "dp/partition.hpp"

long long answer = poe::max_phase_partition(score);
```

`O(pn)`時間・`O(n)`領域。`score`は`p x n`で、`n >= p >= 1`とする。

`sum_partition_range_products<T>(values)`では、`T`に答えの型を指定する。各末尾について区間の最大値と
最小値の寄与を単調スタックでまとめるため、分割境界を一つずつ試さない。

```cpp
using mint = poe::staticmodint<998244353>;
mint answer = poe::sum_partition_range_products<mint>(a);
```

## API・計算量

## 任意の括弧付けによる二項演算

`parenthesized_results(values, operation)`は、状態列へ二項演算を任意の順序で隣接適用したときの
最終状態候補を64bit集合で返す。これは二項演算が結合的でない場合の区間DPを隠蔽する。
状態数を`s<=64`として`O(n^3 s^2)`時間・`O(n^2)`領域。

```cpp
auto possible = poe::parenthesized_results(values, operation);
bool can_make_target = possible >> target & 1;
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `max_phase_partition`

```cpp
template <class T> T max_phase_partition(const std::vector<std::vector<T>>& score)
```

列を各フェーズ一個以上の連続区間へ分け最大得点を返す: max_phase_partition(score); O(pn)。

### `sum_partition_range_products`

```cpp
template <class T, class Value> T sum_partition_range_products(const std::vector<Value>& values)
```

O(n)時間・O(n)領域。全連続分割について各区間の(max-min)を掛けた値の総和を返す。

### `parenthesized_results`

```cpp
inline unsigned long long parenthesized_results( const std::vector<int>& values, const std::vector<std::vector<int>>& operation )
```

O(n^3 s^2)。二項演算を全ての括弧付けで適用して到達できる状態をbit集合で返す。s<=64。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC438 D - Tail of Snake](https://atcoder.jp/contests/abc438/tasks/abc438_d)では、
頭・胴・尾の三行を`score`として渡す。
`verify/atcoder_abc438_d.cpp`は公式サンプル一致を確認済み。

[AtCoder ABC418 G - Binary Operation](https://atcoder.jp/contests/abc418/tasks/abc418_g)では、
短い二進列が演算結果`1`へ縮約可能かを判定するmembership oracleとして使う。
`verify/atcoder_abc418_g.cpp`で公式2 sampleを確認済み。

[AtCoder ABC234 G - Divide a Sequence](https://atcoder.jp/contests/abc234/tasks/abc234_g)では、
`sum_partition_range_products`へ数列をそのまま渡す。
