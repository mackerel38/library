---
title: "Wavelet Matrix"
documentation_of: //cp/structure/waveletmatrix.hpp
---

# Wavelet Matrix

- Status: experimental
- Header: `cp/structure/waveletmatrix.hpp`
- Symbol: `poe::waveletmatrix<T, Sum>`

## Include

```cpp
#include "structure/waveletmatrix.hpp"
```

## できること

更新のない数値列について、位置区間と値域を同時に絞るqueryを`O(log sigma)`で処理する。
値は内部で座標圧縮するため、負数や大きな値もそのまま渡せる。

```cpp
waveletmatrix<int, long long> wm(values);
int third = wm.kth(left, right, 2);
int frequency = wm.count(left, right, value);
long long total = wm.sum_leq(left, right, upper);
```

## API

- `kth(left, right, k)`: 0-indexedで`k`番目に小さい値。区間は非空。
- `count_less` / `count_leq`: 上限未満／以下の個数。
- `count(left, right, value)`: 指定値の個数。
- `count(left, right, lower, upper)`: 値域`[lower, upper)`の個数。
- `sum(left, right)`: 区間全体の和。
- `sum_less` / `sum_leq`: 上限未満／以下の値だけの和。
- `sum(left, right, lower, upper)`: 値域`[lower, upper)`に入る値の和。
- `prev(left, right, upper)`: `upper`未満の最大値。
- `next(left, right, lower)`: `lower`以上の最小値。
- `operator[]`: 元の列の要素。

`prev`と`next`は該当値がなければ`nullopt`を返す。
`Sum`には`T`からの変換、加減算、零初期化が必要で、通常は`long long`を使う。

## 計算量

異なる値の種類数を`sigma`とする。構築`O(n log sigma + sigma log sigma)`、
各値域query`O(log sigma)`、`operator[]`と全区間和は`O(1)`、領域`O(n log sigma)`。

更新がある場合はこの型では扱えない。区間和だけなら`prefixsum`、一点更新がある一般区間積なら
`segtree`の方が軽い。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `waveletmatrix`

```cpp
template <class T, class Sum = long long> struct waveletmatrix
```

静的列の値域query: waveletmatrix<int, long long> wm(values); 構築O(n log sigma)、取得O(log sigma)。

### `waveletmatrix`

```cpp
explicit waveletmatrix(const std::vector<T>& values) : values_(values)
```

O(n log sigma)。値を座標圧縮して構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `operator[]`

```cpp
const T& operator[](int index) const
```

O(1)。wm[index]で元の列の要素を返す。

### `kth`

```cpp
T kth(int left, int right, int k) const
```

O(log sigma)。[left,right)で0-indexedのk番目に小さい値を返す。

### `count_less`

```cpp
int count_less(int left, int right, const T& upper) const
```

O(log sigma)。[left,right)にあるupper未満の要素数を返す。

### `count_leq`

```cpp
int count_leq(int left, int right, const T& upper) const
```

O(log sigma)。[left,right)にあるupper以下の要素数を返す。

### `count`

```cpp
int count(int left, int right, const T& value) const
```

O(log sigma)。[left,right)にあるvalueの個数を返す。

### `count`

```cpp
int count(int left, int right, const T& lower, const T& upper) const
```

O(log sigma)。[left,right)にある[lower,upper)の要素数を返す。

### `sum`

```cpp
Sum sum(int left, int right) const
```

O(1)。[left,right)の全要素の和を返す。

### `sum_less`

```cpp
Sum sum_less(int left, int right, const T& upper) const
```

O(log sigma)。[left,right)にあるupper未満の要素の和を返す。

### `sum_leq`

```cpp
Sum sum_leq(int left, int right, const T& upper) const
```

O(log sigma)。[left,right)にあるupper以下の要素の和を返す。

### `sum`

```cpp
Sum sum(int left, int right, const T& lower, const T& upper) const
```

O(log sigma)。[left,right)にある[lower,upper)の要素の和を返す。

### `prev`

```cpp
std::optional<T> prev(int left, int right, const T& upper) const
```

O(log sigma)。[left,right)でupper未満の最大値。なければnullopt。

### `next`

```cpp
std::optional<T> next(int left, int right, const T& lower) const
```

O(log sigma)。[left,right)でlower以上の最小値。なければnullopt。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC248 D - Range Count Query](https://atcoder.jp/contests/abc248/tasks/abc248_d):
  `count(left, right, value)`を使うverifyコードを収録。
- [AtCoder ABC339 G - Smaller Sum](https://atcoder.jp/contests/abc339/tasks/abc339_g):
  online XORで復号したqueryに`sum_leq(left, right, upper)`を使うverifyコードを収録。
- [AtCoder ABC436 F - Starry Landscape Photo](https://atcoder.jp/contests/abc436/tasks/abc436_f):
  各位置の左右にある自身未満の値の個数を`count_less`で数える。



## 検証

- `tests/api/structure/waveletmatrix.cpp`: 全公開APIと空区間
- `tests/property/structure/waveletmatrix.cpp`: 符号付きランダム列をsort・愚直走査と比較
- `verify/atcoder_abc248_d.cpp`, `verify/atcoder_abc339_g.cpp`: 公開問題用コード
