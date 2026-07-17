# フロー構造の設計

## 構造と探索の分離

残余グラフの状態と探索アルゴリズムを分ける。容量だけを持つ場合と単位費用も持つ場合は、
実行時boolではなく公開型で区別する。

```cpp
poe::flowgraph<long long> graph(n);
poe::costflowgraph<long long, long long> cost_graph(n);
```

両者は`detail::flowgraphbase<Cap, Cost>`を共有する。`flowgraph`の公開辺は
`from, to, cap, flow`、`costflowgraph`ではさらに`cost`を持つ。残余逆辺の費用は`-cost`になる。

最大流は自由関数として呼ぶ。

```cpp
graph.add_edge(from, to, cap);
long long value = poe::dinic(graph, source, sink);
std::vector<bool> source_side = poe::mincut(graph, source);
```

最小費用流も構造とは分離する。現在の実装は追加辺の費用が非負で、同じグラフに対する
`mincost`と`mincostslope`の呼び出しは合計一回とする。

```cpp
cost_graph.add_edge(from, to, cap, cost);
auto [flow, cost] = poe::mincost(cost_graph, source, sink, limit);
```

ACL互換の`maxflow`と`mincostflow`は、それぞれの構造を継承する薄いwrapperとして残す。

## 下限制約

`lowerboundflow<Cap>`は各辺へ`lower, upper`を指定する。

```cpp
poe::lowerboundflow<long long> problem(n);
problem.add_edge(from, to, lower, upper);
auto maximum = problem.maxflow(source, sink);
```

- `circulation()`は全頂点で流量保存を満たす循環の辺流量を返す。
- `feasible()`はその存在だけを返す。
- `maxflow(source, sink)`は非負のs-t流の最大値と辺流量を返す。
- `minflow(source, sink)`は非負のs-t流の最小値と辺流量を返す。
- 解が存在しない場合は`std::nullopt`を返す。

内部では各辺へ下限を先に流し、超始点・超終点と`sink -> source`辺を加えて実現可能性を判定する。

## 劣モジュラ関数最小化

`submodular<Cost>`は二値変数に対する単項コストと二項劣モジュラコストをgraph cutへ変換する。
任意の集合関数を対象とする一般劣モジュラ最小化ではない。

```cpp
poe::submodular<long long> function(n);
function.add_unary(i, cost0, cost1);
function.add_pair(i, j, cost00, cost01, cost10, cost11);
auto [cost, value] = function.minimize();
```

`add_pair`には`cost00 + cost11 <= cost01 + cost10`が必要である。負の単項・二項コストも
定数項へ移して扱える。`add_neq_cost(i, j, cost)`と
`add_implication_cost(i, j, cost)`を頻出形の補助操作として用意する。
