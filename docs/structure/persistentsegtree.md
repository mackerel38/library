---
title: "永続Segment Tree"
documentation_of: //cp/structure/persistentsegtree.hpp
---

# 永続Segment Tree

- Status: experimental
- Header: `cp/structure/persistentsegtree.hpp`
- Symbol: `poe::persistentsegtree<S, op, e>`

## Include

```cpp
#include "structure/persistentsegtree.hpp"
```

## できること

過去versionを変更せず、一点更新後の新しい配列versionを作る。versionは軽い整数なので、
配列全体のコピーや過去状態への巻き戻しはrootの代入だけで`O(1)`になる。

```cpp
persistentsegtree<long long, add, zero> tree(values);
auto first = tree.initial();
auto second = tree.set(first, 2, 10);
long long old_sum = tree.prod(first, 0, 4);
long long new_sum = tree.prod(second, 0, 4);
```

`set`と`apply`は新しいversionを返す。`get`、`prod`、`all_prod`は指定versionを読む。
演算`op`は結合的で、`e()`は単位元であること。更新ごとに`O(log n)`個のnodeを追加する。

## 計算量

構築`O(n)`、versionコピー`O(1)`、一点更新・一点取得・区間積`O(log n)`。
領域は初期`O(n)`、更新`q`回で`O(n+q log n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `persistentsegtree`

```cpp
template <class S, auto op, auto e> struct persistentsegtree
```

永続Segment Tree: persistentsegtree<S,op,e> tree(values); version root=tree.initial()。

### `persistentsegtree`

```cpp
explicit persistentsegtree(int n) : size_(n)
```

O(n)。n個のe()から初期versionを構築する。

### `persistentsegtree`

```cpp
explicit persistentsegtree(const std::vector<S>& values) : size_(static_cast<int>(values.size()))
```

O(n)。valuesから初期versionを構築する。

### `initial`

```cpp
version initial() const noexcept
```

O(1)。構築時のversionを返す。

### `set`

```cpp
version set(version root, int index, const S& value)
```

O(log n)。rootのa[index]をvalueへ置き換えた新しいversionを返す。

### `apply`

```cpp
version apply(version root, int index, const S& value)
```

O(log n)。rootのa[index]をop(a[index],value)へ更新した新versionを返す。

### `get`

```cpp
S get(version root, int index) const
```

O(log n)。rootのa[index]を返す。

### `prod`

```cpp
S prod(version root, int left, int right) const
```

O(log n)。rootの半開区間[left,right)の積を返す。

### `all_prod`

```cpp
S all_prod(version root) const
```

O(1)。rootの全要素の積を返す。空配列ではe()。

### `size`

```cpp
int size() const noexcept
```

O(1)。配列長を返す。

### `node_count`

```cpp
int node_count() const noexcept
```

O(1)。現在確保済みnode数を返す。

### `reserve_nodes`

```cpp
void reserve_nodes(int count)
```

O(1)。今後追加するnode領域を予約する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC453 G - Copy Query](https://atcoder.jp/contests/abc453/tasks/abc453_g)では、
各数列に対応するversionだけを持つ。数列コピーはversion代入、一点代入は`set`、
区間和は`prod`で処理できる。`verify/atcoder_abc453_g.cpp`に使用例を収録した。judge未提出。

## 検証

- `tests/api/structure/persistentsegtree.cpp`: version分岐と非可換積
- `tests/property/structure/persistentsegtree.cpp`: ランダムなversionコピー・更新・区間和を愚直比較
- `verify/atcoder_abc453_g.cpp`: 実在問題用コード
