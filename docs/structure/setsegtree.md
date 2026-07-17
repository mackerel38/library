---
title: "区間集合更新Segment Tree Beats"
documentation_of: //cp/structure/setsegtree.hpp
---

# 区間集合更新Segment Tree Beats

- Header: `cp/structure/setsegtree.hpp`
- Symbol: `poe::setsegtree`, `poe::setmaxresult`
- Status: experimental

## どんな問題に使えるか

各位置に64要素以下の集合を持ち、区間内の全集合への要素追加・削除と、区間内の最大要素数・
その達成位置数を処理する。集合は`uint64_t`のbit maskで表す。

```cpp
#include "structure/setsegtree.hpp"

poe::setsegtree tree(n);
tree.add(l, r, element);
tree.erase(l, r, element);
auto [maximum, count] = tree.max(l, r);
```

nodeに区間内集合のintersection・union・最大要素数・達成数を持つ。更新対象bitが区間内で
全位置同じ状態ならnodeだけで更新し、混在時だけ子へ降りるSegment Tree Beatsである。

`max`, `get`, `operator[]`は`O(log n)`。更新列全体は、bit数を`B<=64`として
`O((n+q)log n+nB)`時間に償却される。単独更新の最悪計算量は線形になりうる。
`add_mask`, `erase_mask`では複数bitをまとめて変更でき、`apply`は
`mask = (mask & keep_mask) | add_mask`を区間へ適用する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `setmaxresult`

```cpp
struct setmaxresult
```

auto [size,count]=tree.max(l,r); 区間内集合の最大要素数と達成位置数。

### `setsegtree`

```cpp
struct setsegtree
```

setsegtree tree(n); tree.add(l,r,x); tree.erase(l,r,x); 0<=x<64。

### `setsegtree`

```cpp
explicit setsegtree(int n) : setsegtree(std::vector<std::uint64_t>(n))
```

O(n)。n個の空集合を作る。n>=1。

### `setsegtree`

```cpp
explicit setsegtree(const std::vector<std::uint64_t>& masks) : n_(static_cast<int>(masks.size())), nodes_(4 * std::max(1, n_)), lazy_(nodes_.size())
```

O(n)。各位置の初期集合をbit maskで与える。

### `size`

```cpp
int size() const noexcept
```

O(1)。集合数を返す。

### `add`

```cpp
void add(int left, int right, int element)
```

全更新で償却O(log n)。[left,right)の全集合へelementを追加する。

### `erase`

```cpp
void erase(int left, int right, int element)
```

全更新で償却O(log n)。[left,right)の全集合からelementを削除する。

### `add_mask`

```cpp
void add_mask(int left, int right, std::uint64_t mask)
```

全更新で償却O(log n)。[left,right)の全集合へmaskの要素を追加する。

### `erase_mask`

```cpp
void erase_mask(int left, int right, std::uint64_t mask)
```

全更新で償却O(log n)。[left,right)の全集合からmaskの要素を削除する。

### `apply`

```cpp
void apply(int left, int right, std::uint64_t keep_mask, std::uint64_t add_mask)
```

全更新で償却O(log n)。各集合maskを(mask&keep_mask)|add_maskへ変更する。

### `max`

```cpp
setmaxresult max(int left, int right)
```

O(log n)。[left,right)の最大要素数と達成位置数を返す。left<right。

### `get`

```cpp
std::uint64_t get(int position)
```

O(log n)。positionの集合maskを返す。

### `operator[]`

```cpp
std::uint64_t operator[](int position)
```

O(log n)。tree[position]で集合maskを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC430 G - Range Set Modifying Query](https://atcoder.jp/contests/abc430/tasks/abc430_g)へ直接使う。
問題の要素`x`を0-indexed bitへ直し、`verify/atcoder_abc430_g.cpp`に収録した。
公式sample確認済み、judge ACは未確認。

## 検証

- `tests/property/structure/setsegtree.cpp`: ランダム区間更新・queryを集合配列の愚直解と比較
- `tests/api/structure/setsegtree.cpp`: add・erase・添え字アクセス
