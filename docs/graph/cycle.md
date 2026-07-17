---
title: "閉路"
documentation_of: //cp/graph/cycle.hpp
---

# 閉路

## Include

```cpp
#include "graph/cycle.hpp"
```

有向・無向グラフから閉路を一つ`O(n+m)`で返す。多重辺と自己辺に対応する。
`vertices[i]`から`edge_ids[i]`を通って次の頂点へ進む。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `cycleresult`

```cpp
struct cycleresult
```

閉路検出結果。vertices[i]からedge_ids[i]を通りvertices[(i+1)%k]へ進む。

### `found`

```cpp
bool found() const noexcept
```

O(1)。閉路が見つかったならtrueを返す。

### `minimumcycleresult`

```cpp
template<class Cost> struct minimumcycleresult
```

重み付き最小cycle。vertices[i]からedge_ids[i]を通りvertices[(i+1)%k]へ進む。

### `found`

```cpp
bool found() const noexcept
```

O(1)。cycleが見つかったならtrueを返す。

### `find_cycle`

```cpp
template<graph_type Graph> cycleresult find_cycle(const Graph& graph)
```

O(n+m)。有向・無向グラフから閉路を一つ返す。存在しなければ空の結果を返す。

### `minimum_cycle_through_vertex`

```cpp
template<undirected_graph_type Graph> requires Graph::is_weighted minimumcycleresult<typename Graph::cost_type> minimum_cycle_through_vertex( const Graph& graph, int root, int forbidden_edge = -1 )
```

O(n^2+m)。非負重み無向グラフでrootを通る最小単純cycleを返す。forbidden_edgeは使用しない。

### `count_simple_cycles`

```cpp
template <class T> T count_simple_cycles(const std::vector<std::vector<int>>& multiplicity)
```

O(2^n n^2)時間・O(2^n n)領域。n<=20の多重無向グラフの頂点単純cycle数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Cycle Detection](https://judge.yosupo.jp/problem/cycle_detection)。

```cpp
auto cycle = poe::find_cycle(graph);
if (!cycle.found()) std::cout << -1 << '\n';
else {
    std::cout << cycle.edge_ids.size() << '\n';
    for (int id : cycle.edge_ids) std::cout << id << '\n';
}
```

`verify/cycle.test.cpp`で2026-07-15に全ケースAC確認済み。

## 指定頂点を通る最小重みcycle

`minimum_cycle_through_vertex(graph, root, forbidden_edge)`は非負重み無向グラフで
`root`を通る最小単純cycleを返す。`forbidden_edge`を省略すると全辺を使い、
指定するとその辺だけを使用禁止にする。

```cpp
auto cycle = poe::minimum_cycle_through_vertex(graph, root);
if (cycle.found()) {
    std::cout << cycle.cost << '\n';
}
```

根からの最短路木を作り、各頂点を根の直下で最初に通る子ごとに分類する。
異なる分類を結ぶ最小の非木辺が、二本の最短路と合わせて最小cycleになる。
計算量`O(n^2+m)`、領域`O(n)`。辺重みは非負であること。

[AtCoder ABC308 Ex - Make Q](https://atcoder.jp/contests/abc308/tasks/abc308_h)では、
cycle上のtail接続頂点を全探索し、最小cycleとcycle隣接辺を禁止した最小cycleを使う。
`verify/atcoder_abc308_h.cpp`で公式sample 3件を確認済み。

[AtCoder ABC456 E - Endless Holidays](https://atcoder.jp/contests/abc456/tasks/abc456_e)では、
都市と曜日の直積グラフに無限に歩けることを有向閉路の存在へ言い換える。
`verify/atcoder_abc456_e.cpp`に使用例を収録した。

## 全サイクルの数え上げ

`count_simple_cycles<T>(multiplicity)`は20頂点以下の多重無向グラフについて、
使う頂点が互いに異なるサイクルを辺集合の違いまで区別して数える。
`multiplicity[u][v]`へ頂点間の辺数を渡す。多重辺2本からなる長さ2サイクルも含む。

最小頂点を固定したsubset DPにより、時間`O(2^n n^2)`、領域`O(2^n n)`。
戻り値型`T`には四則演算と整数からの構築が必要で、通常はmodintを使う。

[AtCoder ABC411 G - Count Cycles](https://atcoder.jp/contests/abc411/tasks/abc411_g)で
`verify/atcoder_abc411_g.cpp`を用意し、公式sampleを確認済み。
小さい多重グラフでは全辺部分集合の直接判定と比較している。
