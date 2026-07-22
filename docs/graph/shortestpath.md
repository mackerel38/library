---
title: "単一始点最短路"
documentation_of: //cp/graph/shortestpath.hpp
---

# 単一始点最短路

## 概要

明示・暗黙graphの最短路と、各頂点に近い複数の始点を扱う。
始点をラベル別にまとめ、自分と異なるラベルの最寄り始点を求めることもできる。

## 厳密な定義

- `shortestpathresult`: 最短路の距離と復元情報: result[v], result.path(v), result.edge_path(v)。
- `nearestsource`: ある頂点に近い始点の番号と距離。
- `nearestsourcesresult`: 各頂点に近い異なる始点を近い順にCount個まで持つ。
- `bfs`: O(n+m)。辺数を距離として、多点始点BFSを行う。重み付きグラフでは重みを無視する。
- `nearest_sources_bfs`: O(Count*(n+m))。各頂点について異なる始点の近い方からCount個をBFSで求める。
- `nearest_labeled_sources_dijkstra`: 非負重みgraphで、異なるラベルの始点を近い順に保持する。
- `distance_to_nearest_different_label_source`: 各頂点と異なるラベルを持つ最寄り指定始点への距離を返す。
- `zero_one_bfs`: O(n+m)。辺重みが0または1のグラフで、多点始点最短路を求める。
- `dijkstra`: O((n+m)log n)。非負重みグラフで、多点始点Dijkstra法を行う。
- `bellmanfordresult`: Bellman-Ford法の結果。negative[v]は負閉路からvへ到達できることを表す。
- `bellman_ford`: O(nm)。負辺を許すグラフで、多点始点Bellman-Ford法を行う。


## Include

```cpp
#include "graph/shortestpath.hpp"
```

- `bfs`: `O(n+m)`、辺数最短路。
- `zero_one_bfs`: `O(n+m)`、重み0/1。
- `dijkstra`: `O((n+m)log n)`、非負重み。
- `bellman_ford`: `O(nm)`、負辺と負閉路影響範囲。

結果から距離、到達判定、頂点列・辺番号列を取得でき、すべて多点始点にも対応する。

各頂点について「最も近い始点」だけでなく、異なる始点を近い順に固定個数必要とするときは
`nearest_sources_bfs<Count>(graph, starts)`を使う。

```cpp
auto nearest = nearest_sources_bfs<2>(graph, facilities);
int through_v = nearest[v][0].distance + nearest[v][1].distance;
```

各要素は`source`と`distance`を持ち、候補が足りなければ`source == -1`となる。
辺重みは無視し、`O(Count(n+m))`時間、`O(Count n)`領域を使う。同距離の始点間の順序は規定しない。

## ラベルの異なる最寄り始点

各指定始点$s$にラベル$L_s$があるとする。`nearest_labeled_sources_dijkstra<Count>`は、各頂点$v$で
ラベルが相異なる候補を距離順に高々`Count`個保持する。各候補は始点番号、ラベル、距離を持つ。

```cpp
auto nearest = nearest_labeled_sources_dijkstra<2>(
    graph, vector<pair<int, int>>{{source, label}});
```

各頂点$v$にもラベル$L_v$があり、求めたいものが

$$
\min_{s\in S,\ L_s\ne L_v}\operatorname{dist}(v,s)
$$

だけなら、次の短縮APIを使う。

```cpp
auto distance = distance_to_nearest_different_label_source(
    graph, labels, sources);
```

該当する始点がない頂点には`std::numeric_limits<Cost>::max()`が入る。辺重みは非負である必要がある。
時間計算量は基礎APIが$O(\mathrm{Count}(n+m)\log(\mathrm{Count}\,n))$、短縮APIが
$O((n+m)\log n)$、領域計算量は$O(\mathrm{Count}\,n+m)$である。

頂点数だけ既知で辺をその場で列挙したい場合は、暗黙graph版を使える。

```cpp
auto result = zero_one_bfs(vertex_count, start, [&](int vertex, auto relax) {
    for (auto [to, cost] : transitions(vertex)) relax(to, cost);
});
```

