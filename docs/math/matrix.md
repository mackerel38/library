---
title: "行列"
documentation_of: //cp/math/matrix.hpp
---

# 行列


## 概要

状態ベクトルが毎回同じ線形変換を受けるDPに使う。遷移回数が`10^9`以上でも、正方行列を
二分累乗すれば`O(k^3 log n)`で進められる。グラフ上の長さ固定walk数、有限オートマトン、
小さい状態数の線形DPが代表例である。
通常は同じ遷移を繰り返し、少数の時刻だけ別の処理をするDPも、通常遷移の二進冪を共有して進められる。

## 厳密な定義

- `matrix`: 加算・乗算を持つ型の行列: matrix<mint> a(h, w); 乗算O(hwk)。
- `semiring_matrix_product`: O(hwk)。zero・add・multiplyで定めた半環上の行列積を返す。
- `semiring_matrix_power`: O(n^3 log exponent)。zero・one・演算を指定した半環上の正方行列累乗を返す。
- `minplus_matrix_product`: O(hwk)。min-plus半環で行列積を返す。infinity以上は到達不能として扱う。
- `minplus_matrix_power`: O(n^3 log exponent)。ちょうどexponent辺の最小費用を表すmin-plus行列累乗を返す。
- `matrix_vector_product`: 行列を列vectorへ作用させる。
- `sparse_matrix_events`: 通常遷移を繰り返し、指定時刻だけcallbackを適用する。
- `sparse_matrix_transitions`: 指定時刻だけ共通の例外行列へ置き換える。

状態数が大きい、遷移が疎、時刻ごとに遷移が変わる場合は、そのままの行列積は向かない。
線形漸化式の一項だけなら`linear_recurrence`の方が`O(k^2 log n)`で軽い。

## Include

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

### 疎な例外時刻を持つ線形DP

状態 $v_t$ が通常は

$$
v_t=Tv_{t-1}
$$

で進み、時刻集合 $E$ だけ例外遷移 $B$ を使う場合は次のように書く。

```cpp
auto final_state = poe::sparse_matrix_transitions(
    initial, normal_transition, exception_transition,
    target_time, exception_positions
);
```

`initial`は時刻0の列vectorである。`exception_positions`は狭義昇順で、各値は`1..target_time`。
時刻$t$が例外なら$Bv_{t-1}$、それ以外なら$Tv_{t-1}$を計算する。

例外ごとに異なる処理を行う場合はcallback版を使う。

```cpp
auto final_state = poe::sparse_matrix_events(
    initial, transition, target_time, event_positions,
    [&](auto& state, unsigned long long time) {
        // timeに固有の処理でstateを更新する
    }
);
```

状態次元を$d$、例外数を$q$とすると、共通の例外行列を使う版の計算量は
$O(d^3\log T+qd^2\log T)$、領域計算量は$O(d^2\log T+d+q)$。
通常行列の二進冪は一度だけ構築する。

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

### `matrix_vector_product`

```cpp
template <class T> std::vector<T> matrix_vector_product( const matrix<T>& coefficients, const std::vector<T>& values )
```

O(hw)。matrixと列vectorの積を返す。

### `sparse_matrix_events`

```cpp
template <class T, class Event> requires std::invocable<Event&, std::vector<T>&, unsigned long long> std::vector<T> sparse_matrix_events( std::vector<T> initial, const matrix<T>& transition, unsigned long long target, const std::vector<unsigned long long>& event_positions, Event event )
```

O(d^3 log target+q d^2 log target)。通常行列で進み、指定時刻だけevent(state,time)を適用する。

### `sparse_matrix_transitions`

```cpp
template <class T> std::vector<T> sparse_matrix_transitions( std::vector<T> initial, const matrix<T>& transition, const matrix<T>& exception_transition, unsigned long long target, const std::vector<unsigned long long>& exception_positions )
```

O(d^3 log target+q d^2 log target)。指定時刻だけexception_transitionへ置き換えてtargetまで進む。

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
`verify/atcoder_abc445_f.cpp`で公式sampleを確認済み。

[AtCoder ABC258 Ex - Odd Steps](https://atcoder.jp/contests/abc258/tasks/abc258_h)では、
禁止された累積和だけ遷移行列を置き換える。
`verify/atcoder_abc258_h.cpp`に提出用コードを収録している。
