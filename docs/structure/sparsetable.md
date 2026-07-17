---
title: "Sparse Table"
documentation_of: //cp/structure/sparsetable.hpp
---

# Sparse Table

- Status: experimental
- Header: `cp/structure/sparsetable.hpp`
- Symbol: `poe::sparsetable<S, op>`, `poe::disjointsparsetable<S, op>`

## Include

```cpp
#include "structure/sparsetable.hpp"
```

## 選び方

更新のない列の非空区間積を`O(1)`で得る。

- `sparsetable`: `min`、`max`、`gcd`のように、結合的かつ冪等な演算。構造と定数倍が軽い。
- `disjointsparsetable`: 文字列連結、行列積、アフィン写像合成など、結合的な一般の演算。

```cpp
int minimum(int a, int b) { return min(a, b); }
sparsetable<int, minimum> rmq(values);
int answer = rmq.prod(left, right);
```

両方とも`operator[]`で元の要素を取得できる。単位元を引数に取らないため、
`prod(left, right)`は`left < right`の非空区間だけを受け付ける。

## 計算量

構築`O(n log n)`、区間積`O(1)`、領域`O(n log n)`。

## 頻出事故

- 和や積を通常の`sparsetable`へ渡してはいけない。同じ要素を重ねて使うため、冪等性が必要。
- 非可換演算でも、`op(left, right)`が順序どおり結合するなら`disjointsparsetable`で扱える。
- 更新がある場合は`segtree`を選ぶ。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sparsetable`

```cpp
template <class S, auto op> struct sparsetable
```

冪等演算の静的区間積: sparsetable<int, op> table(values); 構築O(n log n)、取得O(1)。

### `sparsetable`

```cpp
explicit sparsetable(const std::vector<S>& values) : values_(values)
```

O(n log n)。valuesから構築する。opは結合的かつ冪等であること。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `operator[]`

```cpp
const S& operator[](int index) const
```

O(1)。table[index]で元の列の要素を返す。

### `prod`

```cpp
S prod(int left, int right) const
```

O(1)。table.prod(left, right)で非空な[left, right)の積を返す。

### `disjointsparsetable`

```cpp
template <class S, auto op> struct disjointsparsetable
```

一般モノイドの静的区間積: disjointsparsetable<S, op> table(values); 構築O(n log n)、取得O(1)。

### `disjointsparsetable`

```cpp
explicit disjointsparsetable(const std::vector<S>& values) : values_(values)
```

O(n log n)。valuesから構築する。opは結合的であること。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `operator[]`

```cpp
const S& operator[](int index) const
```

O(1)。table[index]で元の列の要素を返す。

### `prod`

```cpp
S prod(int left, int right) const
```

O(1)。table.prod(left, right)で非空な[left, right)の積を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [Library Checker - Static RMQ](https://judge.yosupo.jp/problem/staticrmq):
  `sparsetable`のverifyコードを収録。judge未提出。
- [Library Checker - Static Range Composite](https://judge.yosupo.jp/problem/static_range_composite):
  `disjointsparsetable`を使える代表問題。verifyコードは未作成。

## 検証

- `tests/api/structure/sparsetable.cpp`: 両構造の公開API
- `tests/property/structure/sparsetable.cpp`: `min`と非可換な文字列連結を愚直計算と比較
- `verify/librarychecker_staticrmq.cpp`: 公開問題用コード。judge未提出
