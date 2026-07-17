---
title: "多項式補間"
documentation_of: //cp/math/interpolation.hpp
---

# 多項式補間

- Header: `cp/math/interpolation.hpp`
- Symbol: `poe::lagrange_consecutive`
- Status: experimental

## どんな問題に使えるか

次数`d`以下と分かっている多項式を`f(0),...,f(d)`から復元せず、任意の一点`f(x)`だけを
`O(d)`で評価する。DPの答えがパラメータに関する低次多項式になる問題や、巨大な`x`での総和に向く。

値が多項式にならない、標本点が連続整数でない、法の上で標本点が衝突する場合には使えない。

## API・計算量

```cpp
#include "math/interpolation.hpp"

mint answer = poe::lagrange_consecutive(samples, x);
```

`n`個の標本に対し`O(n)`時間・`O(n)`領域。`T`は除算可能な体であること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lagrange_consecutive`

```cpp
template <class T> T lagrange_consecutive(const std::vector<T>& values, long long x)
```

O(n)。lagrange_consecutive(values, x): f(i)=values[i]の次数<n多項式のf(x)を返す。
Tは体であり、0,1,...,n-1が相異なること。xがこの範囲なら対応値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[NDPC Q - 区間の和集合](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_q)の公式テーマは
多項式補間によるDPの省メモリ化である。問題全体のDPは固有で、この関数は補間部分を担当する。
。
