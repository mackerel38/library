---
title: "トポロジカルソート"
documentation_of: //cp/graph/topologicalsort.hpp
---

# トポロジカルソート

## 概要

通常・辞書順最小・位置区間制約付きトポロジカル順を扱う。

## 厳密な定義

- `topologicalsortresult`: トポロジカルソート結果。dagはDAGか、uniqueは順序が一意かを表す。
- `boundedtopologicalresult`: 位置区間付きトポロジカルソート結果。position[v]は頂点vを置く0-indexed位置。
- `topological_sort`: O(n+m)。有向グラフをトポロジカルソートし、DAG判定と一意性も返す。
- `topological_sort_min`: O((n+m)log n)。辞書順最小のトポロジカル順序とDAG判定を返す。
- `bounded_topological_sort`: O((n+m)log n)。各頂点vをlower[v]以上upper[v]以下へ置くトポロジカル順序を一つ返す。


## Include

```cpp
#include "graph/topologicalsort.hpp"
```

`topological_sort`は`O(n+m)`で順序、DAG判定、一意性を返す。
`topological_sort_min`は`O((n+m)log n)`で辞書順最小の順序を返す。
`bounded_topological_sort`は`O((n+m)log n)`で、頂点`v`を
`lower[v] <= position[v] <= upper[v]`へ置く順序を一つ返す。
境界は0-indexedの閉区間で、解がなければ`found()`が`false`になる。

位置上限は辺に沿って逆向きに伝播し、配置可能な頂点のうち締切が最も早いものを選ぶ。
個別の解禁時刻と締切があるDAGのスケジューリングにも使える。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `topologicalsortresult`

```cpp
struct topologicalsortresult
```

トポロジカルソート結果。dagはDAGか、uniqueは順序が一意かを表す。

### `is_dag`

```cpp
bool is_dag() const noexcept
```

O(1)。DAGならtrueを返す。

### `is_unique`

```cpp
bool is_unique() const noexcept
```

O(1)。トポロジカル順序が一意ならtrueを返す。DAGでない場合はfalse。

### `boundedtopologicalresult`

```cpp
struct boundedtopologicalresult
```

位置区間付きトポロジカルソート結果。position[v]は頂点vを置く0-indexed位置。

### `found`

```cpp
bool found() const noexcept
```

O(1)。条件を満たす順序が見つかったならtrueを返す。

### `topological_sort`

```cpp
template<directed_graph_type Graph> topologicalsortresult topological_sort(const Graph& graph)
```

O(n+m)。有向グラフをトポロジカルソートし、DAG判定と一意性も返す。

### `topological_sort_min`

```cpp
template<directed_graph_type Graph> topologicalsortresult topological_sort_min(const Graph& graph)
```

O((n+m)log n)。辞書順最小のトポロジカル順序とDAG判定を返す。

### `bounded_topological_sort`

```cpp
template<directed_graph_type Graph> boundedtopologicalresult bounded_topological_sort( const Graph& graph, const std::vector<int>& lower, std::vector<int> upper )
```

O((n+m)log n)。各頂点vをlower[v]以上upper[v]以下へ置くトポロジカル順序を一つ返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ GRL_4_B - Topological Sort](https://onlinejudge.u-aizu.ac.jp/problems/GRL_4_B)。

```cpp
poe::directed_graph<> graph(n);
for (auto [from, to] : edges) graph.add_edge(from, to);
auto result = poe::topological_sort(graph);
for (int vertex : result.order) std::cout << vertex << '\n';
```

正しい順序が複数存在して公開出力との完全一致では検査できないため、
`verify/aoj_grl_4_b.cpp`で2026-07-15にjudge AC確認済み。

[AtCoder ABC304 Ex - Constrained Topological Sort](https://atcoder.jp/contests/abc304/tasks/abc304_h)
では、各頂点の許される位置区間をそのまま渡す。

```cpp
auto result = poe::bounded_topological_sort(graph, lower, upper);
if (!result.found()) {
    std::cout << "No\n";
} else {
    std::cout << "Yes\n";
    for (int vertex = 0; vertex < n; ++vertex) {
        std::cout << result.position[vertex] + 1 << " \n"[vertex + 1 == n];
    }
}
```

`tests/property/graph/boundedtopological.cpp`で全順列探索と比較し、
`verify/atcoder_abc304_h.cpp`で公式サンプルを確認済み。
