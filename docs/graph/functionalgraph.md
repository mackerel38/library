---
title: "Functional Graph"
documentation_of: //cp/graph/functionalgraph.hpp
---

# Functional Graph

- Status: experimental
- Header: `cp/graph/functionalgraph.hpp`
- Symbol: `poe::functionalgraph`

## Include

```cpp
#include "graph/functionalgraph.hpp"
```

## できること

各頂点から出る辺がちょうど一つのグラフを、木部分と有向閉路へ分解する。
巨大回数の遷移だけでなく、閉路頂点の列挙、閉路までの距離、到達可能性と最短step数も返す。

```cpp
functionalgraph graph(next);
int destination = graph.jump(start, k);
int entrance = graph.entry(start);
int length = graph.cycle_size(start);
optional<unsigned long long> distance = graph.steps(start, target);
```

`operator[]`は一回遷移した頂点、`cycle_id`は最終的に入る閉路番号、
`distance_to_cycle`は閉路へ入るまでの辺数を返す。`cycle(id)`の頂点列は辺の向き順である。
`steps(from, to)`は到達不能なら`nullopt`を返す。

## 計算量

`unsigned long long`の全bitに対応するdoublingを持つため、構築`O(64n)`、領域`O(64n)`。
`jump`と`steps`は`O(64)`、その他の取得は`O(1)`。

単に遷移先だけが必要で、閉路情報を使わない場合は`structure/doubling.hpp`も選べる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `functionalgraph`

```cpp
struct functionalgraph
```

各頂点の行先が一つのグラフ: functionalgraph fg(next); 構築O(n log U)、jump O(log U)。

### `functionalgraph`

```cpp
explicit functionalgraph(const std::vector<int>& next) : next_(next)
```

O(n log U)。next[v]を辺v->next[v]として、閉路とdoublingを前計算する。U=2^64。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `operator[]`

```cpp
int operator[](int vertex) const
```

O(1)。fg[vertex]でvertexの次の頂点を返す。

### `jump`

```cpp
int jump(int vertex, unsigned long long steps) const
```

O(log steps)。vertexからsteps回進んだ頂点を返す。

### `cycle_id`

```cpp
int cycle_id(int vertex) const
```

O(1)。vertexが最終的に入る閉路番号を返す。

### `distance_to_cycle`

```cpp
int distance_to_cycle(int vertex) const
```

O(1)。vertexから閉路へ入るまでの辺数を返す。閉路上なら0。

### `entry`

```cpp
int entry(int vertex) const
```

O(1)。vertexから最初に到達する閉路上の頂点を返す。

### `cycle_size`

```cpp
int cycle_size(int vertex) const
```

O(1)。vertexが属する行先閉路の長さを返す。

### `cycle`

```cpp
const std::vector<int>& cycle(int id) const
```

O(1)。閉路番号idの頂点列を辺の向き順で返す。

### `cycle_count`

```cpp
int cycle_count() const noexcept
```

O(1)。閉路数を返す。

### `steps`

```cpp
std::optional<unsigned long long> steps(int from, int to) const
```

O(log n)。fromからtoへ到達する最小step数。到達不能ならnullopt。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC167 D - Teleporter](https://atcoder.jp/contests/abc167/tasks/abc167_d):
  `jump(start, k)`で巨大回数遷移を処理できる。
- [AtCoder ABC296 E - Transition Game](https://atcoder.jp/contests/abc296/tasks/abc296_e):
  `cycle(id).size()`の総和が、閉路上にある頂点数となる。
- [AtCoder ABC436 E - Minimum Swap](https://atcoder.jp/contests/abc436/tasks/abc436_e):
  permutationの各cycle内から最初に交換する二頂点を選べるため、cycleごとの二頂点組を数える。

対応するverifyコードを収録した。ABC436 Eは公式サンプル一致。

## 検証

- `tests/api/graph/functionalgraph.cpp`: 閉路・木・到達stepの公開API
- `tests/property/graph/functionalgraph.cpp`: 小さいランダム関数の愚直遷移と比較
