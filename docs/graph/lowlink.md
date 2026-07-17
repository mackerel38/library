---
title: "lowlink / 二辺連結成分"
documentation_of: //cp/graph/lowlink.hpp
---

# `lowlink` / 二辺連結成分

## 概要

反復DFSによる関節点・橋・二辺連結成分を扱う。

## 厳密な定義

- `lowlinkresult`: LowLinkの結果。articulation_pointsは関節点、bridge_idsは橋の辺番号を表す。
- `lowlink`: O(n+m)。無向グラフの関節点と橋を反復DFSで求める。多重辺と自己辺も使える。
- `two_edge_connected_components`: O(n+m)。橋を除いて無向グラフを二辺連結成分分解する。


## Include

```cpp
#include "graph/lowlink.hpp"
```

`lowlink`は関節点と橋を、`two_edge_connected_components`は橋を除いた成分を返す。
反復DFSで、計算量は`O(n+m)`。多重辺と自己辺に対応する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lowlinkresult`

```cpp
struct lowlinkresult
```

LowLinkの結果。articulation_pointsは関節点、bridge_idsは橋の辺番号を表す。

### `is_bridge`

```cpp
bool is_bridge(int id) const
```

O(1)。辺番号idが橋ならtrueを返す。

### `lowlink`

```cpp
template<undirected_graph_type Graph> lowlinkresult lowlink(const Graph& graph)
```

O(n+m)。無向グラフの関節点と橋を反復DFSで求める。多重辺と自己辺も使える。

### `two_edge_connected_components`

```cpp
template<undirected_graph_type Graph> componentresult two_edge_connected_components(const Graph& graph)
```

O(n+m)。橋を除いて無向グラフを二辺連結成分分解する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Two-Edge-Connected Components](https://judge.yosupo.jp/problem/two_edge_connected_components)。

```cpp
auto component = poe::two_edge_connected_components(graph);
std::cout << component.groups.size() << '\n';
for (const auto& group : component.groups) {
    std::cout << group.size();
    for (int vertex : group) std::cout << ' ' << vertex;
    std::cout << '\n';
}
```

`verify/twoedgeconnectedcomponents.test.cpp`で2026-07-15に全ケースAC確認済み。
