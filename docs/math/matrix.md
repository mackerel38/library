---
title: "行列"
documentation_of: //cp/math/matrix.hpp
---

# 行列

- Header: `cp/math/matrix.hpp`
- Symbol: `poe::matrix<T>`
- Status: experimental

## どんな問題に使えるか

状態ベクトルが毎回同じ線形変換を受けるDPに使う。遷移回数が`10^9`以上でも、正方行列を
二分累乗すれば`O(k^3 log n)`で進められる。グラフ上の長さ固定walk数、有限オートマトン、
小さい状態数の線形DPが代表例である。

状態数が大きい、遷移が疎、時刻ごとに遷移が変わる場合は、そのままの行列積は向かない。
線形漸化式の一項だけなら`linear_recurrence`の方が`O(k^2 log n)`で軽い。

## API・計算量

```cpp
#include "math/matrix.hpp"

poe::matrix<mint> transition({{1, 1}, {1, 0}});
mint fibonacci_n = transition.pow(n)[0][1];
```

- 行列積: `O(hwk)`時間。
- `pow(exponent)`: `O(k^3 log exponent)`時間、`O(k^2)`領域。
- `T{}`と`T{1}`が加法・乗法単位元である半環でも使える。

通常の演算子と異なる半環では、演算と単位元を明示する。

```cpp
auto powered = semiring_matrix_power(base, exponent, zero, one, add, multiply);

// 固定長walkの最小費用では短いmin-plus版を使う。
auto cost = minplus_matrix_power(matrix<long long>(values), steps);
long long return_cost = cost[start][start];
```

- `semiring_matrix_product`: `O(hwk)`。任意の半環演算を指定する。
- `semiring_matrix_power`: `O(k^3 log exponent)`。`zero`と`one`も明示する。
- `minplus_matrix_product`, `minplus_matrix_power`: 加算を最小値、乗算を飽和加算として扱う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `matrix`

```cpp
template <class T> struct matrix
```

加算・乗算を持つ型の行列: matrix<mint> a(h, w); 乗算O(hwk)。

### `matrix`

```cpp
matrix(int height, int width) : height_(height), width_(width)
```

O(hw)。h行w列をT{}で初期化する。

### `matrix`

```cpp
explicit matrix(std::vector<std::vector<T>> values) : height_(static_cast<int>(values.size())), width_(values.empty() ? 0 : static_cast<int>(values[0].size())), data_(std::move(values))
```

O(hw)。二次元vectorから構築する。

### `identity`

```cpp
static matrix identity(int size)
```

O(n^2)。n次単位行列を返す。

### `height`

```cpp
int height() const noexcept
```

O(1)。行数を返す。

### `width`

```cpp
int width() const noexcept
```

O(1)。列数を返す。

### `operator[]`

```cpp
std::vector<T>& operator[](int row)
```

O(1)。row行への参照を返す。

### `operator[]`

```cpp
const std::vector<T>& operator[](int row) const
```

O(1)。row行へのconst参照を返す。

### `operator*`

```cpp
friend matrix operator*(const matrix& left, const matrix& right)
```

O(hwk)。行列積を返す。

### `pow`

```cpp
matrix pow(long long exponent) const
```

O(n^3 log exponent)。正方行列の非負整数乗を返す。

### `semiring_matrix_product`

```cpp
template <class T, class Add, class Multiply> matrix<T> semiring_matrix_product(const matrix<T>& left, const matrix<T>& right, const T& zero, Add add, Multiply multiply)
```

O(hwk)。zero・add・multiplyで定めた半環上の行列積を返す。

### `semiring_matrix_power`

```cpp
template <class T, class Add, class Multiply> matrix<T> semiring_matrix_power(matrix<T> base, unsigned long long exponent, const T& zero, const T& one, Add add, Multiply multiply)
```

O(n^3 log exponent)。zero・one・演算を指定した半環上の正方行列累乗を返す。

### `minplus_matrix_product`

```cpp
template <class T> matrix<T> minplus_matrix_product( const matrix<T>& left, const matrix<T>& right, T infinity = std::numeric_limits<T>::max() / 4 )
```

O(hwk)。min-plus半環で行列積を返す。infinity以上は到達不能として扱う。

### `minplus_matrix_power`

```cpp
template <class T> matrix<T> minplus_matrix_power( matrix<T> base, unsigned long long exponent, T infinity = std::numeric_limits<T>::max() / 4 )
```

O(n^3 log exponent)。ちょうどexponent辺の最小費用を表すmin-plus行列累乗を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[EDPC R - Walk](https://atcoder.jp/contests/dp/tasks/dp_r)と
[TDPC M - 家](https://atcoder.jp/contests/tdpc/tasks/tdpc_house)の反復遷移に使える。
`verify/atcoder_dp_r.cpp`と`verify/atcoder_tdpc_house.cpp`で2026-07-15にjudge AC確認済み。
Fibonacci行列のAPI testも実施済み。

[AtCoder ABC445 F - Exactly K Steps 2](https://atcoder.jp/contests/abc445/tasks/abc445_f)では、
`minplus_matrix_power`で全始点のちょうど`K`辺closed walk最小費用を求める。
`verify/atcoder_abc445_f.cpp`で公式sampleを確認済み。judge ACは未確認。
