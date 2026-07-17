# ACL相当ライブラリ一覧

AtCoder Libraryの公開機能を`poe`名前空間と本リポジトリの入力規約に合わせて実装した。
すべて`experimental`から開始し、ACLそのものへは提出時・実行時とも依存しない。

| カテゴリ | ヘッダ | 主なAPI |
| --- | --- | --- |
| structure | `cp/structure/dsu.hpp` | `dsu` |
| structure | `cp/structure/fenwicktree.hpp` | `fenwicktree` |
| structure | `cp/structure/segtree.hpp` | `segtree` |
| structure | `cp/structure/lazysegtree.hpp` | `lazysegtree` |
| math | `cp/math/math.hpp` | `pow_mod`, `inv_mod`, `crt`, `floor_sum` |
| math | `cp/math/modint.hpp` | `staticmodint`, `dynamicmodint`,各alias |
| math | `cp/math/convolution.hpp` | `convolution`, `convolution_ll` |
| graph | `cp/graph/graph.hpp` | `directed_graph`, `undirected_graph`, graph concepts |
| graph | `cp/graph/shortestpath.hpp` | BFS、0-1 BFS、Dijkstra、Bellman-Ford、経路復元 |
| graph | `cp/graph/topologicalsort.hpp` | トポロジカルソート、DAG・一意性判定 |
| graph | `cp/graph/connectivity.hpp` | 連結成分、二部グラフ判定 |
| graph | `cp/graph/minimumspanningtree.hpp` | 最小全域木・最小全域森 |
| graph | `cp/graph/lowlink.hpp` | 関節点、橋、二辺連結成分 |
| graph | `cp/graph/cycle.hpp` | 有向・無向グラフの閉路検出 |
| graph | `cp/graph/warshallfloyd.hpp` | 全点対最短路、負閉路判定、経路復元 |
| graph | `cp/graph/scc.hpp` | `sccgraph`、共通有向グラフのSCC |
| graph | `cp/graph/twosat.hpp` | `twosat` |
| tree | `cp/tree/rootedtree.hpp` | 根付き木の親・深さ・部分木・DFS順 |
| tree | `cp/tree/lca.hpp` | LCA、祖先、木上jump、距離 |
| tree | `cp/tree/heavylightdecomposition.hpp` | HLDによる頂点・辺パスと部分木区間 |
| tree | `cp/tree/diameter.hpp` | 木の直径と経路復元 |
| tree | `cp/tree/rerooting.hpp` | 汎用全方位木DP |
| flow | `cp/flow/maxflow.hpp` | `maxflow` |
| flow | `cp/flow/mincostflow.hpp` | `mincostflow` |
| flow | `cp/flow/flowgraph.hpp` | `flowgraph`, `costflowgraph` |
| flow | `cp/flow/dinic.hpp` | `dinic`, `mincut` |
| flow | `cp/flow/mincost.hpp` | `mincost`, `mincostslope` |
| flow | `cp/flow/lowerbound.hpp` | `lowerboundflow` |
| flow | `cp/flow/submodular.hpp` | `submodular` |
| string | `cp/string/suffixarray.hpp` | `suffix_array` |
| string | `cp/string/lcp.hpp` | `lcp_array` |
| string | `cp/string/zalgorithm.hpp` | `z_algorithm` |
| string | `cp/string/prefix.hpp` | `prefix_function`, `find_all` |
| string | `cp/string/manacher.hpp` | `manacher_odd`, `manacher_even` |
| string | `cp/string/suffixquery.hpp` | `suffixquery` |
| utility | `cp/utility/presetsegtree.hpp` | 用途別のpoint/range構造21種 |

物理ファイルは`cp/`以下にあるが、`cp`をinclude rootとする。カテゴリ一括includeは
`#include "<category>/all.hpp"`、全体一括includeは`#include "all.hpp"`で使う。
個別ヘッダのincludeを提出時の標準とし、一括includeは探索・試作時の便利機能とする。

## ACLとの差

- 公開型は小文字の`struct`を基本とする。
- `fenwicktree`の`set`、`operator[]`、累積和二分探索など、実戦向け操作を追加する。
- `segtree`と`lazysegtree`は`operator[]`を持つ。
- modintはstream入出力を持つ。
- stringは機能別ヘッダへ分割し、`#include "string/string.hpp"`でもまとめて使える。
- `utility/presetsegtree.hpp`には実装方式を型名へ出さない用途別区間構造を収録する。
- 補完用コメントは日本語で、呼び出し例・計算量・重要な前提を記載する。

詳細な事前条件と計算量は、各ヘッダの公開API直前コメントとカテゴリ文書を正とする。
