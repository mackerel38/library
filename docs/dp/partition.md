---
title: "連続フェーズ分割DP"
documentation_of: //cp/dp/partition.hpp
---

# 連続フェーズ分割DP


## 概要

長さ`n`の列を、順序固定の`p`個の非空な連続区間へ分割する。`score[k][i]`を位置`i`を
フェーズ`k`へ入れた得点として、その総和の最大値を返す。境界ごとの追加コストや、空フェーズを許す問題は
このAPIの対象外。

## 厳密な定義

- `max_phase_partition`: 列を各フェーズ一個以上の連続区間へ分け最大得点を返す: max_phase_partition(score); O(pn)。
- `parenthesized_results`: O(n^3 s^2)。二項演算を全ての括弧付けで適用して到達できる状態をbit集合で返す。s<=64。

## Include

```cpp
#include "dp/partition.hpp"

long long answer = poe::max_phase_partition(score);
```

`O(pn)`時間・`O(n)`領域。`score`は`p x n`で、`n >= p >= 1`とする。

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
