---
title: "Euler tour"
documentation_of: //cp/tree/eulertour.hpp
---

# Euler tour

## Include

```cpp
#include "tree/eulertour.hpp"
```

根から隣接リスト順に木を巡回し、頂点へ入った時と子から戻った時を記録する。
頂点数を`n`とすると、完全な巡回列の長さは空木で`0`、それ以外で`2n-1`になる。

`vertices()`は巡回中の頂点列、`edges()`は隣り合う頂点間で使った辺番号列、
`depths()`は各位置での深さを返す。`first(v)`と`last(v)`では頂点`v`の最初・最後の出現位置を得られる。

部分木を配列区間として扱う用途には、完全な巡回列ではなくDFS行きがけ順を使う。
`index(v)`と`subtree(v)`が、それぞれ行きがけ順の位置と部分木の半開区間を返す。
`preorder(i)`で行きがけ順`i`番目の頂点へ戻せる。

構築は`O(n)`、各取得は`O(1)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `eulertour`

```cpp
template<undirected_graph_type Graph> struct eulertour
```

無向木を根から巡回し、頂点へ入る時と子から戻る時の列を作る: eulertour tour(graph, root); 構築O(n)。

### `eulertour`

```cpp
explicit eulertour(const Graph& graph, int root = 0) : tree_(graph, root)
```

O(n)。隣接リストの順に木を巡回する。空でないグラフは連結な木であること。

### `size`

```cpp
int size() const noexcept
```

O(1)。完全Euler tour列の長さを返す。空木では0、通常は2n-1。

### `vertex_count`

```cpp
int vertex_count() const noexcept
```

O(1)。元の木の頂点数を返す。

### `root`

```cpp
int root() const noexcept
```

O(1)。根を返す。空木では-1を返す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(1)。Euler tourのindex番目にいる頂点を返す。

### `edge`

```cpp
int edge(int index) const
```

O(1)。index番目からindex+1番目への移動に使う辺番号を返す。

### `depth`

```cpp
int depth(int index) const
```

O(1)。Euler tourのindex番目にいる頂点の深さを返す。

### `first`

```cpp
int first(int vertex) const
```

O(1)。頂点vertexがEuler tourに最初に現れる位置を返す。

### `last`

```cpp
int last(int vertex) const
```

O(1)。頂点vertexがEuler tourに最後に現れる位置を返す。

### `index`

```cpp
int index(int vertex) const
```

O(1)。頂点vertexのDFS行きがけ順の位置を返す。

### `subtree`

```cpp
std::pair<int, int> subtree(int vertex) const
```

O(1)。頂点vertexの部分木を表すDFS行きがけ順の半開区間を返す。

### `preorder`

```cpp
int preorder(int index) const
```

O(1)。DFS行きがけ順index番目の頂点を返す。

### `vertices`

```cpp
const std::vector<int>& vertices() const noexcept
```

O(1)。完全Euler tourの頂点列を返す。

### `edges`

```cpp
const std::vector<int>& edges() const noexcept
```

O(1)。完全Euler tourで隣り合う頂点間を移動する辺番号列を返す。

### `depths`

```cpp
const std::vector<int>& depths() const noexcept
```

O(1)。完全Euler tourの各位置での深さを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder Beginner Contest 213 D - Takahashi Tour](https://atcoder.jp/contests/abc213/tasks/abc213_d)。
各頂点の隣接頂点を昇順に追加してから、巡回列をそのまま出力する。

```cpp
eulertour tour(graph, 0);
for (int vertex : tour.vertices()) cout << vertex + 1 << ' ';
```

`verify/atcoder_abc213_d.cpp`に提出用コードを用意しているが、judgeへの提出確認は未実施。
