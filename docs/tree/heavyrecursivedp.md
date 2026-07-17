---
title: "重軽再帰DP"
documentation_of: //cp/tree/heavyrecursivedp.hpp
---

# 重軽再帰DP

- Header: `cp/tree/heavyrecursivedp.hpp`
- Symbol: `poe::heavyrecursivedp`
- Status: experimental

## どんな問題に使えるか

通常の木DPでは、各子のDP配列を畳み込むと容量二乗などの重い計算になることがある。
一方、部分木が「外から受け取った一つの状態を、少数個の状態へ変換する作用」として書けるなら、
変換を上から順に適用して畳み込みを避けられる。

`heavyrecursivedp<Branches>`は最も大きい子部分木への再帰を全branchで共有し、
他の子だけbranchごとに再計算する重軽再帰DPの制御を担当する。

```cpp
#include "tree/heavyrecursivedp.hpp"

auto root_states = poe::heavyrecursivedp<2>(
    graph,
    root,
    initial,
    [&](int vertex, auto& states) {
        // states[0], states[1]へ頂点vertexの遷移を加える
    },
    [&](int vertex, const auto& states) {
        // initialから始めたvertex部分木の結果を保存する
    }
);
```

`transition(vertex, states)`は、子部分木の作用を適用済みの`Branches`個の状態を受け取り、
頂点自身の作用をその場で加える。`record(vertex, states)`は全頂点について一度ずつ呼ばれ、
常に`initial`から始めたその頂点の部分木結果を受け取る。
全頂点の記録が不要なら`record`を省略したoverloadを使える。

branch数は小さい定数を想定する。状態のcopyが高価なときも、heavy childでは同じ入力からの計算を
一度だけ共有する。再帰を使うため、極端に深い木では実行環境のstack上限に注意する。

## 計算量

頂点作用一回の計算量を`D`、部分木サイズ`n`に対する実行時間を`f(n)`とする。
最大の子を一回、残りの子を`Branches`回処理するので、

`f(n) <= f(n1) + Branches * sum_{i>=2} f(ni) + O(D)`

を満たす。二分に均等分割される場合の目安は
`O(n^(log2(Branches+1)) D)`時間である。
`Branches=2`なら`O(n^1.585 D)`時間、追加領域は状態copyを含め`O(state_size log n)`が目安となる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `heavyrecursivedp`

```cpp
template< std::size_t Branches, undirected_graph_type Graph, class State, class Transition, class Record > std::array<State, Branches> heavyrecursivedp( const Graph& graph, int root, const State& initial, Transition transition,
```

O(f(n))。heavyrecursivedp<B>(graph, root, initial, transition, record)で分岐型部分木DPを実行する。
transition(vertex, states)はB個の状態を更新し、record(vertex, states)はinitial始点の各部分木結果を受け取る。

### `heavyrecursivedp`

```cpp
template<std::size_t Branches, undirected_graph_type Graph, class State, class Transition> std::array<State, Branches> heavyrecursivedp( const Graph& graph, int root, const State& initial, Transition transition )
```

O(f(n))。recordが不要な固定根の分岐型部分木DPを実行し、rootのB状態を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC311 Ex - Many Illumination Plans](https://atcoder.jp/contests/abc311/tasks/abc311_h)では、
二色それぞれを親側の色とするナップサックDPを二branchとして渡す。
`verify/atcoder_abc311_h.cpp`で公式sample 3件を確認済み。judge ACは未確認。
