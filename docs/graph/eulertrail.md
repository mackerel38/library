---
title: "Euler Trail"
documentation_of: //cp/graph/eulertrail.hpp
---

# Euler Trail

- Status: experimental
- Header: `cp/graph/eulertrail.hpp`
- Symbol: `poe::eulertrailresult`, `poe::euler_trail`

## Include

```cpp
#include "graph/eulertrail.hpp"
```

## できること

`directed_graph`と`undirected_graph`の全辺をちょうど一度ずつ通る道を、Hierholzer法で復元する。
重み付きグラフも受け取るが、重みは使用しない。多重辺と自己辺を辺番号で区別する。

```cpp
auto result = euler_trail(graph);
if (!result) cout << "No\n";
else {
    for (int vertex : result.vertex_path) cout << vertex << ' ';
}
```

`euler_trail(graph, start)`とすると始点を固定し、省略または`-1`なら次数から自動選択する。
存在する場合、`vertex_path.size() == edge_path.size() + 1`で、辺番号は追加時の番号である。
辺が0本なら、頂点があれば長さ1の頂点列、0頂点なら空列を成功として返す。

## 計算量

時間`O(n+m)`、追加領域`O(n+m)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `eulertrailresult`

```cpp
struct eulertrailresult
```

Euler路の存在判定と復元結果: result.vertex_path, result.edge_path; 構築O(n+m)。

### `operatorbool`

```cpp
explicit operator bool() const noexcept
```

O(1)。Euler路が存在するならtrueを返す。

### `euler_trail`

```cpp
template <graph_type Graph> eulertrailresult euler_trail(const Graph& graph, int start = -1)
```

O(n+m)。全辺を一度ずつ通るEuler路を返す。start=-1なら適切な始点を自動選択する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

Library Checkerの
[Eulerian Trail (Directed)](https://judge.yosupo.jp/problem/eulerian_trail_directed)と
[Eulerian Trail (Undirected)](https://judge.yosupo.jp/problem/eulerian_trail_undirected)が直接の代表問題である。
出力形式を確認したverifyコードは未作成。

## 検証

- `tests/api/graph/eulertrail.cpp`: 有向・無向・自己辺・始点固定
- `tests/property/graph/eulertrail.cpp`: 小グラフで全辺順を探索する愚直解との比較
