---
title: "制約付きmultiset順列と1回swap列の辞書順"
documentation_of: //cp/algorithm/permutation.hpp
---

# 制約付きmultiset順列と1回swap列の辞書順

- Header: `cp/algorithm/permutation.hpp`
- Symbol: `poe::count_permutations_max_drop`, `poe::one_swap_lexicographic_pairs`, `poe::path_order_positions`
- Status: experimental

## 隣接する減少幅を制限したmultiset順列

`count_permutations_max_drop<modulus>(values, maximum_drop)`は、重複する値を区別しない
相異なる順列のうち、すべての隣接項で`next >= current - maximum_drop`となる個数を返す。

```cpp
#include "algorithm/permutation.hpp"

auto count = count_permutations_max_drop<998244353>(values, maximum_drop);
```

値を昇順に挿入すると、現在値から`maximum_drop`以内にある既挿入要素の直後と末尾だけが
挿入位置になる。この位置への同値要素の重複組合せを掛け合わせる。
`O(n log n)`時間、`O(n)`領域。`maximum_drop`は非負、`n < modulus`が必要。

[AtCoder ABC431 F - Almost Sorted 2](https://atcoder.jp/contests/abc431/tasks/abc431_f)へ直接使える。
`verify/atcoder_abc431_f.cpp`で公式sampleを確認済み。judge ACは未確認。

## 1回swap後の列を辞書順に並べる

`one_swap_lexicographic_pairs(values, ranks)`は、`left < right`の全組について一度だけswapした列を
辞書順に並べ、各0-indexed順位に対応する`{left,right}`を返す。同じ列を作る異なるswapも別々に数え、
同順位の列にはそのいずれかを返す。

```cpp
vector<long long> ranks = {0, 10, 100};
auto pairs = one_swap_lexicographic_pairs(values, ranks);
```

元より小さくなるswapは`(left, values[right], -right)`、大きくなるswapは
`(-left, values[right], right)`の順になる性質を使い、offline queryをFenwick Treeで処理する。
`O((n+q) log n)`時間、`O(n+q)`領域。入力列長は2以上で、順位は
`[0,n(n-1)/2)`でなければならない。

[AtCoder ABC431 G - One Time Swap 2](https://atcoder.jp/contests/abc431/tasks/abc431_g)へ直接使える。
`verify/atcoder_abc431_g.cpp`で公式sampleを確認済み。judge ACは未確認。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_permutations_max_drop`

```cpp
template <int modulus> staticmodint<modulus> count_permutations_max_drop( std::vector<long long> values, long long maximum_drop )
```

O(n log n)。multisetの相異なる順列で隣接項next>=current-maximum_dropとなる個数を返す。

### `one_swap_lexicographic_pairs`

```cpp
template <class T> std::vector<std::pair<int, int>> one_swap_lexicographic_pairs( const std::vector<T>& values, const std::vector<long long>& ranks )
```

O((n+q)log n)。各0-indexed rankについて、1回swap後の列を辞書順に並べたrank番目の位置組を返す。
同じ列を作るswapも別々に数え、同順位の列にはそのいずれかを返す。

### `pathorderpositions`

```cpp
struct pathorderpositions
```

path_order_positionsの結果。ranges[value]は可能な0-indexed位置の閉区間、counts[position]は候補数。

### `path_order_positions`

```cpp
template<std::ranges::input_range Range, class Forward> pathorderpositions path_order_positions(const Range& relations, Forward is_forward)
```

O(n)。隣接value間だけに前後制約がある順列で、各valueの可能位置区間と各位置の候補数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC431 F - Almost Sorted 2](https://atcoder.jp/contests/abc431/tasks/abc431_f):
  `count_permutations_max_drop`を直接使う。
- [AtCoder ABC431 G - One Time Swap 2](https://atcoder.jp/contests/abc431/tasks/abc431_g):
  query順位を0-indexedへ直して`one_swap_lexicographic_pairs`へ一括して渡す。

どちらも公式sample確認済みで、judge ACは未確認。

## 有向path制約下の可能位置

`path_order_positions(relations, is_forward)`は、value `i`と`i+1`のどちらを先に置くかだけが
各relationで指定された順列について、各valueが取りうる位置の閉区間`ranges[value]`と、
各位置へ置けるvalue数`counts[position]`を返す。`is_forward(relation)`がtrueなら`i`が先になる。

```cpp
auto result = path_order_positions(directions, [](char c) { return c == 'R'; });
```

同じ向きが左右へ連続する長さから、必ず前・後に置かれるvalue数を求める。
計算量・領域はともに`O(n)`。各valueの可能位置が区間になるpath posetを対象とし、
一般DAGの順序制約には使えない。

[AtCoder ABC430 F - Back and Forth Filling](https://atcoder.jp/contests/abc430/tasks/abc430_f)へ直接使える。
`verify/atcoder_abc430_f.cpp`で公式sampleを確認済み。judge ACは未確認。

## 検証

- `tests/property/algorithm/permutation.cpp`: 小さいmultisetの全順列、全swap列の実体ソートとの比較
- `tests/api/algorithm/permutation.cpp`: 境界と基本的な呼び出し
