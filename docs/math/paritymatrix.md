---
title: "偶数行を禁止した行列の数え上げ"
documentation_of: //cp/math/paritymatrix.hpp
---

# 偶数行を禁止した行列の数え上げ

## 概要

各列の合計が決まっている非負整数行列のうち、どの行にも奇数の要素が少なくとも一つあるものを数えます。
素因数の指数を複数の整数へ配り、各整数が平方数にならないようにする問題などに使えます。

## 厳密な定義

$r$行$c$列の非負整数行列 $A$について、列和を

$$
\sum_{i=0}^{r-1}A_{i,j}=s_j
$$

に固定します。さらに、すべての行 $i$ が

$$
\exists j\quad A_{i,j}\equiv1\pmod2
$$

を満たす行列の個数を返します。

$q$行を「全成分が偶数」として選ぶ包除原理により、答えは

$$
\sum_{q=0}^{r}(-1)^q\binom rq
\prod_{j=0}^{c-1}[x^{s_j}]
\frac{1}{(1-x^2)^q(1-x)^{r-q}}
$$

です。$q$を一つ増やす更新は、係数列へ $1/(1+x)$ を掛ける交互累積和として処理します。

## Include

```cpp
#include "math/paritymatrix.hpp"
```

## API

```cpp
template<class mint>
mint count_matrices_without_even_rows(
    int row_count,
    const vector<int>& column_sums
);
```

- `row_count`は行数、`column_sums[j]`は列 $j$ の合計です。
- `mint`には`staticmodint`など、四則演算ができる型を指定します。
- $D=\max_j s_j$として、$1,2,\ldots,r+D$が法の下で可逆である必要があります。
- 計算量は $O(r(D+c))$、追加メモリは $O(D)$ です。
- 列が空で行が存在する場合は`0`、行も列も空の場合は`1`を返します。

## 使用例

```cpp
using mint = staticmodint<1000000007>;
mint answer = count_matrices_without_even_rows<mint>(3, {3, 2});
// answer == 15
```

[ABC285 Ex - Avoid Square Number](https://atcoder.jp/contests/abc285/tasks/abc285_h)では、行を数列の要素、
列を素数、行列要素を各素数の指数とみなして使えます。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_matrices_without_even_rows`

```cpp
template<class mint> mint count_matrices_without_even_rows( int row_count, const std::vector<int>& column_sums )
```

O(r(d+c))。column_sumsを列和とし、全成分が偶数の行がないr行の非負整数行列数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
