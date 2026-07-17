---
title: "辺の向き付けと縮約"
documentation_of: //cp/graph/orientation.hpp
---

# 辺の向き付けと縮約


## 概要

無向グラフの各辺について、

## 厳密な定義

- `count_acyclic_edge_states`: O(3^n)時間・O(2^n)領域。各辺を二方向または同一視にし、縮約後がDAGとなる割当数を返す。 count_acyclic_edge_states<T>(graph): nは小さいこと。Tは加減算と0,1からの構築に対応すること。

- 一方から他方へ向ける
- 逆向きにする
- 両端頂点を同一視する

のいずれかを選び、すべての同一視を行った後の有向グラフがDAGになる割当数を数える。

## Include

```cpp
#include "graph/orientation.hpp"

modint998244353 answer =
    poe::count_acyclic_edge_states<modint998244353>(graph);
```

indegree 0の頂点集合を順に除く包除を、元グラフ上で除く集合の連結成分単位にまとめる。
部分集合`s`の誘導部分グラフの連結成分数を`c(s)`とすると、

`dp[mask] = Σ dp[mask \ s] * (-1)^(c(s)+1)`

となる。計算量`O(3^n)`、領域`O(2^n)`で、頂点数が小さい場合を想定する。
自己辺は許さない。多重辺は入力できるが、辺ごとの3状態を区別する問題では
単純グラフを渡すこと。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_acyclic_edge_states`

```cpp
template<class T, undirected_graph_type Graph> T count_acyclic_edge_states(const Graph& graph)
```

O(3^n)時間・O(2^n)領域。各辺を二方向または同一視にし、縮約後がDAGとなる割当数を返す。
count_acyclic_edge_states<T>(graph): nは小さいこと。Tは加減算と0,1からの構築に対応すること。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC306 Ex - Balance Scale](https://atcoder.jp/contests/abc306/tasks/abc306_h)では、
比較結果の`<`,`>`,`=`をそれぞれ向き付けと同一視に対応させる。
`verify/atcoder_abc306_h.cpp`で公式sample 3件を確認済み。
