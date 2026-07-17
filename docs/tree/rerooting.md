---
title: "rerooting"
documentation_of: //cp/tree/rerooting.hpp
---

# `rerooting`

## Include

```cpp
#include "tree/rerooting.hpp"
```

結合関数、単位元、頂点作用、辺作用を渡し、全頂点を根とした木DPを`O(n)`で返す。
`merge`は結合則を満たし、指定した`identity`を単位元とすること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rerooting`

```cpp
template<undirected_graph_type Graph, class Value, class Merge, class AddVertex, class AddEdge> std::vector<Value> rerooting(const Graph& graph, const Value& identity, Merge merge, AddVertex add_vertex, AddEdge add_edge)
```

O(n)。木の全頂点を根としたDPを返す。mergeは結合則を満たし、identityを単位元とすること。
add_vertex(統合値, 頂点)、add_edge(送る頂点のDP, 送る向きの辺)を指定する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Educational DP Contest V - Subtree](https://atcoder.jp/contests/dp/tasks/dp_v)。

```cpp
auto merge = [mod](long long left, long long right) { return left * right % mod; };
auto add_vertex = [](long long value, int) { return value; };
auto add_edge = [mod](long long value, const auto&) { return (value + 1) % mod; };
auto answer = poe::rerooting(graph, 1LL, merge, add_vertex, add_edge);
```

`verify/atcoder_dp_v.cpp`で2026-07-15にjudge AC確認済み。
