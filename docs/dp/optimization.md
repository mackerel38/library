---
title: "DP最適化"
documentation_of: //cp/dp/optimization.hpp
---

# DP最適化

- Header: `cp/dp/optimization.hpp`
- Symbol: `poe::divide_conquer_dp`, `poe::optimal_merge_cost`
- Status: experimental

## どんな問題に使えるか

`divide_conquer_dp`は層DPで最適遷移元が列に対して単調な場合、`optimal_merge_cost`は非負重みの
隣接併合費用に使う。どちらも単なる高速化の期待だけで適用してはいけない。単調性・四辺形不等式などの
必要条件を証明できない場合は愚直DPまたは別のデータ構造を使う。

## 使い方

```cpp
#include "dp/optimization.hpp"

long long answer = poe::optimal_merge_cost(weights);
```

`optimal_merge_cost`は隣接群を総重量コストで併合する区間DPをKnuth最適化する。
`divide_conquer_dp`は、各列の最適遷移元が単調な層DPを分割統治最適化する。
単調性が成立しない問題へ使うと誤答になるため、利用者が証明できる場合だけ使う。

## 計算量

- `optimal_merge_cost`: `O(n^2)`時間、`O(n^2)`領域。
- `divide_conquer_dp`: `O(KN log N)`時間、`O(N)`領域。

## 使う前の確認と典型的な誤り

`divide_conquer_dp`では、遷移可能範囲が`before <= column`と一致し、各層で最適な`before`が
`column`に対して単調である必要がある。初期DPを加えた後のtie breakまで含めて条件を確認する。
到達不能値`inf`へ費用は加えないが、有限値同士の加算overflowは利用側で防ぐ。

`optimal_merge_cost`は入力順を保つ隣接併合専用で、任意の二群を併合するHuffman codingとは異なる。
重みは非負で、復元順序は返さず最小費用だけを返す。

各行の行列最小値だけ必要なら`dp/monotone.hpp`、区分線形凸な値関数なら
`dp/slopetrick.hpp`も候補になる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `divide_conquer_dp`

```cpp
template <class T, class Cost> std::vector<T> divide_conquer_dp( int rows, int columns, std::vector<T> initial, Cost cost, T inf )
```

O(rows * columns * log columns)。divide_conquer_dpで各行の最小遷移を単調性付きで計算する。
cost(row, previous, column)を返し、最適previousがcolumnに対して広義単調増加であること。

### `optimal_merge_cost`

```cpp
template <class T> T optimal_merge_cost(const std::vector<T>& weights)
```

O(n^2)。optimal_merge_cost(weights): 隣接する二群を総和コストで併合する最小費用を返す。
weightsは非負であること。Knuth最適化を用いる。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[EDPC N - Slimes](https://atcoder.jp/contests/dp/tasks/dp_n)に`optimal_merge_cost`を直接使える。

```cpp
std::cout << poe::optimal_merge_cost(slime_sizes) << '\n';
```

`verify/atcoder_dp_n.cpp`で2026-07-15にjudge AC確認済み。
`O(n^3)`の愚直区間DPとの固定seed差分も実施済み。
