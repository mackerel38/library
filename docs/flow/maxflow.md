---
title: "maxflow"
documentation_of: //cp/flow/maxflow.hpp
---

# `maxflow`

## 概要

Dinic法による最大流と最小カットを扱う。

## 厳密な定義

- `maxflow`: ACL互換の最大流: maxflow<long long> graph(n); 新規コードではflowgraphとdinicも使える。


## Include

```cpp
#include "flow/maxflow.hpp"
```

ACL互換APIを持つDinic法のwrapper。`add_edge/get_edge/edges/change_edge/flow/min_cut`を提供する。
`flow`は現在の残余グラフへ追加で流した量を返す。一般計算量は`O(n^2m)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `maxflow`

```cpp
template<class Cap> struct maxflow : flowgraph<Cap>
```

ACL互換の最大流: maxflow<long long> graph(n); 新規コードではflowgraphとdinicも使える。

### `flow`

```cpp
Cap flow(int source, int sink)
```

O(n^2m)。sourceからsinkへ流せる最大流を追加し、その量を返す。

### `flow`

```cpp
Cap flow(int source, int sink, Cap limit)
```

O(n^2m)。sourceからsinkへlimitまで最大流を追加し、その量を返す。

### `min_cut`

```cpp
std::vector<bool> min_cut(int source) const
```

O(n+m)。残余グラフでsourceから到達できる頂点を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ GRL_6_A - Maximum Flow](https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A)。

```cpp
poe::maxflow<long long> graph(n);
for (auto [from, to, cap] : edges) graph.add_edge(from, to, cap);
std::cout << graph.flow(0, n - 1) << '\n';
```

`verify/maxflow.test.cpp`で2026-07-15に全ケースAC確認済み。

[AtCoder ABC437 G - Colorful Christmas Tree](https://atcoder.jp/contests/abc437/tasks/abc437_g)では、
木の各辺を削除するときの両端色割当を二部最大流で選ぶ。
`verify/atcoder_abc437_g.cpp`は公式サンプル条件を満たす出力を確認済み。
