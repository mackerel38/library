# グラフ構造の設計

- 状態: 基本グラフと主要アルゴリズムを実装済み
- 決定日: 2026-07-14
- 言語: GNU++23。古い言語規格への互換性のためにconceptやrangesを避けない。

## 公開型

有向・無向は、辺を追加するたびに指定せずグラフの型として固定する。
重み型を省略した場合は重みなしグラフとする。

```cpp
poe::directed_graph<> graph(n);
poe::undirected_graph<> graph(n);
poe::directed_graph<long long> graph(n);
poe::undirected_graph<long long> graph(n);
```

辺追加は全形式で`add_edge`に統一する。

```cpp
graph.add_edge(from, to);
graph.add_edge(from, to, cost);
```

`directed_graph`では有向辺を一本追加する。`undirected_graph`では同じ辺番号を持つ隣接辺を
両方向へ追加し、全辺一覧では無向辺一本として数える。

## 内部実装

公開型ごとに実装を複製しない。方向と重み型を受け取る共通実装を`poe::detail`へ置き、
公開型はaliasまたは薄いwrapperとして提供する。

```cpp
namespace poe::detail {

template<bool Directed, class Cost>
struct graphbase;

}

template<class Cost = void>
using directed_graph = detail::graphbase<true, Cost>;

template<class Cost = void>
using undirected_graph = detail::graphbase<false, Cost>;
```

`Cost = void`は重みなしを表す。公開型から参照できる性質として、少なくとも次を用意する。

```cpp
static constexpr bool is_directed;
static constexpr bool is_weighted;
using cost_type = Cost;
```

## アルゴリズム

方向を問わないBFSなどは、両方を受け取る一つのtemplate関数として実装する。
同じ関数本体を有向・無向で複製しない。

```cpp
template<graph_type Graph>
auto bfs(const Graph& graph, int start);
```

特定の性質が必要な処理はconceptと`requires`で制限する。

```cpp
template<graph_type Graph>
    requires Graph::is_directed
auto topological_sort(const Graph& graph);

template<graph_type Graph>
    requires (!Graph::is_directed)
auto minimum_spanning_tree(const Graph& graph);

template<weighted_graph_type Graph>
auto dijkstra(const Graph& graph, int start);
```

この制約は実行時検査の代用ではなく、意味上成立しない組合せを利用箇所でコンパイルエラーにするために使う。

## 一般グラフとフローの境界

一般グラフの辺へ容量、残余容量、逆辺番号を持ち込まない。フローの残余グラフは意味と更新方法が
異なるため、別設計とする。一般グラフ側では頂点番号や命名方針だけを共有する。

## 実装で確定した項目

- 重みなし辺は`from`、`to`、`id`、重み付き辺はさらに`cost`を持つ。
- `operator[]`は隣接辺列、`edge_at`は追加時の向きの辺、`edges`は全辺列をconst参照で返す。
- 公開型は共通実装のaliasとし、利用側では内部のboolを指定させない。
- 整数距離の加算は広い整数型で計算し、ライブラリの無限大値へ飽和させる。
- `sccgraph`を維持し、共通有向グラフ向けの自由関数`scc_ids`と`scc`を追加する。
