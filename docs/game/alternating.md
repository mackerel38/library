---
title: "循環を含む重み付き交互ゲーム"
documentation_of: //cp/game/alternating.hpp
---

# 循環を含む重み付き交互ゲーム

## 概要

有向グラフ上の駒を二人が交互に動かすゲームを解く。
一方はゲームを有限回で終わらせて費用を小さくし、もう一方は終了を妨げ、終了を避けられない場合は費用を大きくする。
閉路があっても、各頂点から両方の手番で開始した結果を一括して求められる。

## 厳密な定義

非負重み付き有向グラフ $G=(V,E)$ を考える。出辺がない頂点へ到達するとゲームは終了し、費用は通った辺の重みの総和とする。

- `gameplayer::minimize`は有限回で終了させることを最優先し、それが可能なら総費用を最小化する。
- `gameplayer::maximize`は終了させないことを最優先し、それが不可能なら総費用を最大化する。

両者は辺を一本選んで移動し、手番を交代する。`alternating_path_game`は各 $(v,p)$ について、
最適プレイで終了するなら総費用、終了しないなら `nullopt` を返す。

## Include

```cpp
#include "game/alternating.hpp"
```

## 使い方

```cpp
poe::directed_graph<long long> graph(4);
graph.add_edge(0, 1, 1);
graph.add_edge(1, 2, 1);
graph.add_edge(2, 0, 1);
graph.add_edge(1, 3, 1);

auto result = poe::alternating_path_game(graph);
auto answer = result.value(0, poe::gameplayer::minimize);
if (answer) cout << *answer << '\n';
else cout << "INFINITY\n";
```

### 結果の取得

```cpp
bool ends = result.finite(vertex, player);
const optional<Cost>& value = result.value(vertex, player);
```

`value`は有限終了する場合だけ値を持つ。辺重みは非負でなければならない。

## 計算量

頂点数を $n$、辺数を $m$ とすると、時間計算量は $O((n+m)\log n)$、空間計算量は $O(n+m)$。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `gameplayer`

```cpp
enum class gameplayer
```

alternating_path_gameの手番。minimizeは有限終了を優先して費用を最小化する。

### `alternatingpathgameresult`

```cpp
template <class Cost> struct alternatingpathgameresult
```

alternating_path_gameの結果。値がnulloptの局面は最適プレイで終了しない。

### `finite`

```cpp
bool finite(int vertex, gameplayer player) const
```

O(1)。vertexでplayerの手番から始めたゲームが有限回で終了するか返す。

### `value`

```cpp
const std::optional<Cost>& value(int vertex, gameplayer player) const
```

O(1)。有限なら最適プレイ時の総費用、終了しないならnulloptを返す。

### `alternating_path_game`

```cpp
template <directed_graph_type Graph> requires weighted_graph_type<Graph> && std::totally_ordered<typename Graph::cost_type> alternatingpathgameresult<typename Graph::cost_type> alternating_path_game( const Graph& graph )
```

O((n+m) log n)時間・O(n+m)領域。非負重み付き有向graphの交互min/max終了優先ゲームを解く。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC261 Ex - Game on Graph](https://atcoder.jp/contests/abc261/tasks/abc261_h)では、
入力グラフを渡し、開始頂点における`gameplayer::minimize`の値を出力する。

`verify/atcoder_abc261_h.cpp`に提出用コードを収録している。
