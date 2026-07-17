---
title: "木の辺path和"
documentation_of: //cp/tree/pathsum.hpp
---

# 木の辺path和


## 概要

木の辺重みを一点更新しながら、二頂点間pathの辺重み和を求める。
重み付き`undirected_graph<T>`なら辺costをそのまま初期値にする。

## 厳密な定義

- `edgepathsum`: 辺の一点更新と木のpath和を扱う: edgepathsum data(graph); 構築O(n)。

## Include

```cpp
#include "tree/pathsum.hpp"

poe::edgepathsum distance(graph);
distance.set(edge_id, new_weight);
cout << distance.sum(from, to) << '\n';
```

重みなし木では、辺番号順の初期値を第二引数へ渡す。
`set`、`add`、`operator[]`も辺番号を受け取るため、HLD上の位置を提出側で管理しない。

構築`O(n)`、一点更新・一点取得`O(log n)`、path和`O(log^2 n)`、領域`O(n)`。

## インターフェイス

```cpp
edgepathsum(const Graph& graph, int root = 0);              // 重み付き木
edgepathsum(const Graph& graph, const vector<T>& values,
            int root = 0);                                 // 任意の辺値
```

`graph`は連結な無向木、`root`は`[0, graph.size())`。第一形式は
`Graph::is_weighted == true`のときだけ使え、各辺の`cost`を初期値にする。
第二形式の`values[id]`は辺番号`id`の初期値で、要素数は`graph.edge_count()`。
テンプレート引数は通常CTADで推論されるので明記しなくてよい。

```cpp
void set(int id, const T& value);
void add(int id, const T& delta);
T operator[](int id) const;
T sum(int from, int to) const;
```

`id`はグラフが辺追加時に割り当てた辺番号。`set`は代入、`add`は加算、
`operator[]`は現在値の取得を行う。`sum(from, to)`は両端の頂点間にある辺だけを
足し、`from == to`なら`T{}`を返す。`T`にはデフォルト構築、加減算が必要。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `edgepathsum`

```cpp
template <class T, undirected_graph_type Graph> struct edgepathsum
```

辺の一点更新と木のpath和を扱う: edgepathsum data(graph); 構築O(n)。

### `edgepathsum`

```cpp
explicit edgepathsum(const Graph& graph, int root = 0) requires Graph::is_weighted : decomposition_(graph, root), values_(initial_values(graph, decomposition_))
```

O(n)。重み付き木のcostを初期値として構築する。

### `edgepathsum`

```cpp
edgepathsum(const Graph& graph, const std::vector<T>& values, int root = 0) : decomposition_(graph, root), values_(ordered_values(graph, decomposition_, values))
```

O(n)。辺番号順のvaluesを初期値として構築する。

### `set`

```cpp
void set(int id, const T& value)
```

O(log n)。辺idの値をvalueへ変更する。

### `add`

```cpp
void add(int id, const T& delta)
```

O(log n)。辺idの値へdeltaを加える。

### `operator[]`

```cpp
T operator[](int id) const
```

O(log n)。辺idの現在値を返す。

### `sum`

```cpp
T sum(int from, int to) const
```

O(log^2 n)。fromからtoへのpathに含まれる辺の値の総和を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC294 G - Distance Queries on a Tree](https://atcoder.jp/contests/abc294/tasks/abc294_g)へ直接使える。
`verify/atcoder_abc294_g.cpp`で公式sample 1から3を確認済み。

## 検証

- `tests/api/tree/pathsum.cpp`: 代入・加算・辺取得・path和
- `tests/property/tree/pathsum.cpp`: 更新を混ぜたBFS復元pathとの比較
