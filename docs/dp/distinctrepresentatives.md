---
title: "distinctrepresentatives"
documentation_of: //cp/dp/distinctrepresentatives.hpp
---

# `distinctrepresentatives`

## 概要

各人が自分の候補集合から一つずつ値を選ぶとき、全員が互いに異なる値を選ぶ方法数を求める。
候補集合を直接列挙できなくても、任意個の集合の共通部分サイズが分かれば計算できる。

## 厳密な定義

有限集合 $S_0,S_1,\ldots,S_{n-1}$ に対し、

$$
x_i\in S_i\quad(0\le i<n),\qquad x_i\ne x_j\quad(i\ne j)
$$

を満たす順序付き列 $(x_0,\ldots,x_{n-1})$ の個数を返す。
`intersection_count(mask)` は、非空な添字集合 $B$ をbit maskで受け取り、
$\left|\bigcap_{i\in B}S_i\right|$ を返す。

## Include

```cpp
#include "dp/distinctrepresentatives.hpp"
```

## API

```cpp
T count_distinct_representatives<T>(int size, intersection_count);
```

`size` は集合数で31未満。`intersection_count(mask)` は非空maskに対する共通部分サイズを返す。
集合分割上のMöbius反転を用いる。計算量は $O(3^n)$、使用メモリは $O(2^n)$。

```cpp
T count_distinct_bounded_multiples<T>(long long limit, divisors);
```

$S_i=\{x\mid1\le x\le\mathrm{limit},\ \mathrm{divisors}[i]\mid x\}$ としたshortcut。
各共通部分は添字集合の最小公倍数を $L$ として $\lfloor\mathrm{limit}/L\rfloor$ 個である。
最小公倍数が `limit` を超える場合もoverflowせず0として扱う。計算量とメモリは同じ。

## 使用例

[AtCoder ABC236 Ex - Distinct Multiples](https://atcoder.jp/contests/abc236/tasks/abc236_h)では、入力をそのままshortcutへ渡す。

```cpp
cout << count_distinct_bounded_multiples<modint998244353>(M, divisors) << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_distinct_representatives`

```cpp
template<class T, class IntersectionCount> T count_distinct_representatives(int size, IntersectionCount intersection_count)
```

O(3^n)時間・O(2^n)領域。集合族から互いに異なる代表元を順序付きで選ぶ方法数を返す。

### `count_distinct_bounded_multiples`

```cpp
template<class T> T count_distinct_bounded_multiples( long long limit, const std::vector<long long>& divisors )
```

O(3^n)時間・O(2^n)領域。各iがdivisors[i]の倍数を選び、全値が相異なる列の個数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
