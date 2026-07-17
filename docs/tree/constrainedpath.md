---
title: "条件付き最長パス"
documentation_of: //cp/tree/constrainedpath.hpp
---

# 条件付き最長パス

- Header: `cp/tree/constrainedpath.hpp`
- Symbol: `poe::longest_constrained_path`
- Status: experimental

## どんな問題に使えるか

木上の単純パスについて、端点に必要な条件と内部頂点に必要な条件が異なる場合に、
条件を満たす最大頂点数を`O(n)`で求める。次数制約だけでなく、色・使用可能フラグ・
頂点属性などをpredicateへ渡せる。

```cpp
#include "tree/constrainedpath.hpp"

auto length = longest_constrained_path(
    graph,
    [&](int v) { return graph[v].size() >= 3; },
    [&](int v) { return graph[v].size() >= 4; }
);
```

端点だけからなる1頂点pathも許す。空の木では0を返す。辺数が必要なら、非空の答えから
1を引く。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `longest_constrained_path`

```cpp
template <undirected_graph_type Graph, class EndpointPredicate, class InternalPredicate> int longest_constrained_path(const Graph& graph, EndpointPredicate endpoint, InternalPredicate internal)
```

O(n)。端点がendpoint、内部頂点がinternalを満たす木上pathの最大頂点数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC447 F - Centipede Graph](https://atcoder.jp/contests/abc447/tasks/abc447_f)では、
ムカデの胴体を「端点の次数3以上、内部の次数4以上」のpathへ言い換えて使う。
`verify/atcoder_abc447_f.cpp`に提出用コードを用意し、公式sampleを確認済み。judge ACは未確認。
