---
title: "グラフ彩色数"
documentation_of: //cp/graph/coloring.hpp
---

# グラフ彩色数


## 概要

頂点数・辺数がともに30以下の単純無向グラフについて、隣接頂点を異色にする
`K`彩色の個数を数える。

## 厳密な定義

- `count_graph_colorings`: O(m^2 1.59^m)程度。30辺以下の単純無向graphのK彩色数を返す。

## Include

```cpp
#include "graph/coloring.hpp"

auto answer = poe::count_graph_colorings<998244353>(graph, colors);
```

次数0・1は係数を掛けて頂点を消し、次数2は二近傍が同色か異色かで削除・縮約へ分ける。
最小次数が3以上になった部分だけ、独立集合のindicatorを
`subset_convolution_unit_power`で`K`乗する。巨大な`K`を色ごとに反復しない。

30辺以下では`O(m^2 1.59^m)`程度、最大部分集合DPは
`O(n^2 2^n)`時間・`O(n 2^n)`領域。法は途中の`1..n`で除算可能でなければならない。

## インターフェイス

```cpp
template <int modulus, undirected_graph_type Graph>
staticmodint<modulus> count_graph_colorings(
    const Graph& graph, long long colors);
```

- `graph`: 彩色する単純無向グラフ。自己辺・多重辺は不可。
- `colors`: 使用できる色数。`0`以上。
- 戻り値: 隣接頂点が異色になる頂点彩色の総数を`modulus`で割った値。

`graph.size() <= 30`かつ`graph.edge_count() <= 30`が必要。再帰の途中で最小次数が
3以上になった部分は24頂点以下でなければならない。`modulus`は素数を想定し、
途中で現れる`1..n`が法の上で逆元を持つ必要がある。入力のグラフは変更しない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_graph_colorings`

```cpp
template <int modulus, undirected_graph_type Graph> staticmodint<modulus> count_graph_colorings(const Graph& graph, long long colors)
```

O(m^2 1.59^m)程度。30辺以下の単純無向graphのK彩色数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC294 Ex - K-Coloring](https://atcoder.jp/contests/abc294/tasks/abc294_h)へ直接使える。
`verify/atcoder_abc294_h.cpp`で公式sample 5件を確認済み。

## 検証

- `tests/api/graph/coloring.cpp`: 空グラフ・木・完全グラフの既知値
- `tests/property/graph/coloring.cpp`: 小グラフの全彩色列挙との比較
