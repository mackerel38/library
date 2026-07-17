---
title: "区間間距離辺の最短路"
documentation_of: //cp/graph/rangeedge.hpp
---

# 区間間距離辺の最短路

- Header: `cp/graph/rangeedge.hpp`
- Symbol: `poe::rangedistancetransition`, `poe::range_distance_shortest_paths`
- Status: experimental

## どんな問題に使えるか

座標昇順の頂点列があり、一つの操作が半開区間`[l,r)`の任意頂点から、交差しない別区間
`[L,R)`の任意頂点へ移動できる問題に使う。頂点`u`から`v`への費用は
`base_cost + abs(position[u]-position[v])`とする。

```cpp
#include "graph/rangeedge.hpp"

vector<rangedistancetransition<long long>> transitions{
    {from_left, from_right, to_left, to_right, base_cost},
};
auto distance = range_distance_shortest_paths(position, transitions, start);
```

各区間対を全辺へ展開せず、東向き・西向きそれぞれの乗車側と降車側をsegment tree状の
補助頂点で表す。「区間に辺を張る典型」と、端点からの座標差の加算をAPI内に隠している。
区間同士は重ならず、接していてもよい。`position`は非減少、費用は非負でなければならない。

## 計算量

頂点数`n`、遷移数`q`として補助graphは`O(n+q)`頂点、`O(n+q log n)`辺。
Dijkstraを含めて`O(n log n + q log^2 n)`時間、`O(n+q log n)`領域。
戻り値の到達不能値は、通常の`dijkstra`と同じ十分大きい値である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rangedistancetransition`

```cpp
template <class Cost> struct rangedistancetransition
```

range_distance_shortest_pathsへ渡す半開区間間の全方向付き遷移。

### `range_distance_shortest_paths`

```cpp
template <class Cost> std::vector<Cost> range_distance_shortest_paths( const std::vector<Cost>& position, const std::vector<rangedistancetransition<Cost>>& transitions, int start )
```

O((n+q)log n)頂点辺構築後Dijkstra。区間間のcost+座標距離遷移によるstartからの最短距離。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC414 G - AtCoder Express 4](https://atcoder.jp/contests/abc414/tasks/abc414_g)では、
乗車区間、降車区間、初乗り運賃をそのまま遷移へ渡す。
`verify/atcoder_abc414_g.cpp`で公式sampleを確認済み。judge ACは未確認。

## 検証

- `tests/api/graph/rangeedge.cpp`: 公式sample相当
- `tests/property/graph/rangeedge.cpp`: 小グラフへ全区間対の辺を直接張ったDijkstraとの比較
