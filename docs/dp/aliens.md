---
title: "Aliens trick"
documentation_of: //cp/dp/aliens.hpp
---

# Aliens trick


## 概要

「ちょうどK個選ぶ／K区間に分ける」最小化DPで、個数制約を一個あたりのペナルティへ移すと
高速なDPになる場合に使う。ペナルティを増やすほど最適解の個数が単調減少し、値が離散凸である必要がある。

## 厳密な定義

- `aliens_result`: Aliens trickのペナルティ探索結果。
- `aliens_min`: O(log(high-low) * solve)。最小化DPで区切り数targetの値をAliens trickで復元する。 solve(p)は「元費用+p*個数」の最小値と、その中で最小の個数を返す。 penalty増加に対してcountが広義単調減少し、離散凸性によりvalue-penalty*targetが成立すること。
- `aliens_max`: O(log(high-low) * solve)。最大化DPで個数targetの値をAliens trickで復元する。 solve(p)は「元価値-p*個数」の最大値と、その中で最小の個数を返す。 penalty増加に対してcountが広義単調減少し、離散凹性によりvalue+penalty*targetが成立すること。

単に個数が単調になるだけでは不十分で、双対ギャップなく`penalized-penalty*K`で復元できることを
証明する必要がある。最大化問題やtieの選び方が逆の場合は変換してから使う。

## Include

```cpp
#include "dp/aliens.hpp"
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `aliens_result`

```cpp
template <class T> struct aliens_result
```

Aliens trickのペナルティ探索結果。

### `aliens_min`

```cpp
template <class T, class Solve> aliens_result<T> aliens_min(T low, T high, int target, Solve solve)
```

O(log(high-low) * solve)。最小化DPで区切り数targetの値をAliens trickで復元する。
solve(p)は「元費用+p*個数」の最小値と、その中で最小の個数を返す。
penalty増加に対してcountが広義単調減少し、離散凸性によりvalue-penalty*targetが成立すること。

### `aliens_max`

```cpp
template <class T, class Solve> aliens_result<T> aliens_max(T low, T high, int target, Solve solve)
```

O(log(high-low) * solve)。最大化DPで個数targetの値をAliens trickで復元する。
solve(p)は「元価値-p*個数」の最大値と、その中で最小の個数を返す。
penalty増加に対してcountが広義単調減少し、離散凹性によりvalue+penalty*targetが成立すること。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

## 使う前の確認

- 探索区間に答えを復元できる境界の罰金が含まれていること。
- `solve(p)`の個数が`p`の増加に対して広義単調減少すること。
- 目的値が個数について離散凸または離散凹で、双対ギャップがないこと。
- `penalty * target`と罰金付き目的値が`T`に収まること。

個数の単調性だけを観察して適用するのが典型的な誤りである。同値解で大きい個数を返す設計へ
変える場合は、二分探索の不等号も一緒に設計し直す必要がある。

罰金固定後のDPがmin-plus/max-plus畳み込みになる場合は`math/tropicalconvolution.hpp`も候補になる。

[NDPC O - ゲーム](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_o)の満点想定解で利用される。
このヘッダはペナルティ二分探索だけを担当し、問題固有の`solve(penalty)`は利用側で実装する。
小さい離散凸列で全個数走査と照合済み。
