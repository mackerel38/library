---
title: "根から各頂点へのpath rollback"
documentation_of: //cp/tree/rootpath.hpp
---

# 根から各頂点へのpath rollback

- Header: `cp/tree/rootpath.hpp`
- Symbol: `poe::root_path_rollback`
- Status: experimental

## どんな問題に使えるか

木を一つの根から反復DFSし、根から現在頂点までのpathに対応する状態を全頂点で求める。
状態型は`snapshot()`と`rollback(snapshot)`を持つ必要がある。

```cpp
#include "tree/rootpath.hpp"

auto answer = poe::root_path_rollback(
    graph, root, state,
    [&](auto& state, int vertex) {
        state.add(value[vertex]);
    },
    [&](auto& state, int vertex) {
        return state.value();
    }
);
```

各頂点へ入る直前にsnapshotを取り、`add(state, vertex)`後の`get(state, vertex)`を
`answer[vertex]`へ保存する。部分木を出ると状態を元へ戻し、関数終了時も呼出し前の状態に戻る。
再帰を使わないため、長い一本道でも実行時stackを消費しない。

頂点数を`n`として走査自体は`O(n)`。全体は`add`、`get`、`rollback`を各頂点一回呼ぶ計算量となる。
空でない連結な木を要求する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `root_path_rollback`

```cpp
template<undirected_graph_type Graph, class State, class Add, class Get> auto root_path_rollback( const Graph& graph, int root, State& state, Add add, Get get )
```

O(n+各callback)。stateを追加・rollbackし、根から各頂点へのpath状態を全頂点で評価する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC302 Ex - Ball Collector](https://atcoder.jp/contests/abc302/tasks/abc302_h)では、
状態に`rollbackgraphdsu`を使い、各木頂点の二つの値を無向辺として追加する。
`verify/atcoder_abc302_h.cpp`で公式sample 2件を確認済み。

## 検証

- `tests/api/tree/rootpath.cpp`: path上の加算値と関数終了後のrollback
- `tests/property/tree/rootpath.cpp`: 各頂点から親を辿る愚直値との比較
