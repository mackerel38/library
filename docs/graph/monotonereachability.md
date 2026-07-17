---
title: "単調に増える到達可能集合"
documentation_of: //cp/graph/monotonereachability.hpp
---

# 単調に増える到達可能集合

- Header: `cp/graph/monotonereachability.hpp`
- Symbol: `poe::monotonereachability`
- Status: experimental

## どんな問題に使えるか

固定有向グラフ上で始点だけが追加され、「追加済み始点のどれかからこの頂点へ到達できるか」を
onlineで答える。すでに到達済みの頂点から辺を再探索しないため、query全体で各辺を高々一度見る。

```cpp
#include "graph/monotonereachability.hpp"

poe::monotonereachability reached(graph);
reached.add_source(source);
if (reached.contains(vertex)) {
    // いずれかの始点から到達可能
}
```

「頂点`v`から追加済み終点へ到達可能か」を知りたい場合は、`graph.reversed()`から構築する。

## 計算量

構築`O(n+m)`、`contains`は`O(1)`。すべての`add_source`を合わせて`O(n+m)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `monotonereachability`

```cpp
struct monotonereachability
```

始点追加だけを行う到達可能集合: monotonereachability reached(graph); reached.add_source(source);

### `monotonereachability`

```cpp
template <directed_graph_type Graph> explicit monotonereachability(const Graph& graph) : adjacency_(graph.size()), reached_(graph.size())
```

O(n+m)。以後変更しない有向graphに対する空の到達可能集合を作る。

### `add_source`

```cpp
void add_source(int source)
```

償却O(新しく到達する頂点とその出辺数)。sourceから到達できる頂点を集合へ加える。

### `contains`

```cpp
bool contains(int vertex) const
```

O(1)。vertexが追加済み始点のいずれかから到達可能か返す。

### `count`

```cpp
int count() const noexcept
```

O(1)。到達可能な頂点数を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC435 D - Reachability Query 2](https://atcoder.jp/contests/abc435/tasks/abc435_d)では、
辺を反転して黒頂点を始点として追加する。元のグラフで頂点`v`から黒頂点へ到達可能であることと、
反転グラフで黒頂点から`v`へ到達可能であることが同値になる。
`verify/atcoder_abc435_d.cpp`で公式sampleを確認済み。judge ACは未確認。
