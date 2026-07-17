---
title: "有限手数ゲーム"
documentation_of: //cp/game/finite.hpp
---

# 有限手数ゲーム


## 概要

グラフ上の駒を指定回数動かし、最後の頂点に定めた真偽で勝敗が決まる完全情報零和ゲームを解く。
真側の手番は行き先のOR、偽側の手番はANDを取る。

## 厳密な定義

- `finitegameresult`: 有限手数の真偽ゲーム結果。winning[v]は開始頂点vで真側が勝つことを表す。
- `finite_horizon_game`: O(turns*(n+m))。turn_maximizes(turn)で各手番のOR・ANDを指定する有限手数ゲームを解く。

## Include

```cpp
#include "game/finite.hpp"

auto result = finite_horizon_game(graph, terminal_winning, turns);
if (result[start]) cout << "first\n";
```

既定では0手目のplayerが真を最大化し、以降は最大化・最小化を交互に行う。
`first_maximizes=false`なら最小化側から始める。

交互でない手番列では、`turn_maximizes(turn)`を渡す。

```cpp
auto result = finite_horizon_game(graph, terminal, turns,
    [&](int turn) { return controller[turn] == 0; });
```

頂点に出辺がない場合、最大化手番ではfalse、最小化手番ではtrueとなる。
「必ず一手動かす」ゲームでこの挙動を避けたい場合は、全頂点に出辺があることを確認する。

## 計算量

頂点数`n`、辺数`m`、手数`turns`として`O(turns(n+m))`時間、`O(n)`領域。
辺重みは使用しない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `finitegameresult`

```cpp
struct finitegameresult
```

有限手数の真偽ゲーム結果。winning[v]は開始頂点vで真側が勝つことを表す。

### `operator[]`

```cpp
bool operator[](int vertex) const
```

O(1)。vertexから開始して真側が勝つならtrueを返す。

### `finite_horizon_game`

```cpp
template <graph_type Graph, class TurnMaximizes> requires std::invocable<TurnMaximizes, int> finitegameresult finite_horizon_game( const Graph& graph, const std::vector<bool>& terminal_winning, int turns, TurnMaximizes turn_maximizes )
```

O(turns*(n+m))。turn_maximizes(turn)で各手番のOR・ANDを指定する有限手数ゲームを解く。

### `finite_horizon_game`

```cpp
template <graph_type Graph> finitegameresult finite_horizon_game( const Graph& graph, const std::vector<bool>& terminal_winning, int turns, bool first_maximizes = true )
```

O(turns*(n+m))。first_maximizesから真最大化・最小化を交互に行うゲームを解く。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC427 D - The Simple Game](https://atcoder.jp/contests/abc427/tasks/abc427_d):
  頂点文字が`A`ならterminalをtrueとし、Aliceから`2K`手交互に操作する。

`verify/atcoder_abc427_d.cpp`に提出用コードを収録し、公式sampleとの一致を確認済み。


`tests/property/game/finite.cpp`では、小さいランダムgraphを再帰minimaxと比較する。
