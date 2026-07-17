---
title: "キー重複を除く上位 K 件"
documentation_of: //cp/structure/topk.hpp
---

# キー重複を除く上位 K 件

- Header: `cp/structure/topk.hpp`
- Symbol: `poe::topkdistinct`, `poe::merge_topkdistinct`, `poe::empty_topkdistinct`, `poe::make_topkdistinct`
- Status: experimental

## どんな問題に使えるか

候補を値の降順に最大`K`件だけ残し、同じキーからは最大値の一件だけを採用する。
`K`が小さいときのprefix/suffix集約、Segment Tree、SWAG、木DPで使える。
保持領域は`O(K)`、一件追加とmergeは`O(K^2)`。

```cpp
#include "structure/segtree.hpp"
#include "structure/topk.hpp"

using state = topkdistinct<long long, int, 4>;
using tree = segtree<state,
    merge_topkdistinct<long long, int, 4>,
    empty_topkdistinct<long long, int, 4>>;

state one = make_topkdistinct<long long, int, 4>(value, key);
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `topkdistinct`

```cpp
template <class Value, class Key, std::size_t K> struct topkdistinct
```

キーが相異なる値上位K件を保持する固定長monoid。merge O(K^2)。

### `add`

```cpp
void add(Value value, Key key)
```

O(K^2)。同じkeyは大きいvalueだけ残して候補を追加する。

### `size`

```cpp
int size() const noexcept
```

O(1)。有効な候補数を返す。

### `operator[]`

```cpp
const entry& operator[](int index) const
```

O(1)。value降順のindex番目を返す。

### `merge_topkdistinct`

```cpp
template <class Value, class Key, std::size_t K> topkdistinct<Value, Key, K> merge_topkdistinct( topkdistinct<Value, Key, K> first, const topkdistinct<Value, Key, K>& second )
```

O(K^2)。二集合からキーが相異なる値上位K件を返す。

### `empty_topkdistinct`

```cpp
template <class Value, class Key, std::size_t K> topkdistinct<Value, Key, K> empty_topkdistinct()
```

O(1)。topkdistinct monoidの単位元を返す。

### `make_topkdistinct`

```cpp
template <class Value, class Key, std::size_t K> topkdistinct<Value, Key, K> make_topkdistinct(Value value, Key key)
```

O(K^2)。候補一件からtopkdistinctを作る。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC447 G - Div. 1 & Div. 2](https://atcoder.jp/contests/abc447/tasks/abc447_g)では、
区間ごとにジャンルが異なる面白さ上位4件をSegment Treeへ載せる部分に対応する。
`verify/atcoder_abc447_g.cpp`に使用例を収録した。公式sample確認済み。
