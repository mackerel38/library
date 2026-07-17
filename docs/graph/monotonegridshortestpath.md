---
title: "列単調grid最短路"
documentation_of: //cp/graph/monotonegridshortestpath.hpp
---

# 列単調grid最短路


## 概要

高さが小さく固定されたgridで、壁の一点更新を行いながら、列番号を減らさずに進む経路の
最短距離を求める。各列を「進入した行から退出する行への縦移動回数」のmin-plus行列にし、
Segment Tree上で合成する。

## 厳密な定義

- `monotonegridshortestpath`: 固定高さgridを列方向に逆戻りしない最短路: monotonegridshortestpath<H> path(cells)。

## Include

```cpp
#include "graph/monotonegridshortestpath.hpp"

monotonegridshortestpath<3> path(cells);
path.toggle(row, column);
int answer = path.distance(0, 0, 2, n - 1);
```

`cells`は行ごとの文字列で、既定では`#`だけを壁とみなす。`vector<array<bool, Height>>`を渡せば、
列ごとに通行可能マスを直接指定できる。`set`、`toggle`、`get`、`operator[]`で状態を操作できる。
`distance`は始点列以下の終点列に対して使い、到達不能なら`-1`を返す。

この構造が扱うのは「列を左へ戻らない経路」である。一般のgrid最短路では、障害物を避けるための
左移動が真に必要な場合があるため、そのまま置き換えてはいけない。問題の構造から単調な最短路の
存在を示せる場合や、最初から右・上下移動だけが許される場合に使う。

## 計算量

幅を`w`とする。構築は`O(Height^3 w)`時間、`O(Height^2 w)`領域。
一点更新と距離queryは`O(Height^3 log w)`。
`Height`を小さなコンパイル時定数として使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `monotonegridshortestpath`

```cpp
template <int Height> struct monotonegridshortestpath
```

固定高さgridを列方向に逆戻りしない最短路: monotonegridshortestpath<H> path(cells)。

### `monotonegridshortestpath`

```cpp
explicit monotonegridshortestpath(const std::vector<column_type>& columns) : columns_(columns), tree_(make_matrices(columns))
```

O(Height^3 * width)。trueを通行可能として列列から構築する。

### `monotonegridshortestpath`

```cpp
explicit monotonegridshortestpath( const std::vector<std::string>& cells, char wall = '#' ) : monotonegridshortestpath(make_columns(cells, wall))
```

O(Height^3 * width)。'.'を通行可能、wallを壁として行文字列から構築する。

### `width`

```cpp
int width() const noexcept
```

O(1)。gridの幅を返す。

### `get`

```cpp
bool get(int row, int column) const
```

O(1)。(row,column)が通行可能ならtrueを返す。

### `operator[]`

```cpp
const column_type& operator[](int column) const
```

O(1)。path[column]はその列の通行可否を返す。

### `set`

```cpp
void set(int row, int column, bool passable)
```

O(Height^3 log width)。(row,column)の通行可否をpassableへ変更する。

### `toggle`

```cpp
void toggle(int row, int column)
```

O(Height^3 log width)。(row,column)の通行可否を反転する。

### `distance`

```cpp
int distance(int from_row, int from_column, int to_row, int to_column) const
```

O(Height^3 log width)。列を左へ戻らない最短距離を返し、到達不能なら-1。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC429 F - Shortest Path Query](https://atcoder.jp/contests/abc429/tasks/abc429_f):
  3行gridの壁を反転し、左上から右下への最短距離を各更新後に求める。

`verify/atcoder_abc429_f.cpp`に提出用コードを収録し、公式sample 2件との一致を確認済み。


`tests/property/graph/monotonegridshortestpath.cpp`では、ランダムな壁更新後の全始終点を
右・上下だけの愚直BFSと比較する。
