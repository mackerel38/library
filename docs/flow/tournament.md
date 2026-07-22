---
title: "総当たり戦の単独優勝可能性"
documentation_of: //cp/flow/tournament.hpp
---

# 総当たり戦の単独優勝可能性

## 概要

一部の試合結果だけが決まっている総当たり戦について、ある選手が最終的に単独優勝できるか判定します。
可能なら、未決定試合の勝敗を補った全試合結果も一つ構成できます。

## 厳密な定義

$n$ 選手の各unordered pairはちょうど一度対戦し、引き分けはありません。
既知結果を有向対 $(w,l)$、すなわち選手 $w$ が選手 $l$ に勝った試合として与えます。

`complete_roundrobin_for_unique_winner` は、既知結果をすべて保ったまま、指定選手 $p$ の
最終勝数が他のすべての選手より真に大きくなるよう未決定試合を埋めます。存在しなければ
`possible == false` を返します。

$p$ が関わる未決定試合はすべて $p$ の勝ちとしてよいので、これで $p$ の最終勝数 $W$ を
確定します。残る各試合をどちらの選手の1勝へ割り当てるかを、各選手の上限 $W-1$ を持つ
最大流として解きます。

一選手の構成は $O(n^3)$ 時間・$O(n^2)$ 領域、全選手の列挙は $O(n^4)$ 時間・
$O(n^2)$ 領域です。

## Include

```cpp
#include "flow/tournament.hpp"
```

## 使い方

### `complete_roundrobin_for_unique_winner`

```cpp
auto completion = complete_roundrobin_for_unique_winner(
    players, decided_results, winner);
if (completion.possible) {
    for (auto [win, lose] : completion.results) {
        // 全n(n-1)/2試合の結果
    }
}
```

選手番号は0-indexedです。`decided_results` に自己対戦や同じunordered pairを二度入れては
いけません。成功時の `results` は既知結果を含む全試合を `{winner, loser}` で返します。

### `possible_unique_roundrobin_winners`

```cpp
vector<int> winners = possible_unique_roundrobin_winners(
    players, decided_results);
```

単独優勝できる選手番号を昇順で返します。誰も単独優勝できなければ空です。

## 頻出事故

- 同率首位を許しません。他選手の最終勝数上限は、候補者の勝数より1小さい値です。
- 既知結果を逆転させることはありません。
- 候補者自身の未決定試合はすべて勝ちにしてよいですが、既知の負けはそのままです。
- 勝数だけを制約する総当たり戦用です。試合ごとの重みや引き分けには対応しません。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `roundrobincompletion`

```cpp
struct roundrobincompletion
```

総当たり戦を単独優勝で完了できるかと、その全試合の{勝者,敗者}を表す。

### `complete_roundrobin_for_unique_winner`

```cpp
inline roundrobincompletion complete_roundrobin_for_unique_winner( int players, const std::vector<std::pair<int, int>>& decided_results, int winner )
```

O(n^3)時間・O(n^2)領域。既知結果を保ち、winnerが単独優勝する総当たり戦を一つ構成する。

### `possible_unique_roundrobin_winners`

```cpp
inline std::vector<int> possible_unique_roundrobin_winners( int players, const std::vector<std::pair<int, int>>& decided_results )
```

O(n^4)時間・O(n^2)領域。既知結果から単独優勝できる選手を昇順で返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC241 G - Round Robin](https://atcoder.jp/contests/abc241/tasks/abc241_g)では、
入力の選手番号を0-indexedへ直して `possible_unique_roundrobin_winners` に渡し、返り値へ1を
足して出力するだけです。

```cpp
const auto winners = possible_unique_roundrobin_winners(n, results);
for (int winner : winners) cout << winner + 1 << ' ';
```
