# Graph

個別文書: [graph](graph.md)、[connectivity](connectivity.md)、[cycle](cycle.md)、
[lowlink](lowlink.md)、[minimumspanningtree](minimumspanningtree.md)、[scc](scc.md)、
[shortestpath](shortestpath.md)、[topologicalsort](topologicalsort.md)、[twosat](twosat.md)、
[warshallfloyd](warshallfloyd.md)。

共通グラフ型の設計判断と拡張方針は[グラフ構造の設計](design.md)に記載する。

## `directed_graph` / `undirected_graph`

有向・無向を公開型として固定し、辺追加を`add_edge`へ統一する。

```cpp
poe::directed_graph<> directed(n);
directed.add_edge(from, to);

poe::undirected_graph<long long> weighted(n);
weighted.add_edge(left, right, cost);
```

テンプレート引数を省略すると重みなし、指定するとその型の重み付きグラフになる。
`operator[]`は頂点から出る辺、`edge_at`は辺番号から追加時の辺、`edges`は全辺一覧を返す。
無向辺は隣接リストへ両方向に入り、全辺一覧では一本として数える。多重辺と自己辺を許す。

`graph_type`、`weighted_graph_type`、`directed_graph_type`、`undirected_graph_type`を用意し、
アルゴリズム本体は有向・無向で複製せず、一つのtemplate関数で実装する。

## 最短路

`shortestpath.hpp`は次を提供する。

- `bfs`: `O(n+m)`。重みを無視した辺数最短路。
- `zero_one_bfs`: `O(n+m)`。重み0/1の最短路。
- `dijkstra`: `O((n+m)log n)`。非負重みの最短路。
- `bellman_ford`: `O(nm)`。負辺と到達可能な負閉路の影響範囲に対応。

すべて単一始点と多点始点に対応する。結果の`operator[]`で距離、`reachable`で到達判定、
`path`で頂点列、`edge_path`で辺番号列を復元できる。

## トポロジカルソート

`topological_sort`は`O(n+m)`で順序、DAG判定、一意性を返す。
`topological_sort_min`は`O((n+m)log n)`で辞書順最小の順序を返す。
有向グラフ以外を渡すとコンパイル時に拒否する。

## 連結性

`connected_components`は無向グラフを`O(n+m)`で連結成分分解する。
`bipartite_coloring`は二部彩色と、失敗時に同色を結ぶ辺番号を返す。
判定だけなら`is_bipartite`を使える。

## 最小全域木・最小全域森

`minimum_spanning_forest`はKruskal法により`O(m log m)`で最小全域森を返す。
結果には重み合計、採用辺番号、連結成分数が入る。
連結が保証される場合は`minimum_spanning_tree`を使い、非連結ならassertする。

## LowLink

`lowlink`は反復DFSにより`O(n+m)`で関節点と橋を求める。多重辺と自己辺にも対応する。
`two_edge_connected_components`は橋を除き、`O(n+m)`で二辺連結成分分解する。

## 閉路検出

`find_cycle`は有向・無向の両方を一つのtemplate関数で扱い、`O(n+m)`で閉路を一つ返す。
結果の`vertices[i]`から`edge_ids[i]`を通って次の頂点へ進む。多重辺と自己辺にも対応する。

## Warshall–Floyd

`warshall_floyd`は重み付きグラフの全点対最短路を時間`O(n^3)`、空間`O(n^2)`で求める。
距離、到達判定、頂点列復元、負閉路とその影響を受ける頂点対の判定を提供する。

## `sccgraph` / 共通グラフのSCC

`sccgraph`へ有向辺を追加し、`scc_ids()`または`scc()`で強連結成分分解できる。
共通の`directed_graph`には自由関数`poe::scc_ids(graph)`と`poe::scc(graph)`を使う。
いずれも`O(n+m)`で、成分番号はトポロジカル順になる。DFSは反復実装である。

## `twosat`

`add_clause(i, f, j, g)`で`(x_i=f) or (x_j=g)`を追加する。
`satisfiable()`は`O(n+m)`で判定し、成功時の割当を`answer()`で返す。
`force`、`imply`、`add_xor`、`add_equal`、`at_most_one`と添え字アクセスも提供する。
