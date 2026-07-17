---
title: "DAG上のimpartial game"
documentation_of: //cp/game/dag.hpp
---

# DAG上のimpartial game


## 概要

DAGの頂点を局面、出辺を一手とする通常プレイのimpartial gameに使う。遷移先を持たない局面で
手番を迎えた側が負ける。Grundy数を求めれば、一つのゲームの勝敗だけでなく複数ゲームの直和も
XORで扱える。

## 厳密な定義

- `dag_grundy`: O(n+m)。DAG上で手を辺遷移とするimpartial gameの各頂点のGrundy数を返す。
- `dag_winning_positions`: O(n+m)。DAG上の通常プレイで各頂点が先手必勝ならtrueを返す。

## Include

```cpp
#include "game/dag.hpp"

auto grundy = poe::dag_grundy(graph);
auto winning = poe::dag_winning_positions(graph);
bool first_wins = winning[start];
```

入力graphがDAGであることが必要である。閉路を含むゲームは引き分けや無限手順を別途定義する必要があり、
このAPIの対象外とする。

## 計算量

`O(n+m)`時間、`O(n)`追加領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `dag_grundy`

```cpp
template <directed_graph_type Graph> std::vector<int> dag_grundy(const Graph& graph)
```

O(n+m)。DAG上で手を辺遷移とするimpartial gameの各頂点のGrundy数を返す。

### `dag_winning_positions`

```cpp
template <directed_graph_type Graph> std::vector<bool> dag_winning_positions(const Graph& graph)
```

O(n+m)。DAG上の通常プレイで各頂点が先手必勝ならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC433 G - Substring Game](https://atcoder.jp/contests/abc433/tasks/abc433_g)では、
Suffix Automatonの状態を局面、文字追加遷移を手とするDAGを作る。初期状態0の勝敗が答えになる。
`verify/atcoder_abc433_g.cpp`で公式sampleを確認済み。
