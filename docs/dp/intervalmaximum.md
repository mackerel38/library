---
title: "区間最大値制約付き列の数え上げ"
documentation_of: //cp/dp/intervalmaximum.hpp
---

# 区間最大値制約付き列の数え上げ

## 概要

各要素が $0$ 以上の整数である列について、「この区間の最大値はちょうど $x$」という制約をすべて満たす列の個数を求める。
区間ごとの上限制約と、「最大値を実際に取る要素が少なくとも一つある」という条件をまとめて処理する。

## 厳密な定義

長さ $n$ の整数列 $A=(A_0,\ldots,A_{n-1})$ と、全体の上限 $M$ を考える。
各制約 $(l,r,x)$ は半開区間 $[l,r)$ に対して

$$
\max_{l\le i<r} A_i=x
$$

を要求する。`count_interval_maximum_sequences`は

$$
0\le A_i\le M\quad(0\le i<n)
$$

と、与えたすべての区間最大値制約を満たす列 $A$ の個数を法 `mod` で返す。

## Include

```cpp
#include "dp/intervalmaximum.hpp"
```

## API

```cpp
struct intervalmaximumconstraint {
    int left;
    int right;
    int maximum;
};

template <int mod>
staticmodint<mod> count_interval_maximum_sequences(
    int size,
    int maximum_value,
    const vector<intervalmaximumconstraint>& constraints
);
```

- `size`: 列の長さ $n$
- `maximum_value`: 各要素の全体上限 $M$
- `constraints`: 半開区間 `[left, right)` の最大値を `maximum` にする制約

`0 <= maximum <= maximum_value < mod` が必要である。

## 使い方

```cpp
vector<poe::intervalmaximumconstraint> constraints = {
    {0, 2, 2}, // max(A[0], A[1]) = 2
    {1, 3, 3}, // max(A[1], A[2]) = 3
};

auto answer = poe::count_interval_maximum_sequences<998244353>(
    3, 3, constraints
);
cout << answer << '\n'; // 5
```

## 計算量

列の長さを $n$、制約数を $q$ とすると、時間計算量は $O((n+q)\log(n+q))$、空間計算量は $O(n+q)$。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `intervalmaximumconstraint`

```cpp
struct intervalmaximumconstraint
```

count_interval_maximum_sequencesへ渡す半開区間[left,right)の最大値制約。

### `count_interval_maximum_sequences`

```cpp
template <int mod> staticmodint<mod> count_interval_maximum_sequences( int size, int maximum_value, const std::vector<intervalmaximumconstraint>& constraints )
```

O((n+q) log(n+q))時間・O(n+q)領域。0..maximum_valueの列で全区間最大値制約を満たす個数。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC262 Ex - Max Limited Sequence](https://atcoder.jp/contests/abc262/tasks/abc262_h)では、
入力区間を0-indexedの半開区間へ直してそのまま渡せる。

`verify/atcoder_abc262_h.cpp`に提出用コードを収録している。
