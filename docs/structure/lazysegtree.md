---
title: "lazysegtree"
documentation_of: //cp/structure/lazysegtree.hpp
---

# `lazysegtree`

## Include

```cpp
#include "structure/lazysegtree.hpp"
```

`lazysegtree<S, op, e, F, mapping, composition, id>`としてモノイド作用を扱う。
構築`O(n)`、一点・区間`apply`、`set/get/prod/max_right/min_left`は`O(log n)`。
`composition(newer, older)`はolderの後にnewerを適用する作用を返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lazysegtree`

```cpp
template <class S, auto op, auto e, class F, auto mapping, auto composition, auto id> struct lazysegtree
```

区間作用と区間積を扱う: lazysegtree<S, op, e, F, mapping, composition, id> seg(v)。

### `lazysegtree`

```cpp
explicit lazysegtree(int n) : lazysegtree(std::vector<S>(n, e()))
```

O(n)。n要素をe()で初期化する。

### `lazysegtree`

```cpp
explicit lazysegtree(const std::vector<S>& values) : n_(static_cast<int>(values.size()))
```

O(n)。valuesから構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `set`

```cpp
void set(int index, const S& value)
```

O(log n)。a[index]をvalueへ置き換える。

### `get`

```cpp
S get(int index)
```

O(log n)。a[index]を返す。

### `operator[]`

```cpp
S operator[](int index)
```

O(log n)。seg[index]はa[index]を返す。

### `prod`

```cpp
S prod(int left, int right)
```

O(log n)。半開区間[left, right)の積を返す。

### `all_prod`

```cpp
S all_prod() const
```

O(1)。全要素の積を返す。

### `apply`

```cpp
void apply(int index, const F& f)
```

O(log n)。a[index]へ作用fを適用する。

### `apply`

```cpp
void apply(int left, int right, const F& f)
```

O(log n)。半開区間[left, right)へ作用fを適用する。

### `max_right`

```cpp
template <class G> int max_right(int left, G g)
```

O(log n)。g(prod(left, right))がtrueとなる最大のrightを返す。g(e())はtrue。

### `min_left`

```cpp
template <class G> int min_left(int right, G g)
```

O(log n)。g(prod(left, right))がtrueとなる最小のleftを返す。g(e())はtrue。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum)。

```cpp
segment.apply(left, right, affine{multiply, add});
std::cout << segment.prod(left, right).sum.val() << '\n';
```

状態型・作用型の完全な定義は`verify/lazysegtree.test.cpp`にあり、
2026-07-15に全ケースAC確認済み。
