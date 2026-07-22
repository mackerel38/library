---
title: "intervalinsertion"
documentation_of: //cp/dp/intervalinsertion.hpp
---

# `intervalinsertion`

## 概要

区間の両端だけが置かれた状態から内部の要素を一つずつ挿入する全順序について、場合の数と費用総和をまとめて求める。
円環上で各要素が向いている側の最近傍を削除する確率過程も直接計算できる。

## 厳密な定義

### `interval_insertion_aggregate`

区間 $(l,r)$ に最初に挿入する位置を $m$ とする。残りを $(l,m)$ と $(m,r)$ に分け、左右の挿入順を任意に混ぜる。
`choice(l,m,r)` が返す選択肢数を $w_{l,m,r}$、その選択肢の即時費用の総和を $c_{l,m,r}$ とする。
すべての再帰的な挿入順と各段階の選択肢について、その重みの総和と費用の総和を各区間に返す。

### `circular_directional_removal_expectation`

円環上の $n$ 個の要素はそれぞれ左 `L` または右 `R` を向く。各時刻に残っている要素を一様に一つ選び、
その要素が向く側で最初にある別の要素を削除し、元の円環上の距離を費用として払う。
一要素だけになるまでの総費用の期待値を返す。

## Include

```cpp
#include "dp/intervalinsertion.hpp"
```

## API

```cpp
template<class T>
struct intervalinsertionchoice {
    T count;
    T cost;
};

auto result = interval_insertion_aggregate<T>(size, maximum_span, choice);
result.count_at(left, right);
result.cost_at(left, right);
```

`choice(left, middle, right)` は `intervalinsertionchoice<T>` を返す。
`count` はその挿入で区別する選択肢数、`cost` は全選択肢の即時費用の総和である。
幅が `maximum_span` 以下の区間だけを計算する。計算量は
$O(\mathrm{size}\cdot\mathrm{maximum\_span}^2)$、使用メモリは $O(\mathrm{size}^2)$。

```cpp
T circular_directional_removal_expectation<T>(string_view directions);
```

`directions` は `L` と `R` だけからなる長さ2以上の文字列。長さを $n$ とすると計算量は $O(n^3)$、使用メモリは $O(n^2)$。
`T` は整数から構築でき、四則演算を持つ体の要素とする。

## 使用例

[AtCoder ABC238 Ex - Removing People](https://atcoder.jp/contests/abc238/tasks/abc238_h)では、向きの文字列をそのまま渡す。

```cpp
cout << circular_directional_removal_expectation<modint998244353>(directions) << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `intervalinsertionchoice`

```cpp
template<class T> struct intervalinsertionchoice
```

一回の挿入で区別される選択肢数countと、その即時費用の総和cost。

### `intervalinsertionaggregate`

```cpp
template<class T> struct intervalinsertionaggregate
```

区間内の全挿入順と選択肢について、場合の数と費用総和を保持する。

### `count_at`

```cpp
const T& count_at(int left, int right) const
```

O(1)。端点left,rightの間を埋める場合の数を返す。

### `cost_at`

```cpp
const T& cost_at(int left, int right) const
```

O(1)。端点left,rightの間を埋める全場合の費用総和を返す。

### `interval_insertion_aggregate`

```cpp
template<class T, class Choice> intervalinsertionaggregate<T> interval_insertion_aggregate( int size, int maximum_span, Choice choice )
```

O(size*maximum_span^2)。両端から始めて区間内部を挿入する全順序の重みと費用を集約する。

### `circular_directional_removal_expectation`

```cpp
template<class T> T circular_directional_removal_expectation(std::string_view directions)
```

O(n^3)時間・O(n^2)領域。円環で向く側の最近傍を一様な生存者が削除する総距離の期待値。

<!-- END AUTO-GENERATED API REFERENCE -->
