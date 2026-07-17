# Tree

個別文書: [rootedtree](rootedtree.md)、[lca](lca.md)、
[heavylightdecomposition](heavylightdecomposition.md)、[diameter](diameter.md)、
[rerooting](rerooting.md)。

木アルゴリズムは`undirected_graph`を受け取り、入力が連結な木であることを前提とする。
実装は再帰を使わず、長い一本道でも実行時スタックを消費しない。

## `rootedtree`

`rootedtree tree(graph, root)`で無向木を根付き木として`O(n)`で前処理する。
親、親辺、深さ、根からの距離、部分木サイズ、DFS行きがけ順、祖先判定を`O(1)`で取得できる。

## `lcatree`

`lcatree tree(graph, root)`で二分累乗表を`O(n log n)`で構築する。
`lca`、`kth_ancestor`、`jump`、二頂点間の重み和と辺数を`O(log n)`で取得できる。

```cpp
poe::lcatree tree(graph, 0);
int ancestor = tree.lca(left, right);
int vertex = tree.jump(from, to, step);
auto distance = tree.distance(from, to);
```

## `hld`

Heavy-Light Decompositionを`O(n)`で構築する。`path`は頂点パス、`edge_path`は辺パスを
`O(log n)`個の`pathsegment`へ分解する。各区間は半開区間で、`reversed`は区間を読む方向を表す。
可換な区間和などでは方向を無視でき、非可換な合成では方向に従って処理する。

`subtree(vertex)`は部分木区間、`edge_index(id)`は辺を子頂点の位置へ変換する。

## 木の直径

`tree_diameter`は非負重みの木の直径を`O(n)`で求め、距離、両端点、頂点列、辺番号列を返す。
重みなしグラフでは各辺を重み1として扱う。

## `rerooting`

`rerooting`は全方位木DPを`O(n)`で行う。単位元、結合関数、頂点を加える関数、辺を越える関数を渡す。
有向・無向用の実装を複製せず、共通の`undirected_graph_type` conceptで受け取る。

```cpp
auto answer = poe::rerooting(graph, identity, merge, add_vertex, add_edge);
```

`merge`は結合則を満たし、指定した`identity`を単位元とする必要がある。
