---
title: "最大clique・最大独立集合"
documentation_of: //cp/graph/maximumclique.hpp
---

# 最大clique・最大独立集合

## Include

```cpp
#include "graph/maximumclique.hpp"
```

`maximum_clique(graph)`は、全頂点対の間に辺がある最大頂点集合を返す。
`maximum_independent_set(graph)`は、内部に辺を持たない最大頂点集合を返す。
どちらも結果の`vertices`に頂点列、`size()`に頂点数が入る。

63頂点以下の`undirected_graph`を受け取り、greedy coloringによる上界を使ったbranch-and-boundで探索する。
最大独立集合は補グラフの最大cliqueとして同じ実装を使う。最悪計算量は`O(2^n)`であり、
63頂点なら常に速いという意味ではない。密度や探索順によって実行時間が大きく変わる。

自己辺は無視し、多重辺は一本として扱う。重み付きグラフも受け取るが辺重みは使わない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `maximumvertexsetresult`

```cpp
struct maximumvertexsetresult
```

最大clique・最大独立集合の結果。verticesは条件を満たす最大頂点集合。

### `size`

```cpp
int size() const noexcept
```

O(1)。選ばれた頂点数を返す。

### `maximum_clique`

```cpp
template<undirected_graph_type Graph> maximumvertexsetresult maximum_clique(const Graph& graph)
```

worst O(2^n)。63頂点以下の無向グラフの最大cliqueを彩色枝刈りで求める。

### `maximum_independent_set`

```cpp
template<undirected_graph_type Graph> maximumvertexsetresult maximum_independent_set(const Graph& graph)
```

worst O(2^n)。63頂点以下の無向グラフの最大独立集合を補グラフの最大cliqueとして求める。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder Beginner Contest 002 D - 派閥](https://atcoder.jp/contests/abc002/tasks/abc002_4)。

```cpp
undirected_graph graph(n);
for (auto [x, y] : relations) graph.add_edge(x, y);
cout << maximum_clique(graph).size() << '\n';
```

`verify/atcoder_abc002_d.cpp`に提出用コードを用意し、公式サンプル4件との一致を確認済み。
。
