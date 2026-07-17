---
title: "`segtree`"
documentation_of: //cp/structure/segtree.hpp
---

# `segtree`

## Include

```cpp
#include "structure/segtree.hpp"
```

`segtree<S, op, e>`としてモノイドの一点代入と区間積を扱う。
構築`O(n)`、`set/get/prod`は`O(log n)`、`all_prod`は`O(1)`。
`max_right/min_left`も`O(log n)`で、述語は`g(e()) == true`かつ副作用なしとする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `segtree`

```cpp
template <class S, auto op, auto e> struct segtree
```

モノイドの区間積を管理する: segtree<S, op, e> seg(values); 各操作O(log n)。

### `segtree`

```cpp
explicit segtree(int n) : segtree(std::vector<S>(n, e()))
```

O(n)。n要素をe()で初期化する。

### `segtree`

```cpp
explicit segtree(const std::vector<S>& values) : n_(static_cast<int>(values.size()))
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
S get(int index) const
```

O(1)。a[index]を返す。

### `operator[]`

```cpp
S operator[](int index) const
```

O(1)。seg[index]はa[index]を返す。

### `prod`

```cpp
S prod(int left, int right) const
```

O(log n)。半開区間[left, right)の積を返す。

### `all_prod`

```cpp
S all_prod() const
```

O(1)。全要素の積を返す。

### `max_right`

```cpp
template <class G> int max_right(int left, G g) const
```

O(log n)。g(prod(left, right))がtrueとなる最大のrightを返す。g(e())はtrue。

### `min_left`

```cpp
template <class G> int min_left(int right, G g) const
```

O(log n)。g(prod(left, right))がtrueとなる最小のleftを返す。g(e())はtrue。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Point Set Range Composite](https://judge.yosupo.jp/problem/point_set_range_composite)。

```cpp
poe::segtree<affine, compose, identity> segment(functions);
if (type == 0) segment.set(position, affine{a, b});
else {
    affine function = segment.prod(left, right);
    std::cout << function.a * x + function.b << '\n';
}
```

`verify/segtree.test.cpp`で2026-07-15に全ケースAC確認済み。
