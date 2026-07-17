---
title: "部分和"
documentation_of: //cp/dp/subsetsum.hpp
---

# 部分和

- Header: `cp/dp/subsetsum.hpp`
- Symbol: `poe::subsetsum`, `poe::bounded_subsetsum`, `poe::subsetsumstatistics`, `poe::subset_sum_less_by_size`, `poe::nonadjacentsubsetsums`, `poe::nonadjacent_subset_sums`, `poe::count_nonadjacent_subsets_mod`
- Status: experimental

## どんな問題に使えるか

非負整数を選んで作れる総和の可否だけが必要な問題に向く。各要素を高々一回なら`subsetsum`、
値ごとに使用上限があるなら`bounded_subsetsum`を使う。個数や最大価値も同時に管理する問題、
負数を含む問題、選んだ要素の復元が必要な問題にはこのAPIだけでは足りない。

## 使い方

```cpp
#include "dp/subsetsum.hpp"

auto possible = poe::subsetsum(scores, 10000);
int kinds = std::ranges::count(possible, true);
```

`subsetsum`は各要素を高々一度使う0/1部分和を、`bounded_subsetsum`は値ごとの使用個数を
制限した部分和を返す。値は非負、個数制限付きでは値が正でなければならない。

## 戻り値とよくある誤り

戻り値の長さは`limit + 1`で、`possible[x]`が和`x`を作れるかを表す。和0は常に作れる。

- `subsetsum`へ負数を渡さない。
- `bounded_subsetsum`の`value`と`count`は同じ長さにし、値0を渡さない。
- 組合せ数や最大価値が必要な問題へ可否bitsetを使わない。
- 復元情報は保持しない。選んだ要素が必要なら親情報を持つ別DPを書く。

価値最大化なら`dp/knapsack.hpp`、部分和を畳み込みとして数えるなら`fps`カテゴリを検討する。

## 計算量

- `subsetsum`: `O(nL / 64)`時間、`O(L)`領域。ただし返却用展開も`O(L)`。
- `bounded_subsetsum`: `O(nL)`時間、`O(L)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `subsetsum`

```cpp
inline std::vector<bool> subsetsum(const std::vector<int>& values, int limit)
```

O(n * limit / 64)。subsetsum(values, limit): 0/1部分和で作れる0以上limit以下の和を返す。

### `bounded_subsetsum`

```cpp
inline std::vector<bool> bounded_subsetsum( const std::vector<int>& values, const std::vector<int>& counts, int limit )
```

O(n * limit)。bounded_subsetsum(value, count, limit): 個数制限付き部分和の可否を返す。

### `subsetsumstatistics`

```cpp
struct subsetsumstatistics
```

subset_sum_less_by_sizeの結果。count[k]はk個選ぶ組数、sum[k]はその部分集合和の総計。

### `subset_sum_less_by_size`

```cpp
inline subsetsumstatistics subset_sum_less_by_size( const std::vector<long long>& values, long long limit )
```

O(n 2^(n/2))時間・O(2^(n/2))領域。要素数別に部分集合和がlimit未満の個数と和を返す。

### `nonadjacentsubsetsums`

```cpp
template <class T> struct nonadjacentsubsetsums
```

非隣接部分集合和列挙。last_selectedは最後の要素を選ぶ場合、allは全場合の剰余。

### `nonadjacent_subset_sums`

```cpp
template <class T> nonadjacentsubsetsums<T> nonadjacent_subset_sums( const std::vector<T>& values, T modulo )
```

O(F_n)。path上で隣接要素を同時に選ばない部分集合和をmoduloで割った剰余を列挙する。

### `count_nonadjacent_subsets_mod`

```cpp
template <class T> unsigned long long count_nonadjacent_subsets_mod( const std::vector<T>& values, T modulo, T target = T
```

O(F_(n/2) log F_(n/2))。非隣接部分集合のうち和≡target(mod modulo)となる個数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Typical DP Contest A - コンテスト](https://atcoder.jp/contests/tdpc/tasks/tdpc_contest)に直接使える。

```cpp
auto possible = poe::subsetsum(points, std::reduce(points.begin(), points.end()));
std::cout << std::ranges::count(possible, true) << '\n';
```

`verify/atcoder_tdpc_contest.cpp`で2026-07-15にjudge AC確認済み。
固定seedの愚直差分も実施済み。

## 要素数別のMITM統計

`subset_sum_less_by_size(values, limit)`は、非負整数を40個以下受け取り、各`k`について
「ちょうど`k`個選び、部分集合和が`limit`未満になる部分集合の個数」と「それらの部分集合和の総計」を返す。
結果は`count[k]`と`sum[k]`に入り、総計は`__int128_t`で保持する。

半分全列挙とsortを用い、計算量`O(n 2^(n/2))`、領域`O(2^(n/2))`。
部分集合の最大和は`long long`に収まる必要がある。期待値、ランダム順列のprefix集合、
選択個数別の閾値集計に使い、最大価値だけ欲しい場合は`knapsack01_mitm`を使う。

## 非隣接部分集合和

`nonadjacent_subset_sums(values, modulo)`は、path状に並ぶ要素から隣接する二要素を同時に選ばない
部分集合について、和の剰余をすべて列挙する。`all`に全場合、`last_selected`に最後の要素を
選んだ場合が入る。重複する剰余も選び方ごとに残る。

```cpp
auto sums = nonadjacent_subset_sums(values, modulo);
auto count = count_nonadjacent_subsets_mod(values, modulo, target);
```

`count_nonadjacent_subsets_mod`は列を半分に分け、左右の剰余を照合する。分割境界の二要素を
両方選んだ組を`last_selected`同士の組から引く。長さ`n`の列挙数を
`F_0=1,F_1=2,F_n=F_(n-1)+F_(n-2)`とすると、時間
`O(F_(ceil(n/2)) log F_(ceil(n/2)))`、領域`O(F_(ceil(n/2)))`。

[AtCoder ABC427 F - Not Adjacent](https://atcoder.jp/contests/abc427/tasks/abc427_f)では、
`target=0`として直接使う。`verify/atcoder_abc427_f.cpp`で公式sample 3件を確認済み。
judge ACは未確認。

[ABC464 F - Random Vault Heist](https://atcoder.jp/contests/abc464/tasks/abc464_f)では、
閾値未満のprefix集合について要素数別の個数と金額総計を集約する。`verify/atcoder_abc464_f.cpp`は
公式サンプル3件との一致を確認済みだが、judgeへの提出確認は未実施。
