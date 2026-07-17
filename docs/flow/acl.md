# Flow

個別文書: [flowgraph](flowgraph.md)、[dinic](dinic.md)、[maxflow](maxflow.md)、
[mincost](mincost.md)、[mincostflow](mincostflow.md)、[lowerbound](lowerbound.md)、
[submodular](submodular.md)。

新規コードでは、状態を持つ`flowgraph<Cap>`・`costflowgraph<Cap, Cost>`と、
自由関数`dinic/mincut/mincost/mincostslope`を組み合わせる。詳しい設計と下限制約・
劣モジュラ最小化は[design.md](design.md)を参照する。

## `maxflow<Cap>`

`flowgraph`と`dinic`を包むACL互換wrapper。
`add_edge/get_edge/edges/change_edge/flow/min_cut`を提供する。
`flow`は現在の残余グラフへ追加で流した量を返すため、複数回呼び出せる。
最大流後の`min_cut(source)`は最小カットのsource側を返す。

## `mincostflow<Cap, Cost>`

`costflowgraph`と`mincost`を包むACL互換wrapper。ポテンシャル付きDijkstraによる最小費用流。
`add_edge/get_edge/edges/flow/slope`を提供する。追加辺の費用は非負であること。
`slope`は`{流量, 最小費用}`の折れ点を返し、同じ傾きの連続点をまとめる。
同じグラフに対する`flow`と`slope`の呼び出しは、合わせて1回だけとする。
