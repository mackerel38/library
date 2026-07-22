---
title: "rangeproductpower"
documentation_of: //cp/math/rangeproductpower.hpp
---

# `rangeproductpower`

## 概要

正整数列の各区間について、区間内の数の積が平方数・立方数などの完全累乗数かをまとめて判定する。

## 厳密な定義

正整数列 $A=(A_0,\ldots,A_{n-1})$、半開区間 $[l_i,r_i)$、整数 $k\ge2$ に対し、各 $i$ について

$$
\prod_{j=l_i}^{r_i-1}A_j=x^k
$$

を満たす正整数 $x$ が存在するかを返す。

## Include

```cpp
#include "math/rangeproductpower.hpp"
```

## API

```cpp
vector<bool> range_product_power_queries(
    const vector<int>& values,
    const vector<pair<int, int>>& queries,
    int exponent
);
```

- `values`: 正整数列。
- `queries`: 判定する半開区間 `[left, right)` の列。
- `exponent`: 累乗指数。`2`以上。
- 返り値: 各queryに対する判定を入力順に格納した列。

$A=\max_i A_i$ とする。線形篩で各値を素因数分解し、Mo's algorithmで区間内の各素因数指数を
modulo $k$ で管理する。すべての指数が0であることと、積が完全 $k$ 乗であることは同値である。

計算量は $O(A+n\log A+(n+q)\sqrt q\log A)$、使用メモリは $O(A+n\log A+q)$。
判定は乱択を使わず厳密である。

## 使用例

[AtCoder ABC238 G - Cubic?](https://atcoder.jp/contests/abc238/tasks/abc238_g)では、`exponent=3`として各区間積が立方数かを判定する。

```cpp
for (bool answer : range_product_power_queries(values, queries, 3)) {
    cout << (answer ? "Yes" : "No") << '\n';
}
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `range_product_power_queries`

```cpp
inline std::vector<bool> range_product_power_queries( const std::vector<int>& values, const std::vector<std::pair<int, int>>& queries, int exponent )
```

O(A+n log A+(n+q)sqrt(q)log A)。各区間の積がexponent乗数かを厳密に判定する。

<!-- END AUTO-GENERATED API REFERENCE -->
