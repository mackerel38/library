---
title: "頂点有効化と到達可能性"
documentation_of: //cp/graph/incrementalreachability.hpp
---

# 頂点有効化と到達可能性

- Header: `cp/graph/incrementalreachability.hpp`
- Symbol: `poe::incrementalreachabilityresult`, `poe::incremental_reachability`
- Status: experimental

## どんな問題に使えるか

有向グラフの頂点を指定順に有効化し、各段階の誘導部分グラフでsource集合から到達可能な
頂点数と、各頂点へ初めて到達するstepを`O(n+m)`で求める。辺・頂点を毎回一から探索
し直さず、未有効な頂点へ到達する辺を予約しておく。

```cpp
#include "graph/incrementalreachability.hpp"

auto result = incremental_reachability(graph, order, sources);
int reached = result.count[step];
int first_step = result.reached_at[vertex];

// 頂点番号順・単一sourceなら短いoverloadを使える。
auto natural = incremental_reachability(graph, source);
```

`order`は全頂点の順列でなければならない。`reached_at[v] == -1`なら、全頂点を有効化しても
sourceから到達できない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `incrementalreachabilityresult`

```cpp
struct incrementalreachabilityresult
```

頂点有効化に伴う到達情報。reached_at[v]は初めて到達するstep、count[i]はstep i後の到達数。

### `incremental_reachability`

```cpp
template <directed_graph_type Graph> incrementalreachabilityresult incremental_reachability( const Graph& graph, const std::vector<int>& order, const std::vector<int>& sources )
```

O(n+m)。order順に頂点を有効化した各時点の、sourcesからの有効頂点内到達集合を返す。

### `incremental_reachability`

```cpp
template <directed_graph_type Graph> incrementalreachabilityresult incremental_reachability(const Graph& graph, int source)
```

O(n+m)。頂点0,1,...の順に有効化した各時点のsourceからの到達情報を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC446 F - Reachable Set 2](https://atcoder.jp/contests/abc446/tasks/abc446_f)では、
頂点`1..k`だけを有効化した誘導部分グラフで、頂点1から全頂点へ到達できるかを各`k`について
判定する。`verify/atcoder_abc446_f.cpp`に提出用コードを用意し、公式sampleを確認済み。
judge ACは未確認。
