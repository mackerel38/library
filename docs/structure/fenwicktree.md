---
title: "Fenwick Tree"
documentation_of: //cp/structure/fenwicktree.hpp
---

# Fenwick Tree

- Status: experimental
- Header: `cp/structure/fenwicktree.hpp`
- Symbol: `poe::fenwicktree<T>`

## Include

```cpp
#include "structure/fenwicktree.hpp"
```

## できること

長さ `n` の列に対する一点加算、代入、要素取得、区間和を`O(log n)`で処理する。
累積和に対する二分探索も`O(log n)`で行える。
外から渡す添字は 0-index とし、区間は半開区間 `[left, right)` で表す。

```cpp
poe::fenwicktree<long long> sums(5);
sums.add(2, 7);
sums.set(4, -3);

long long value = sums[2];         // 7
long long prefix = sums.sum(3);    // [0, 3) の和。7
long long range = sums.sum(2, 5);  // [2, 5) の和。4
```

## API

### `explicit fenwicktree(int size)`

長さ `size`、全要素が `T{}` の列を構築する。`size >= 0` が必要。

### `int size() const`

構築時に指定した列長を返す。

### `void add(int index, const T& delta)`

`a[index] += delta` を行う。`0 <= index < size()` が必要。

### `void set(int index, const T& value)`

`a[index] = value` を行う。`0 <= index < size()` が必要。

### `T sum(int right) const`

`a[0] + ... + a[right - 1]` を返す。`0 <= right <= size()` が必要。
`right == 0` なら `T{}` を返す。

### `T sum(int left, int right) const`

半開区間 `[left, right)` の和を返す。
`0 <= left <= right <= size()` が必要。空区間なら `T{}` を返す。

### `T operator[](int index) const`

`a[index]`を返す。`0 <= index < size()`が必要。
内部表現は一点の値を直接保持しないため、返り値は参照ではなく値である。

### `int lower_bound(const T& value) const`

`sum(index + 1) >= value`となる最小の`index`を返す。存在しなければ`size()`を返す。
全要素が非負であることが必要。`value <= T{}`なら`0`を返す。

### `int upper_bound(const T& value) const`

`sum(index + 1) > value`となる最小の`index`を返す。存在しなければ`size()`を返す。
全要素が非負であることが必要。`value < T{}`なら`0`を返す。

## 型と前提

- `T{}` が加算の単位元であること。
- `T` がコピー可能で、`operator+=`、`operator-`、`operator<`を持つこと。
- API の添字条件違反は `assert` で検出する。
- 初期列を渡すコンストラクタは`O(n)`で構築する。
- `lower_bound`と`upper_bound`の使用中は、全要素が非負でなければならない。

## 計算量

| 操作 | 時間 | 追加領域 |
| --- | --- | --- |
| 構築 | `O(n)` | `O(n)` |
| `add` | `O(log n)` | `O(1)` |
| `set` | `O(log n)` | `O(1)` |
| `sum(right)` | `O(log n)` | `O(1)` |
| `sum(left, right)` | `O(log n)` | `O(1)` |
| `operator[]` | `O(log n)` | `O(1)` |
| `lower_bound` / `upper_bound` | `O(log n)` | `O(1)` |

## 頻出事故

- `sum(right)`は`right`を含まず、`[0, right)`を返す。
- `sum(left, right)`も`right`を含まない。
- `add(size(), delta)`は範囲外。
- `operator[]`は参照を返さないため、`sums[index] = value`とは書けない。代入には`set`を使う。
- `unsigned`型では、負の更新や`sum(right) - sum(left)`の結果が
  数学的な整数和と一致しない場合がある。
- 負の要素がある状態で`lower_bound`または`upper_bound`を使うと正しい結果にならない。

## 検証

- `tests/api/structure/fenwicktree.cpp`: 公開APIと境界値のsmoke test
- `tests/property/structure/fenwicktree.cpp`: 単純な配列との固定seedランダム差分
- `verify/fenwicktree.test.cpp`: Library Checker `Point Add Range Sum`
- 2026-07-15に`verify/fenwicktree.test.cpp`の全ケースAC確認済み。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `fenwicktree`

```cpp
template <class T> struct fenwicktree
```

一点加算と区間和を扱う配列: fenwicktree<long long> fw(n); 構築O(n)、更新・取得O(log n)。

### `fenwicktree`

```cpp
explicit fenwicktree(int size) : size_(size)
```

O(n)。fenwicktree<T> fw(size): T{}で初期化されたsize要素を作る。

### `fenwicktree`

```cpp
explicit fenwicktree(const std::vector<T>& values) : size_(static_cast<int>(values.size())), data_(values.size() + 1)
```

O(n)。fenwicktree<T> fw(values): valuesから構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。fw.size(): 配列長を返す。

### `add`

```cpp
void add(int index, const T& delta)
```

O(log n)。fw.add(i, delta): a[i] += deltaを行う。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。fw.set(i, value): a[i] = valueを行う。

### `sum`

```cpp
T sum(int right) const
```

O(log n)。fw.sum(right): [0, right)の和を返す。

### `sum`

```cpp
T sum(int left, int right) const
```

O(log n)。fw.sum(left, right): [left, right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(log n)。fw.sum(): 全要素の和を返す。

### `operator[]`

```cpp
T operator[](int index) const
```

O(log n)。fw[index]: a[index]を返す。

### `lower_bound`

```cpp
int lower_bound(const T& value) const
```

O(log n)。fw.lower_bound(x): fw.sum(i + 1) >= xとなる最初のi。なければfw.size()。
全要素が非負であること。

### `upper_bound`

```cpp
int upper_bound(const T& value) const
```

O(log n)。fw.upper_bound(x): fw.sum(i + 1) > xとなる最初のi。なければfw.size()。
全要素が非負であること。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Point Add Range Sum](https://judge.yosupo.jp/problem/point_add_range_sum)。

```cpp
poe::fenwicktree<long long> sums(values);
if (type == 0) sums.add(position, delta);
else std::cout << sums.sum(left, right) << '\n';
```
