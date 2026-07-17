---
title: "最寄りの有効頂点"
documentation_of: //cp/recipe/nearestactive.hpp
---

# 最寄りの有効頂点

- Status: experimental
- Header: `cp/recipe/nearestactive.hpp`
- Symbol: `poe::nearestactive<Graph>`, `poe::nearestactiveresult<Distance>`

## Include

```cpp
#include "recipe/nearestactive.hpp"
```

`recipe`なので`all.hpp`には含めない。必要な問題で明示的にincludeする。

## できること

木の頂点を有効・無効に更新しながら、指定頂点から最も近い有効頂点を求める。
重心分解のancestor距離と重心ごとのmultiset管理を内部で行う。

```cpp
nearestactive active(graph);
active.add(0);
active.toggle(vertex);
auto answer = active.nearest(query_vertex);
if (answer) cout << answer->vertex << ' ' << answer->distance << '\n';
```

- `add` / `remove`: 有効化・無効化。状態が変わったなら`true`。
- `set(vertex, enabled)`: 指定状態へ変更。
- `toggle`: 状態を反転し、反転後の状態を返す。
- `nearest`: 最寄り頂点と距離。距離が同じなら頂点番号が小さいもの。
- `distance`: 距離だけを返す。
- `operator[]`: 現在の有効状態。

有効頂点がなければ`nearest`と`distance`は`nullopt`。
辺重みは非負であること。重みなし木は各辺を1と数える。

## 計算量

構築`O(n log n)`、`add/remove/set/toggle`は`O(log^2 n)`、
`nearest/distance`は`O(log n)`、領域`O(n log n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `nearestactiveresult`

```cpp
template <class Distance> struct nearestactiveresult
```

最寄りの有効頂点query結果。距離が同じなら頂点番号が小さいものを返す。

### `nearestactive`

```cpp
template <undirected_graph_type Graph> struct nearestactive
```

有効頂点を更新し最寄りを尋ねる: nearestactive active(graph); 更新O(log^2 n)、query O(log n)。

### `nearestactive`

```cpp
explicit nearestactive(const Graph& graph) : decomposition_(graph), active_(graph.size()), nearest_(graph.size())
```

O(n log n)。全頂点が無効な状態で、非負辺重みの木を前処理する。

### `add`

```cpp
bool add(int vertex)
```

O(log^2 n)。vertexを有効化する。状態が変わったならtrue。

### `remove`

```cpp
bool remove(int vertex)
```

O(log^2 n)。vertexを無効化する。状態が変わったならtrue。

### `toggle`

```cpp
bool toggle(int vertex)
```

O(log^2 n)。vertexの有効・無効を反転し、変更後の状態を返す。

### `set`

```cpp
bool set(int vertex, bool enabled)
```

O(log^2 n)。vertexをenabledで指定した状態にし、状態が変わったならtrue。

### `nearest`

```cpp
std::optional<nearestactiveresult<distance_type>> nearest(int vertex) const
```

O(log n)。vertexから最寄りの有効頂点と距離。存在しなければnullopt。

### `distance`

```cpp
std::optional<distance_type> distance(int vertex) const
```

O(log n)。vertexから最寄りの有効頂点までの距離。存在しなければnullopt。

### `operator[]`

```cpp
bool operator[](int vertex) const
```

O(1)。vertexが有効ならtrue。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Codeforces 342E - Xenia and Tree](https://codeforces.com/problemset/problem/342/E)は、
頂点の有効化と最寄り有効頂点までの距離queryを処理する代表問題である。
`verify/codeforces_342_e.cpp`を収録した。judge未提出。

ABC299 Eも「黒頂点までの最小距離」を扱うが、一般グラフ上で全queryがofflineな構成問題なので、
この動的木recipeを使う問題ではない。

## 検証

- `tests/api/tree/nearestactive.cpp`: 重み付き木の全更新APIとtie break
- `tests/property/tree/nearestactive.cpp`: ランダムtoggle後の全頂点queryを愚直距離と比較
- `verify/codeforces_342_e.cpp`: 公開問題用コード
