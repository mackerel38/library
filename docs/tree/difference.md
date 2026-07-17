---
title: "木上差分"
documentation_of: //cp/tree/difference.hpp
---

# 木上差分

- Status: experimental
- Header: `cp/tree/difference.hpp`
- Symbol: `poe::vertexpathdifference`, `poe::edgepathdifference`, `poe::subtreedifference`

## Include

```cpp
#include "tree/difference.hpp"
```

## 選び方

- `vertexpathdifference<Graph,T>`: 二頂点間パス上の全頂点へoffline加算。
- `edgepathdifference<Graph,T>`: 二頂点間パス上の全辺へoffline加算。
- `subtreedifference<Graph,T>`: 部分木または部分木以外の全頂点へoffline加算。

```cpp
vertexpathdifference<decltype(graph), long long> vertices(graph);
vertices.add(from, to, value);
vertices.build();
cout << vertices[vertex] << '\n';
```

辺版の`operator[]`と`values()`は、`graph.add_edge`が返した辺番号順。
部分木版は`add(vertex, value)`と`add_complement(vertex, value)`を持つ。
すべての予約後に`build()`を一度呼び、その後`operator[]`または`values()`で読む。
`clear()`は予約と結果をすべて`T{}`へ戻す。

`T`には零初期化、`+=`、`-=`が必要。更新を一件ずつ即座に取得するonline構造ではない。

## 計算量

| 型 | 前処理 | 一回の予約 | `build` | 領域 |
| --- | --- | --- | --- | --- |
| 頂点・辺path | `O(n log n)` | `O(log n)` | `O(n)` | `O(n log n)` |
| subtree | `O(n)` | `O(1)` | `O(n)` | `O(n)` |

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `vertexpathdifference`

```cpp
template <undirected_graph_type Graph, class T> using vertexpathdifference = detail::pathdifference<Graph, T, false>
```

offline頂点パス加算: vertexpathdifference<Graph,long long> diff(graph); add O(log n)、build O(n)。

### `edgepathdifference`

```cpp
template <undirected_graph_type Graph, class T> using edgepathdifference = detail::pathdifference<Graph, T, true>
```

offline辺パス加算: edgepathdifference<Graph,long long> diff(graph); add O(log n)、build O(n)。

### `subtreedifference`

```cpp
template <undirected_graph_type Graph, class T> struct subtreedifference
```

部分木・部分木外へのoffline頂点加算: subtreedifference<Graph,T> diff(graph); 各add O(1)。

### `subtreedifference`

```cpp
explicit subtreedifference(const Graph& graph, int root = 0) : tree_(graph, root), difference_(graph.size() + 1)
```

O(n)。全頂点をT{}としてroot根のDFS順を前処理する。

### `add`

```cpp
void add(int vertex, const T& value)
```

O(1)。vertexの部分木の全頂点へvalueを加える予約をする。

### `add_complement`

```cpp
void add_complement(int vertex, const T& value)
```

O(1)。vertexの部分木以外の全頂点へvalueを加える予約をする。

### `build`

```cpp
void build()
```

O(n)。予約した加算を反映して頂点番号順の値を確定する。

### `operator[]`

```cpp
const T& operator[](int vertex) const
```

O(1)。build後、頂点vertexの値を返す。

### `values`

```cpp
const std::vector<T>& values() const
```

O(1)。build後、頂点番号順の全値を返す。

### `clear`

```cpp
void clear()
```

O(n)。全予約と確定値をT{}へ戻す。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `parent`

```cpp
int parent(int vertex) const
```

O(1)。根付き木でvertexの親を返す。根では-1。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC187 E - Through Path](https://atcoder.jp/contests/abc187/tasks/abc187_e)では、
辺を切った一方側への加算を`subtreedifference`の`add`または`add_complement`へ変換できる。
`verify/atcoder_abc187_e.cpp`を収録した。judge未提出。

頂点・辺path版はtree imosとして、複数経路が各頂点・辺を通る回数や重み総和を一括計算する。

## 検証

- `tests/api/tree/difference.cpp`: 3種類の公開APIと辺番号順
- `tests/property/tree/difference.cpp`: ランダム木のpath・subtree加算を愚直更新と比較
- `verify/atcoder_abc187_e.cpp`: 公式問題用コード