`expand(vertex, relax)`から`relax(to, cost)`を呼ぶ。`cost`は0または1。
辺を保存せず`O(n+m)`時間、距離・親頂点に`O(n)`領域を使う。暗黙辺には番号がないため
`parent_edge`と`edge_path`は利用しない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `shortestpathresult`

```cpp
template<class Distance> struct shortestpathresult
```

最短路の距離と復元情報: result[v], result.path(v), result.edge_path(v)。

### `reachable`

```cpp
bool reachable(int vertex) const
```

O(1)。頂点vertexへ到達可能か返す。

### `operator[]`

```cpp
Distance operator[](int vertex) const
```

O(1)。頂点vertexまでの距離を返す。到達不能ならinfを返す。

### `path`

```cpp
std::vector<int> path(int vertex) const
```

O(経路長)。始点からvertexまでの頂点列を返す。到達不能なら空列を返す。

### `edge_path`

```cpp
std::vector<int> edge_path(int vertex) const
```

O(経路長)。始点からvertexまでの辺番号列を返す。到達不能なら空列を返す。

### `nearestsource`

```cpp
template <class Distance> struct nearestsource
```

ある頂点に近い始点の番号と距離。

### `nearestsourcesresult`

```cpp
template <class Distance, int Count> struct nearestsourcesresult
```

各頂点に近い異なる始点を近い順にCount個まで持つ。

### `reachable`

```cpp
bool reachable(int vertex, int index) const
```

O(1)。vertexにindex番目の始点が存在するか返す。

### `operator[]`

```cpp
const std::array<nearestsource<Distance>, Count>& operator[](int vertex) const
```

O(1)。vertexに近い始点列を返す。

### `nearestlabeledsource`

```cpp
template<class Distance, class Label> struct nearestlabeledsource
```

ある頂点に近い指定ラベルの始点・ラベル・距離。

### `nearestlabeledsourcesresult`

```cpp
template<class Distance, class Label, int Count> struct nearestlabeledsourcesresult
```

各頂点について異なるラベルの近い始点をCount個まで持つ。

### `operator[]`

```cpp
const std::array<nearestlabeledsource<Distance, Label>, Count>& operator[]( int vertex ) const
```

O(1)。vertexに記録したラベル別の近い始点列を返す。

### `bfs`

```cpp
template<graph_type Graph> shortestpathresult<int> bfs(const Graph& graph, const std::vector<int>& starts)
```

O(n+m)。辺数を距離として、多点始点BFSを行う。重み付きグラフでは重みを無視する。

### `bfs`

```cpp
template<graph_type Graph> shortestpathresult<int> bfs(const Graph& graph, int start)
```

O(n+m)。辺数を距離として、startからBFSを行う。重み付きグラフでは重みを無視する。

### `nearest_sources_bfs`

```cpp
template <int Count, graph_type Graph> nearestsourcesresult<int, Count> nearest_sources_bfs( const Graph& graph, const std::vector<int>& starts )
```

O(Count*(n+m))。各頂点について異なる始点の近い方からCount個をBFSで求める。

### `nearest_labeled_sources_dijkstra`

```cpp
template<int Count, weighted_graph_type Graph, class Label> nearestlabeledsourcesresult<typename Graph::cost_type, Label, Count> nearest_labeled_sources_dijkstra( const Graph& graph, const std::vector<std::pair<int, Label>>& sources )
```

O(Count(m+n)log(Count n))。非負重みgraphで異なるラベルの近い始点をCount個求める。

### `distance_to_nearest_different_label_source`

```cpp
template<weighted_graph_type Graph, class Label> std::vector<typename Graph::cost_type> distance_to_nearest_different_label_source( const Graph& graph, const std::vector<Label>& labels, const std::vector<int>& sources )
```

O((m+n)log n)。各頂点から自身と異なるラベルを持つ最寄り指定始点への距離を返す。

### `zero_one_bfs`

```cpp
template<weighted_graph_type Graph> auto zero_one_bfs(const Graph& graph, const std::vector<int>& starts)
```

O(n+m)。辺重みが0または1のグラフで、多点始点最短路を求める。

### `zero_one_bfs`

```cpp
template<weighted_graph_type Graph> auto zero_one_bfs(const Graph& graph, int start)
```

O(n+m)。辺重みが0または1のグラフで、startから最短路を求める。

