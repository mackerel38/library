---
title: "動的頂点集合の直径"
documentation_of: //cp/tree/vertexsetdiameter.hpp
---

# 動的頂点集合の直径


## 概要

固定された重みなし木上で、候補頂点を追加・削除しながら候補同士の最大距離を求める。
候補集合の直径は、二集合それぞれの直径端点4個だけで結合できる性質を使う。

## 厳密な定義

- `vertexsetdiameterresult`: 動的頂点集合の直径。空ならleft=right=-1、singletonならleft=right。
- `vertexsetdiameter`: 頂点の有効無効を更新し、有効頂点間の最大辺距離を管理する。構築O(n log n)、更新O(log n)。

## Include

```cpp
#include "tree/vertexsetdiameter.hpp"

vertexsetdiameter diameter(graph); // 初期状態は全頂点active
diameter.flip(vertex);
auto result = diameter.get();
cout << result.distance << '\n';
```

初期状態を個別指定するときは`vertexsetdiameter(graph, active)`とする。
`set(vertex, value)`、`flip(vertex)`、`active(vertex)`を持つ。
結果は`left`、`right`、`distance`を持ち、空集合では`empty()==true`かつ両端が`-1`、
一点集合では両端が同じで距離0になる。

## 計算量と前提

Euler tour上のRMQでLCA距離を`O(1)`にし、頂点集合をsegment tree状に結合する。
頂点数を`n`として構築`O(n log n)`、更新`O(log n)`、全体取得と距離取得`O(1)`、領域`O(n log n)`。
入力は連結な重みなし木であること。辺重み付き直径には使えない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `vertexsetdiameterresult`

```cpp
struct vertexsetdiameterresult
```

動的頂点集合の直径。空ならleft=right=-1、singletonならleft=right。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。集合が空ならtrue。

### `vertexsetdiameter`

```cpp
template<undirected_graph_type Graph> struct vertexsetdiameter
```

頂点の有効無効を更新し、有効頂点間の最大辺距離を管理する。構築O(n log n)、更新O(log n)。

### `vertexsetdiameter`

```cpp
explicit vertexsetdiameter(const Graph& graph) : vertexsetdiameter(graph, std::vector<bool>(graph.size(), true))
```

O(n log n)。全頂点をactiveにして構築する。

### `vertexsetdiameter`

```cpp
vertexsetdiameter(const Graph& graph, const std::vector<bool>& active) : tour_(graph), rmq_(make_rmq_values()), active_(active)
```

O(n log n)。active[v]で初期状態を指定する。

### `size`

```cpp
int size() const noexcept
```

O(1)。元の木の頂点数を返す。

### `active`

```cpp
bool active(int vertex) const
```

O(1)。vertexが有効ならtrue。

### `set`

```cpp
void set(int vertex, bool value)
```

O(log n)。vertexの有効無効をvalueへ変更する。

### `flip`

```cpp
void flip(int vertex)
```

O(log n)。vertexの有効無効を反転する。

### `get`

```cpp
const vertexsetdiameterresult& get() const noexcept
```

O(1)。有効頂点集合の直径端点と距離を返す。

### `distance`

```cpp
int distance(int left, int right) const
```

O(1)。二頂点間の辺数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC460 F - Farthest Pair Query](https://atcoder.jp/contests/abc460/tasks/abc460_f):
  黒頂点をactiveとして各queryで`flip`し、`get().distance`を出力する。

`verify/atcoder_abc460_f.cpp`で公式sampleを確認済み。
API testと小さいランダム木・ランダム更新を全頂点対距離の愚直計算と照合する。
