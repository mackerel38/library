---
title: "matrix_graph"
documentation_of: //cp/graph/matrixgraph.hpp
---

# `matrix_graph`

<!-- API REFERENCE: COLLAPSED -->

## 概要

頂点数が比較的小さく、二頂点の間に辺があるかを何度も`O(1)`で調べたいときに使う。
隣接リスト版の`directed_graph`・`undirected_graph`と相互変換できる。

## 厳密な定義

頂点集合を`V = {0, 1, ..., n-1}`とし、各順序対`(u, v)`に対して辺の有無を一つのセルで保持する。
重み付きの場合、辺が存在するセルは`Cost`型の重みを一つ持つ。したがって多重辺は保持できない。

- `directed_matrix_graph`では`(u, v)`と`(v, u)`を別の辺として扱う
- `undirected_matrix_graph`では常に`has_edge(u, v) == has_edge(v, u)`となり、重みも等しい
- 自己辺`(u, u)`は一本と数える
- 頂点数`n`に対して`O(n^2)`領域を使う

## Include

```cpp
#include "graph/matrixgraph.hpp"
```

## 最短の使用例

```cpp
poe::directed_matrix_graph<long long> graph(n);
graph.set_edge(from, to, cost);

if (graph.has_edge(from, to)) {
    cout << graph.cost(from, to) << '\n';
}
```

## 型の選び方

| グラフ | 重みなし | 重み付き |
| --- | --- | --- |
| 有向 | `directed_matrix_graph<>` | `directed_matrix_graph<Cost>` |
| 無向 | `undirected_matrix_graph<>` | `undirected_matrix_graph<Cost>` |

普段はこの四つの形を使う。

```cpp
poe::directed_matrix_graph<long long> directed(n);
poe::undirected_matrix_graph<> undirected(n);
```

本体は`matrix_graph<Cost, Directed>`だが、方向を明記できる上記の別名を推奨する。

## 構築

```cpp
directed_matrix_graph<long long> graph(n);
```

`n`頂点で辺のないグラフを`O(n^2)`で作る。

```cpp
directed_matrix_graph<long long> matrix(list_graph);
directed_graph<long long> restored(matrix);
```

向きと`Cost`が同じ隣接リストグラフ・隣接行列グラフは、明示的コンストラクタで相互変換できる。
変換には`O(n^2+m)`かかる。隣接リストに多重辺がある場合、行列には最後に追加された辺だけが
残る。行列から隣接リストへ変換すると辺番号は付け直される。

```cpp
constexpr long long inf = (1LL << 60);
vector<vector<long long>> cost = {
    {inf, 4, inf},
    {inf, inf, 7},
    {2, inf, inf},
};
directed_matrix_graph<long long> graph(cost, inf);
```

重み付きでは、第二引数`absent`と等しいセルを「辺なし」として二次元`vector`から構築できる。
重みなしでは、二次元`vector`の`false`または`0`を「辺なし」、それ以外を「辺あり」とする。
無向グラフへ渡す行列は対称でなければならない。いずれも`O(n^2)`。

`poe::matrix<Cost>`も同じ形式でコンストラクタへ渡せる。

```cpp
poe::matrix<long long> cost_matrix(cost);
directed_matrix_graph<long long> graph(cost_matrix, inf);
```

## 頂点追加

```cpp
int vertex = graph.add_vertex();
int first = graph.add_vertices(count);
```

`add_vertex()`は追加した頂点番号を返す。`add_vertices(count)`は追加された連続区間
`[first, first + count)`の`first`を返す。隣接行列を拡張するため、それぞれ償却`O(n)`、
償却`O(n * count + count^2)`。

## 辺操作

```cpp
graph.set_edge(from, to);        // 重みなし
graph.set_edge(from, to, cost);  // 重み付き
bool erased = graph.remove_edge(from, to);
bool exists = graph.has_edge(from, to);
```

すべて`O(1)`。`set_edge`は辺がなければ追加し、既にあれば上書きする。無向グラフでは
`[from][to]`と`[to][from]`を同時に変更する。自己辺も`edge_count()`では一本と数える。

```cpp
const Cost& cost = graph.cost(from, to);
const auto& row = graph[from];
```

`cost(from, to)`は存在する重み付き辺の重みを`O(1)`で返す。`operator[]`は行を返し、
重み付きでは各セルが`optional<Cost>`、重みなしでは各セルが0または1になる。通常はセルの
内部表現へ依存せず、`has_edge`と`cost`を使う方がよい。

## 取得・二次元vectorへの変換

```cpp
int n = graph.size();
int m = graph.edge_count();
auto values = graph.values(inf);
auto cost_matrix = graph.to_matrix(inf);
```

