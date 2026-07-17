---
title: "末尾追加できる閾値写像列"
documentation_of: //cp/structure/thresholdsequence.hpp
---

# 末尾追加できる閾値写像列


## 概要

各操作が一つの閾値`p`を持ち、現在値`x`を

## 厳密な定義

- `thresholdsequence`: thresholdsequence<long long> seq(up, down); seq.append(p); seq.apply(x)。追加ならしO(log n)、適用O(log^2 n)。

- `x <= p`なら`x + up`
- `x > p`なら`x - down`

へ変えるとき、操作の末尾追加と「初期値から全操作を通した最終値」を処理する。長いif列を毎query
走査する必要があるオンライン問題に使える。元の操作列を、同じ写像を表す単調な閾値列へ正規化し、
二進カウンタ状のブロックとして保持する。

## Include

```cpp
#include "structure/thresholdsequence.hpp"

poe::thresholdsequence<long long> sequence(up, down);
for (long long threshold : initial) sequence.append(threshold);
sequence.append(new_threshold);
long long answer = sequence.apply(initial_value);
```

`up,down`は正でなければならない。中間値を含めて型`T`に収まる必要がある。`append`はならし
`O(log n)`時間、`apply`は`O(log^2 n)`時間、全体で`O(n)`領域を使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `thresholdsequence`

```cpp
template<std::signed_integral T> struct thresholdsequence
```

thresholdsequence<long long> seq(up, down); seq.append(p); seq.apply(x)。追加ならしO(log n)、適用O(log^2 n)。

### `thresholdsequence`

```cpp
thresholdsequence(T up, T down) : up_(up), down_(down)
```

O(1)。各操作をx<=thresholdならx+up、そうでなければx-downとして空列を作る。

### `append`

```cpp
void append(T threshold)
```

ならしO(log n)。末尾へ閾値thresholdの操作を追加する。

### `apply`

```cpp
T apply(T value) const
```

O(log^2 n)。valueへ現在の全操作を追加順に適用した値を返す。

### `size`

```cpp
std::size_t size() const noexcept
```

O(1)。追加済み操作数を返す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。操作が空ならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC427 G - Takahashi's Expectation 2](https://atcoder.jp/contests/abc427/tasks/abc427_g)で、
プレゼント価値を閾値として末尾へ追加し、質問の初期テンションへ`apply`する。公式sample確認済み。

## 検証

- `tests/api/structure/thresholdsequence.cpp`: 空列、基本呼び出し、sample相当
- `tests/property/structure/thresholdsequence.cpp`: ランダムな追加途中の全初期値を愚直走査と比較
