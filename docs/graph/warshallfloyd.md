---
title: "warshall_floyd"
documentation_of: //cp/graph/warshallfloyd.hpp
---

# `warshall_floyd`

## Include

```cpp
#include "graph/warshallfloyd.hpp"
```

全点対最短路を時間`O(n^3)`、空間`O(n^2)`で求める。
距離・到達判定・頂点列復元・負閉路とその影響を受ける頂点対の判定を提供する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `warshallfloydresult`

```cpp
template<class Distance> struct warshallfloydresult
```

全点対最短路結果: result(from, to), result.path(from, to)で取得する。

### `reachable`

```cpp
bool reachable(int from, int to) const
```

O(1)。fromからtoへ到達可能か返す。

### `operator()`

```cpp
Distance operator()(int from, int to) const
```

O(1)。fromからtoへの距離を返す。到達不能ならinf。

### `has_negative_cycle`

```cpp
bool has_negative_cycle() const
```

O(n)。負閉路が存在するならtrueを返す。

### `affected_by_negative_cycle`

```cpp
bool affected_by_negative_cycle(int from, int to) const
```

O(n)。fromからtoの最短路が負閉路の影響を受けるならtrueを返す。

### `path`

```cpp
std::vector<int> path(int from, int to) const
```

O(経路長+n)。有限な最短路の頂点列を返す。到達不能または負閉路の影響下なら空列。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `warshall_floyd`

```cpp
template<weighted_graph_type Graph> auto warshall_floyd(const Graph& graph)
```

O(n^3)、空間O(n^2)。重み付きグラフの全点対最短路をWarshall-Floyd法で求める。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ GRL_1_C - All Pairs Shortest Path](https://onlinejudge.u-aizu.ac.jp/problems/GRL_1_C)。

```cpp
auto result = poe::warshall_floyd(graph);
if (result.has_negative_cycle()) std::cout << "NEGATIVE CYCLE\n";
else for (int from = 0; from < n; ++from)
    for (int to = 0; to < n; ++to)
        if (result.reachable(from, to)) std::cout << result(from, to) << ' ';
```

候補問題。
