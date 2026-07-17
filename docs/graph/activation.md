---
title: "同期単調activation"
documentation_of: //cp/graph/activation.hpp
---

# 同期単調activation

  `poe::gridactivationresult`, `poe::grid_synchronous_activation`

## 概要

固定graphまたはgridで、無効な頂点が条件を満たすと有効になり、一度有効になった頂点は戻らない過程を扱う。
各ラウンドの開始時点で条件を判定し、条件を満たした頂点を一斉に有効化する。

## 厳密な定義

- `activationresult`: 同期単調activationの結果。roundは初期頂点が0、未到達が-1。
- `gridactivationresult`: grid同期単調activationの結果。round[row][column]は初期マスが0、未到達が-1。
- `synchronous_activation`: O(n+m)。各round開始時にcan_activate(vertex,active近傍数)を満たす頂点を一斉に有効化する。
- `grid_synchronous_activation`: O(hwK)。gridで各round開始時に条件を満たすマスをdirections近傍により一斉有効化する。

## Include

```cpp
#include "graph/activation.hpp"

auto result = synchronous_activation(graph, initially_active,
    [&](int vertex, int active_neighbors) {
        return active_neighbors >= threshold[vertex];
    });
```

`result[vertex]`は有効になったラウンドで、初期頂点は`0`、最後まで無効なら`-1`。
`contains(vertex)`と最終的な`active_count`も返す。

gridでは座標predicateを直接渡せる。

```cpp
auto result = grid_synchronous_activation(h, w,
    [&](int r, int c) { return grid[r][c] == '#'; },
    [&](int r, int c, int active_neighbors) {
        return active_neighbors == 1;
    });
```

上下左右以外を使う場合は、最後の引数へ`grid8`または独自の方向配列を渡す。

## 計算量と前提

graph版は`O(n+m)`時間・`O(n)`追加領域、方向数`K`のgrid版は`O(hwK)`時間・`O(hw)`領域。
各頂点は一度だけ有効になり、その隣接辺を一度だけ次ラウンド候補へ反映する。

`can_activate`は現在の有効近傍数だけに依存し、過程全体で有効状態は単調増加しなければならない。
条件自体は近傍数について単調でなくてもよく、`active_neighbors == 1`のような条件にも使える。
同期更新なので、一つのラウンド内で新しく有効になった頂点は同ラウンドの他頂点の判定へ影響しない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `activationresult`

```cpp
struct activationresult
```

同期単調activationの結果。roundは初期頂点が0、未到達が-1。

### `contains`

```cpp
bool contains(int vertex) const
```

O(1)。vertexが最終的に有効ならtrueを返す。

### `operator[]`

```cpp
int operator[](int vertex) const
```

O(1)。vertexが有効になったラウンドを返す。未到達は-1。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `gridactivationresult`

```cpp
struct gridactivationresult
```

grid同期単調activationの結果。round[row][column]は初期マスが0、未到達が-1。

### `contains`

```cpp
bool contains(int row, int column) const
```

O(1)。(row,column)が最終的に有効ならtrueを返す。

### `height`

```cpp
int height() const noexcept
```

O(1)。行数を返す。

### `width`

```cpp
int width() const noexcept
```

O(1)。列数を返す。

### `synchronous_activation`

```cpp
template <graph_type Graph, class CanActivate> activationresult synchronous_activation( const Graph& graph, const std::vector<bool>& initially_active, CanActivate can_activate )
```

O(n+m)。各round開始時にcan_activate(vertex,active近傍数)を満たす頂点を一斉に有効化する。

### `grid_synchronous_activation`

```cpp
template <class InitiallyActive, class CanActivate, std::size_t K> gridactivationresult grid_synchronous_activation( int height, int width, InitiallyActive initially_active, CanActivate can_activate, const std::array<gridpoint, K>& directions )
```

O(hwK)。gridで各round開始時に条件を満たすマスをdirections近傍により一斉有効化する。

### `grid_synchronous_activation`

```cpp
template <class InitiallyActive, class CanActivate> gridactivationresult grid_synchronous_activation( int height, int width, InitiallyActive initially_active, CanActivate can_activate )
```

O(hw)。上下左右4近傍で同期単調activationを行う。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC425 D - Ulam-Warburton Automaton](https://atcoder.jp/contests/abc425/tasks/abc425_d)では、
初期黒マスを有効とし、有効な4近傍がちょうど一つの白マスを一斉に有効化する。
`verify/atcoder_abc425_d.cpp`で公式sample 3件一致。

API testではpath graphのthreshold activationを確認し、property testでは小gridの全盤面について
毎回全マスを走査する同期愚直法と照合する。
