---
title: "XOR tuple・distinct tuple・multiset計数"
documentation_of: //cp/math/xorcount.hpp
---

# XOR tuple・distinct tuple・multiset計数

## 概要

値域$[0,M]$から選ぶ順序付きtupleについて、XORが指定値になる個数を長さごとに求める。
さらに、重複を許すtuple数から全要素が相異なるtuple数へ変換し、非減少列、すなわちmultiset数へ変換できる。

値そのものを全列挙できないが、長さが数百程度のXOR数え上げに使う。

## 厳密な定義

### `bounded_xor_tuple_counts`

返り値$F$は
$$
F_L=\left|\left\{(a_1,\ldots,a_L)\in[0,M]^L\mid
a_1\mathbin{\mathtt{xor}}\cdots\mathbin{\mathtt{xor}}a_L=X\right\}\right|
$$
を満たす。$0\le L\le n$をすべて返す。

### `distinct_xor_tuple_counts`

大きさ$U$の値集合がXORにより指数2の可換群の部分集合として与えられているとする。
入力$F_L$を、その集合から選ぶ重複可能な順序付きtupleのXORが固定値となる個数とする。
返り値$G_L$は、同じ条件で全要素が相異なる順序付きtupleの個数である。

同じ値の偶数回出現はXORから消えることを使うため、加算や一般のmod和には適用できない。

### `xor_multiset_count`

$G_L$を`distinct_xor_tuple_counts`の返り値とする。大きさ$U$の値集合から重複を許して$n$個選び、
XORが同じ固定値となるmultisetの個数を返す。これは値を非減少に並べた長さ$n$の列の個数と等しい。

## Include

```cpp
#include "math/xorcount.hpp"
```

## 使用例

```cpp
using mint = poe::modint998244353;
auto tuple = poe::bounded_xor_tuple_counts<mint>(n, maximum, target);
auto distinct = poe::distinct_xor_tuple_counts(tuple, maximum + 1);
mint answer = poe::xor_multiset_count(n, maximum + 1, distinct);
```

## 計算量と前提

- `bounded_xor_tuple_counts`: $O(n^3\log(M+X))$時間、$O(n)$作業領域。
- `distinct_xor_tuple_counts`: $O(n^3)$時間、$O(n^2)$領域。
- `xor_multiset_count`: $O(n)$時間、$O(n)$領域。

`T`は四則演算ができる体を想定する。`xor_multiset_count`は$0!,\ldots,n!$が可逆であることを要求する。
`alphabet_size`は実際に選べる相異なる値の個数であり、$[0,M]$なら$M+1$を渡す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `bounded_xor_tuple_counts`

```cpp
template<class T> std::vector<T> bounded_xor_tuple_counts( int maximum_length, unsigned long long maximum, unsigned long long target )
```

O(n^3 log(maximum+target))。0..maximumから選ぶ長さ0..nの順序付きtupleでXORがtargetとなる個数を返す。

### `distinct_xor_tuple_counts`

```cpp
template<class T> std::vector<T> distinct_xor_tuple_counts( const std::vector<T>& tuple_counts, long long alphabet_size )
```

O(n^3)時間・O(n^2)空間。XORが固定値となる重複可tuple数から、全要素が相異なるtuple数を返す。

### `xor_multiset_count`

```cpp
template<class T> T xor_multiset_count( int length, long long alphabet_size, const std::vector<T>& distinct_counts )
```

O(n)。distinct_xor_tuple_countsの結果から、長さnの非減少XOR列、すなわちmultiset数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC288 Ex - A Nameless Counting Problem](https://atcoder.jp/contests/abc288/tasks/abc288_h)は、
上の三関数を順に適用する。第一段階では共通上限$M$のtupleをbit DPで数え、第二・第三段階で
順序付き重複列からdistinct列、非減少列へ変換する。

`verify/atcoder_abc288_h.cpp`に提出用コードを収録している。
