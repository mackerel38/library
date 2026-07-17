---
title: "グループ遷移付きBFS"
documentation_of: //cp/graph/hyperedge.hpp
---

# グループ遷移付きBFS

- Header: `cp/graph/hyperedge.hpp`
- Symbol: `poe::bfs_with_hyperedges`
- Status: experimental

## どんな問題に使えるか

通常の単位コスト辺に加え、同じグループに属する任意の頂点間を1手で移動できる最短路を求める。
各グループは最短距離が確定した最初の所属頂点から一度だけ展開するため、巨大なcliqueを辺へ展開しない。
グループ遷移のコストが1でない場合には使えない。

## API・計算量

```cpp
#include "graph/hyperedge.hpp"

auto distance = poe::bfs_with_hyperedges(graph, groups, start);
auto grid_distance = poe::bfs_with_hyperedges(vertex_count, groups, start,
    [&](int vertex, auto visit) { /* 通常辺ごとにvisit(to) */ });
```

通常辺数を`m`、グループ所属関係の総数を`incidence`として
`O(n+m+incidence)`時間・領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `bfs_with_hyperedges`

```cpp
template <class Expand> std::vector<int> bfs_with_hyperedges(int vertex_count, const std::vector<std::vector<int>>& groups, int start, Expand expand)
```

通常辺と「同じgroup内の任意頂点へ1手」を持つBFS: bfs_with_hyperedges(n, groups, s, expand); O(n+m+incidence)。
expand(v, visit)内で通常辺の行先ごとにvisit(to)を呼ぶ。

### `bfs_with_hyperedges`

```cpp
template <graph_type Graph> std::vector<int> bfs_with_hyperedges(const Graph& graph, const std::vector<std::vector<int>>& groups, int start)
```

通常辺をgraphから読むhyperedge BFS: bfs_with_hyperedges(graph, groups, start); O(n+m+incidence)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC436 D - Teleport Maze](https://atcoder.jp/contests/abc436/tasks/abc436_d)では、
同じ英小文字のマスを一つのグループにする。
`verify/atcoder_abc436_d.cpp`は公式サンプル一致を確認済み、judge未提出。
