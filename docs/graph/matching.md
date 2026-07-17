---
title: "Bipartite Matching"
documentation_of: //cp/graph/matching.hpp
---

# Bipartite Matching

  `poe::bipartite_degree_feasible`, `poe::max_bipartite_degree_reward`

## 概要

左右の頂点集合を明示した二部グラフの最大matchingをHopcroft--Karp法で求める。
flow graphへ変換する定型コードは不要で、最小頂点coverもKőnigの定理から同時に復元する。

## 厳密な定義

- `bipartitematchingresult`: 二部最大matchingの結果: result[left], result.right[right], result.size(); 取得O(1)。
- `bipartitematching`: 左右の頂点集合を分けた二部グラフ: bipartitematching graph(L, R); solveはO(m sqrt(L+R))。
- `bipartite_degree_feasible`: O(LR+R log R)。完全二部グラフで右頂点の要求次数を左頂点の次数上限内に割り当てられるか返す。
- `max_bipartite_degree_reward`: O(LR^3)時間・O(LR^2)領域。要求次数を割当て可能な右頂点部分集合の最大報酬を返す。

```cpp
bipartitematching graph(left_size, right_size);
graph.add_edge(left, right);
auto result = graph.solve();
cout << result.size() << '\n';
for (auto [left, right] : result.edges()) cout << left << ' ' << right << '\n';
```

`result[left]`と`result.right[right]`は対応頂点を返し、未matchingなら`-1`。
最小頂点coverは`left_cover`と`right_cover`に分かれて入る。
`independent_size()`は最大独立集合の大きさ、`left_independent()`と`right_independent()`は
その頂点を左右に分けて返す。
多重辺を追加しても正しさは変わらないが、探索の定数倍は増える。

完全二部グラフで、左頂点には次数上限、右頂点には要求次数だけがあり、接続相手は自由な場合は
辺を陽に張らず次数列だけで扱える。

```cpp
bool possible = bipartite_degree_feasible(left_capacity, right_degree);
long long best = max_bipartite_degree_reward(left_capacity, right_degree, reward);
```

前者は全右頂点を割り当てられるか判定する。後者は右頂点を任意に選び、選んだ各頂点の要求次数を
満たせる部分集合の最大報酬を返す。右次数の降順prefixごとにGale--Ryser型条件を課すため、実際の
辺や割当てを列挙する必要がない。接続可能な辺が頂点対ごとに制限される場合はこのAPIを使えない。

## Include

```cpp
#include "graph/matching.hpp"
```

## 計算量

左頂点数`L`、右頂点数`R`、辺数`m`として、構築`O(L+R+m)`、
`solve()`は`O(m sqrt(L+R))`、領域`O(L+R+m)`。
次数列の判定は`O(LR+R log R)`。報酬最大化は`O(LR^3)`時間、`O(LR^2)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `bipartitematchingresult`

```cpp
struct bipartitematchingresult
```

二部最大matchingの結果: result[left], result.right[right], result.size(); 取得O(1)。

### `size`

```cpp
int size() const noexcept
```

O(1)。matchingの辺数を返す。

### `independent_size`

```cpp
int independent_size() const noexcept
```

O(1)。二部グラフの最大独立集合の頂点数を返す。

### `left_independent`

```cpp
std::vector<int> left_independent() const
```

O(L)。最大独立集合に含まれる左頂点を返す。

### `right_independent`

```cpp
std::vector<int> right_independent() const
```

O(R)。最大独立集合に含まれる右頂点を返す。

### `operator[]`

```cpp
int operator[](int vertex) const
```

O(1)。左頂点vertexと対応する右頂点。未matchingなら-1。

### `edges`

```cpp
std::vector<std::pair<int, int>> edges() const
```

O(L)。matchingに使う(left, right)の組を返す。

### `bipartitematching`

```cpp
struct bipartitematching
```

左右の頂点集合を分けた二部グラフ: bipartitematching graph(L, R); solveはO(m sqrt(L+R))。

### `bipartitematching`

```cpp
bipartitematching(int left_size, int right_size) : left_size_(left_size), right_size_(right_size), adjacency_(left_size)
```

O(L+R)。辺のない二部グラフを作る。

### `add_edge`

```cpp
int add_edge(int left, int right)
```

ならしO(1)。左頂点leftと右頂点rightを結ぶ辺を追加し、辺番号を返す。

### `left_size`

```cpp
int left_size() const noexcept
```

O(1)。左頂点数を返す。

### `right_size`

```cpp
int right_size() const noexcept
```

O(1)。右頂点数を返す。

### `edge_count`

```cpp
int edge_count() const noexcept
```

O(1)。追加した辺数を返す。

### `operator[]`

```cpp
const std::vector<int>& operator[](int left) const
```

O(1)。左頂点から出る右頂点一覧を返す。

### `solve`

```cpp
bipartitematchingresult solve() const
```

O(m sqrt(L+R))。最大matchingと同じ大きさの最小頂点coverを返す。

### `bipartite_degree_feasible`

```cpp
inline bool bipartite_degree_feasible( const std::vector<int>& left_capacity, std::vector<int> right_degree )
```

O(LR+R log R)。完全二部グラフで右頂点の要求次数を左頂点の次数上限内に割り当てられるか返す。

### `max_bipartite_degree_reward`

```cpp
template<class T> T max_bipartite_degree_reward( const std::vector<int>& left_capacity, const std::vector<int>& right_degree, const std::vector<T>& reward )
```

O(LR^3)時間・O(LR^2)領域。要求次数を割当て可能な右頂点部分集合の最大報酬を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC091 C - 2D Plane 2N Points](https://atcoder.jp/contests/abc091/tasks/arc092_a)では、
条件を満たす赤点と青点の間に辺を張り、最大matchingの大きさを答える。
`verify/atcoder_abc091_c.cpp`を収録した。

[AtCoder ABC461 G - Graph Problem 2026](https://atcoder.jp/contests/abc461/tasks/abc461_g)では、
元の各頂点を左右に複製した二部グラフの最大独立集合サイズを使う。
`verify/atcoder_abc461_g.cpp`で公式sampleを確認済み。

[AtCoder ABC424 G - Set list](https://atcoder.jp/contests/abc424/tasks/abc424_g)では、
アイドルを左頂点、選択候補の曲を右頂点として`max_bipartite_degree_reward`を使う。
`verify/atcoder_abc424_g.cpp`で公式sampleを確認済み。

## 検証

- `tests/api/graph/matching.cpp`: matching・最小頂点cover・添字アクセス
- `tests/property/graph/matching.cpp`: 小さい二部グラフの部分集合DPとの比較とcover検証
- `tests/api/graph/matchingdegree.cpp`: 次数列判定と報酬最大化の基本例
- `tests/property/graph/matchingdegree.cpp`: 小さい割当て・右頂点部分集合の全列挙との比較
