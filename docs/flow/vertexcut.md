---
title: "vertexcut"
documentation_of: //cp/flow/vertexcut.hpp
---

# `vertexcut`

## 概要

辺ではなく頂点に削除費用があるグラフで、二頂点間の経路をすべて切る最小費用と、実際に削除する頂点を求める。

## 厳密な定義

グラフ $G=(V,E)$、始点 $s$、終点 $t$、各頂点の非負費用 $c_v$ が与えられる。
$s$ から $t$ へのすべての経路が少なくとも一頂点を含むような削除可能頂点集合 $X$ のうち、

$$
\sum_{v\in X} c_v
$$

を最小にする $X$ とその費用を返す。`std::nullopt` を指定した頂点は削除できない。

## Include

```cpp
#include "flow/vertexcut.hpp"
```

## API

```cpp
template<class Cost>
struct vertexcutresult {
    bool possible;
    Cost cost;
    std::vector<int> vertices;
};
```

- `possible`: 削除可能な頂点だけで分離できるか。
- `cost`: 最小費用。`possible == false` のときは `0`。
- `vertices`: 削除する頂点番号の昇順列。

```cpp
minimum_vertex_cut(graph, costs, source, sink)
```

`costs` が `std::vector<Cost>` なら、`source` と `sink` だけを削除不可として扱う。
`std::vector<std::optional<Cost>>` なら、`std::nullopt` の頂点をすべて削除不可として扱う。
有向・無向、重み付き・重みなしの各グラフを渡せる。辺の重みは使用しない。

頂点分割とDinic法を用いる。頂点数を $n$、辺数を $m$ とすると計算量は $O(n^2(n+m))$、使用メモリは $O(n+m)$。

## 使用例

[AtCoder ABC239 G - Builder Takahashi](https://atcoder.jp/contests/abc239/tasks/abc239_g)では、頂点 `0` と `n - 1` を切り離すために破壊する頂点を求める。

```cpp
undirected_graph<> graph(n);
graph.read(m);
vector<long long> costs(n);
for (auto& cost : costs) cin >> cost;

const auto result = minimum_vertex_cut(graph, costs, 0, n - 1);
cout << result.cost << '\n';
cout << result.vertices.size() << '\n';
for (int vertex : result.vertices) cout << vertex + 1 << ' ';
cout << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `vertexcutresult`

```cpp
template<class Cost> struct vertexcutresult
```

最小重み頂点cutの結果。possible=falseなら削除可能な頂点だけでは分離できない。

### `minimum_vertex_cut`

```cpp
template<class Graph, class Cost> vertexcutresult<Cost> minimum_vertex_cut( const Graph& graph, const std::vector<std::optional<Cost>>& costs, int source, int sink )
```

O(n^2(n+m))。nulloptの頂点を削除せず、sourceからsinkへの最小重み頂点cutを返す。

### `minimum_vertex_cut`

```cpp
template<class Graph, class Cost> vertexcutresult<Cost> minimum_vertex_cut( const Graph& graph, const std::vector<Cost>& costs, int source, int sink )
```

O(n^2(n+m))。sourceとsink以外を削除可能として最小重み頂点cutを返す。

<!-- END AUTO-GENERATED API REFERENCE -->