### `zero_one_bfs`

```cpp
template <class Expand> shortestpathresult<int> zero_one_bfs( int vertex_count, const std::vector<int>& starts, Expand expand )
```

O(n+m)。expand(v,relax)がrelax(to,cost)で重み0/1の出辺を列挙する暗黙graphの最短路。

### `zero_one_bfs`

```cpp
template <class Expand> shortestpathresult<int> zero_one_bfs(int vertex_count, int start, Expand expand)
```

O(n+m)。暗黙graphでstartから0-1 BFSを行う。expandの呼び方は多点始点版と同じ。

### `dijkstra`

```cpp
template<weighted_graph_type Graph> auto dijkstra(const Graph& graph, const std::vector<int>& starts)
```

O((n+m)log n)。非負重みグラフで、多点始点Dijkstra法を行う。

### `dijkstra`

```cpp
template<weighted_graph_type Graph> auto dijkstra(const Graph& graph, int start)
```

O((n+m)log n)。非負重みグラフで、startからDijkstra法を行う。

### `bellmanfordresult`

```cpp
template<class Distance> struct bellmanfordresult : shortestpathresult<Distance>
```

Bellman-Ford法の結果。negative[v]は負閉路からvへ到達できることを表す。

### `affected_by_negative_cycle`

```cpp
bool affected_by_negative_cycle(int vertex) const
```

O(1)。頂点vertexの最短距離が負閉路の影響を受けるか返す。

### `has_negative_cycle`

```cpp
bool has_negative_cycle() const
```

O(n)。始点から到達できる負閉路が存在するか返す。

### `path`

```cpp
std::vector<int> path(int vertex) const
```

O(経路長)。有限な最短路の頂点列を返す。到達不能または負閉路の影響下なら空列を返す。

### `edge_path`

```cpp
std::vector<int> edge_path(int vertex) const
```

O(経路長)。有限な最短路の辺番号列を返す。到達不能または負閉路の影響下なら空列を返す。

### `bellman_ford`

```cpp
template<weighted_graph_type Graph> auto bellman_ford(const Graph& graph, const std::vector<int>& starts)
```

O(nm)。負辺を許すグラフで、多点始点Bellman-Ford法を行う。

### `bellman_ford`

```cpp
template<weighted_graph_type Graph> auto bellman_ford(const Graph& graph, int start)
```

O(nm)。負辺を許すグラフで、startからBellman-Ford法を行う。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Shortest Path](https://judge.yosupo.jp/problem/shortest_path)。

```cpp
auto result = poe::dijkstra(graph, source);
if (!result.reachable(target)) std::cout << -1 << '\n';
else for (int id : result.edge_path(target)) {
    auto edge = graph.edge_at(id);
    std::cout << edge.from << ' ' << edge.to << '\n';
}
```

`verify/shortestpath.test.cpp`で2026-07-15に全ケースAC確認済み。

[AtCoder ABC431 E - Reflection on Grid](https://atcoder.jp/contests/abc431/tasks/abc431_e)では、
各マスと出射方向を状態にし、鏡を維持する遷移を0、変更する遷移を1として暗黙graph版を使う。
`verify/atcoder_abc431_e.cpp`で公式sampleを確認済み。

[AtCoder ABC429 E - Hit and Away](https://atcoder.jp/contests/abc429/tasks/abc429_e)では、
各危険頂点に近い異なる安全頂点2個への距離和を求める。
`verify/atcoder_abc429_e.cpp`で公式sample 2件を確認済み。

[AtCoder ABC424 C - New Skill Acquired](https://atcoder.jp/contests/abc424/tasks/abc424_c)では、
前提skillから新skillへの有向辺を張り、仮想skill 0からの`bfs`到達頂点を数える。
`verify/atcoder_abc424_c.cpp`に使用例を収録した。

[AtCoder ABC245 G - Foreign Friends](https://atcoder.jp/contests/abc245/tasks/abc245_g)では、
国番号を頂点ラベル、人気者を指定始点として`distance_to_nearest_different_label_source`へ渡す。
無限大の要素だけ`-1`へ置き換える。`verify/atcoder_abc245_g.cpp`に提出用コードを用意している。
