---
title: "Gauss消去法"
documentation_of: //cp/math/gaussian.hpp
---

# Gauss消去法

- Header: `cp/math/gaussian.hpp`
- Symbol: `poe::linearstatus`, `poe::linearsystemresult`, `poe::matrix_rank`, `poe::determinant`, `poe::solve_linear`, `poe::binary_matrix_rank`, `poe::solve_binary_linear`
- Status: experimental

```cpp
#include "math/gaussian.hpp"
```

除算できる体上でrank、行列式、連立一次方程式を求める。`solve_linear(A,b)`は解の有無だけでなく、
特殊解`solution`と斉次方程式の解空間`basis`を返す。全解は特殊解へbasisの任意の線形結合を足したも。

`h*w`行列の計算量は`O(hw min(h,w))`、正方行列では`O(n^3)`、領域`O(hw)`。
浮動小数の近似0判定は行わないため、主に`modint`などの厳密な体を想定する。

## GF(2)版

`binary_matrix_rank(A)`と`solve_binary_linear(A, b)`は、要素が0か1の`matrix<int>`を
64 bit単位にpackしてGF(2)上の消去を行う。後者の返り値は一般の体と同じ`linearsystemresult<int>`で、
`solution`が一つの0/1解、`basis`がXORで足せる斉次解空間の基底になる。

消去の計算量は`h*w*min(h,w)/64`、領域は`O(hw/64)`。全kernel basisの出力には別に
`O(w^2)`時間・領域が必要になる。rankだけ必要なら`binary_matrix_rank`を使い、basisを構築しない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `linearstatus`

```cpp
enum class linearstatus
```

linearstatus: 連立一次方程式の解なし・一意解・複数解。

### `linearsystemresult`

```cpp
template<class T> struct linearsystemresult
```

solve_linearの結果。solutionは特殊解、basisは斉次解空間の基底。

### `operator[]`

```cpp
const T& operator[](int index) const
```

O(1)。result[i]: 特殊解のi番目を返す。

### `matrix_rank`

```cpp
template<class T> int matrix_rank(matrix<T> values)
```

O(h*w*min(h,w))。matrix_rank(a): 体上の行列aのrankを返す。

### `determinant`

```cpp
template<class T> T determinant(matrix<T> values)
```

O(n^3)。determinant(a): 体上の正方行列aの行列式を返す。

### `solve_linear`

```cpp
template<class T> linearsystemresult<T> solve_linear(matrix<T> coefficients, const std::vector<T>& right)
```

O(h*w*min(h,w))。solve_linear(a,b): 体上のAx=bを解き、特殊解と斉次解基底を返す。

### `binary_matrix_rank`

```cpp
inline int binary_matrix_rank(const matrix<int>& values)
```

O(hw min(h,w)/64)。0/1行列aをGF(2)上でbit並列Gauss消去し、rankを返す。

### `solve_binary_linear`

```cpp
inline linearsystemresult<int> solve_binary_linear( const matrix<int>& coefficients, const std::vector<int>& right )
```

O(hw min(h,w)/64 + w^2)。GF(2)上のAx=bを解き、0/1の特殊解と斉次解基底を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC366 G - XOR Neighbors](https://atcoder.jp/contests/abc366/tasks/abc366_g)ではGF(2)の解空間基底が必要になる。
一般の体版とGF(2)版についてLibrary Checker System of Linear Equationsのverifyコードを用意している。
どちらもproperty test済みだが、GF(2)版。