`size()`と`edge_count()`は`O(1)`。重み付きの`values(absent)`は、辺のないセルを
`absent`で埋めた通常の二次元`vector`を返し、`to_matrix(absent)`は同内容の
`poe::matrix<Cost>`を返す。どちらも`O(n^2)`。

## 検証状況

- `tests/api/graph/matrixgraph.cpp`: 全公開操作と相互変換
- `tests/property/graph/matrixgraph.cpp`: 有向重み付き・無向重みなしのランダム往復変換

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `matrix_graph`

```cpp
template<class Cost = void, bool Directed = true> struct matrix_graph
```

n*n隣接行列のグラフ: directed_matrix_graph<long long> matrix(n); 構築O(n^2)。

### `matrix_graph`

```cpp
matrix_graph() = default
```

O(1)。空グラフを作る。

### `matrix_graph`

```cpp
explicit matrix_graph(int n) : adjacency_(n, std::vector<cell_type>(n))
```

O(n^2)。n頂点で辺のない隣接行列グラフを作る。

### `matrix_graph`

```cpp
template<graph_type Graph> requires (Graph::is_directed == Directed && std::same_as<typename Graph::cost_type, Cost>) explicit matrix_graph(const Graph& source) : matrix_graph(source.size())
```

O(n^2+m)。同じ向き・重み型の隣接リストグラフから変換する。

### `matrix_graph`

```cpp
template<class Value> requires (std::is_void_v<Cost> && std::convertible_to<Value, bool>) explicit matrix_graph(const std::vector<std::vector<Value>>& values) : matrix_graph(static_cast<int>(values.size()))
```

O(n^2)。非zeroを辺とする重みなし二次元vectorから作る。

### `matrix_graph`

```cpp
explicit matrix_graph(const std::vector<std::vector<stored_cost_type>>& values, const stored_cost_type& absent) requires (!std::is_void_v<Cost>) : matrix_graph(static_cast<int>(values.size()))
```

O(n^2)。absent以外を辺重みとする二次元vectorから作る。

### `matrix_graph`

```cpp
template<class Value> requires (std::is_void_v<Cost> && std::convertible_to<Value, bool>) explicit matrix_graph(const matrix<Value>& values) : matrix_graph(values.height())
```

O(n^2)。非zeroを辺とする重みなしmatrixから作る。

### `matrix_graph`

```cpp
explicit matrix_graph(const matrix<stored_cost_type>& values, const stored_cost_type& absent) requires (!std::is_void_v<Cost>) : matrix_graph(values.height())
```

O(n^2)。absent以外を辺重みとするmatrixから作る。

### `add_vertex`

```cpp
int add_vertex()
```

償却O(n)。頂点を一つ追加し、その頂点番号を返す。

### `add_vertices`

```cpp
int add_vertices(int count)
```

償却O(n*count+count^2)。count頂点を追加し、追加区間の先頭番号を返す。

### `set_edge`

```cpp
void set_edge(int from, int to) requires std::is_void_v<Cost>
```

O(1)。重みなし辺を追加または上書きする。

### `set_edge`

```cpp
void set_edge(int from, int to, const stored_cost_type& cost) requires (!std::is_void_v<Cost>)
```

O(1)。重み付き辺を追加または上書きする。

### `remove_edge`

```cpp
bool remove_edge(int from, int to)
```

O(1)。辺があれば削除し、削除できたかを返す。

### `has_edge`

```cpp
bool has_edge(int from, int to) const
```

O(1)。fromからtoへの辺があればtrueを返す。

### `cost`

```cpp
const stored_cost_type& cost(int from, int to) const requires (!std::is_void_v<Cost>)
```

O(1)。存在する重み付き辺の重みを返す。

### `operator[]`

```cpp
const std::vector<cell_type>& operator[](int from) const
```

O(1)。from行を返す。重み付きcellはoptional<Cost>、重みなしcellは0/1。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `edge_count`

```cpp
int edge_count() const noexcept
```

O(1)。辺数を返す。無向辺も一本と数える。

### `values`

```cpp
std::vector<std::vector<stored_cost_type>> values(const stored_cost_type& absent) const requires (!std::is_void_v<Cost>)
```

O(n^2)。辺なしをabsentで埋めた通常の二次元vectorへ変換する。

### `to_matrix`

```cpp
matrix<stored_cost_type> to_matrix(const stored_cost_type& absent) const requires (!std::is_void_v<Cost>)
```

O(n^2)。辺なしをabsentで埋めたmatrixへ変換する。

### `directed_matrix_graph`

```cpp
template<class Cost = void> using directed_matrix_graph = matrix_graph<Cost, true>
```

有向隣接行列グラフ: directed_matrix_graph<long long> graph(n)。

### `undirected_matrix_graph`

```cpp
template<class Cost = void> using undirected_matrix_graph = matrix_graph<Cost, false>
```

無向隣接行列グラフ: undirected_matrix_graph<long long> graph(n)。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->
