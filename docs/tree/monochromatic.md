---
title: "動的な同色連結成分"
documentation_of: //cp/tree/monochromatic.hpp
---

# 動的な同色連結成分

- Header: `cp/tree/monochromatic.hpp`
- Symbol: `poe::monochromatictree`
- Status: experimental

## どんな問題に使えるか

固定木の各頂点が色0または1と加算可能な重みを持つとき、色反転と重み更新を行いながら、
指定頂点を含む同色連結成分の重み総和を求める。

```cpp
#include "tree/monochromatic.hpp"

monochromatictree data(graph, weight, color);
data.flip(vertex);
data.add(vertex, delta);
data.set(vertex, value);
cout << data.sum(vertex) << '\n';
```

`color(vertex)`は現在色、`operator[](vertex)`は現在重みを返す。
色数が3以上、辺の色、成分の最大値など加算以外の集約にはこの型を使えない。

## 計算量と仕組み

根から連続する同色区間の上端をHLD上のFenwick Treeで探し、各色について「同色の子だけを
辿った部分木和」を木上区間加算・一点取得で維持する。頂点数`n`として構築`O(n log^2 n)`、
`flip`、`add`、`set`、`sum`はいずれも`O(log^2 n)`、領域`O(n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `monochromatictree`

```cpp
template<class T, undirected_graph_type Graph> struct monochromatictree
```

二色木で色反転・重み更新・同色連結成分和を扱う。構築O(n log^2 n)、各操作O(log^2 n)。

### `monochromatictree`

```cpp
monochromatictree( const Graph& graph, const std::vector<T>& weight, const std::vector<int>& color, int root = 0 ) : tree_(graph, root), color_(graph.size()), weight_(graph.size()), color_count_
```

O(n log^2 n)。各頂点のweightと0/1のcolorから構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `color`

```cpp
int color(int vertex) const
```

O(1)。vertexの現在の色を返す。

### `operator[]`

```cpp
const T& operator[](int vertex) const
```

O(1)。vertexの現在の重みを返す。

### `add`

```cpp
void add(int vertex, const T& delta)
```

O(log^2 n)。vertexの重みにdeltaを加える。

### `set`

```cpp
void set(int vertex, const T& value)
```

O(log^2 n)。vertexの重みをvalueへ置き換える。

### `flip`

```cpp
void flip(int vertex)
```

O(log^2 n)。vertexの色を反転する。

### `sum`

```cpp
T sum(int vertex) const
```

O(log^2 n)。vertexを含む同色連結成分の重み総和を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC460 G - Vertex Flip Query](https://atcoder.jp/contests/abc460/tasks/abc460_g):
  queryを公開APIへそのまま対応させられる。

`verify/atcoder_abc460_g.cpp`で公式sample二つを確認済み。
小さいランダム木で色反転・加算・代入・成分和を愚直BFSと照合する。
