---
title: "周期文字列"
documentation_of: //cp/string/periodic.hpp
---

# 周期文字列


## 概要

`primitive_period(text)`は、文字列全体を整数回繰り返して構成できる最短文字列を返す。
例えば`"ababab"`なら`"ab"`、`"ababa"`なら元の文字列を返す。

## 厳密な定義

- `primitive_period`: O(n)。primitive_period(text): textを構成する最短周期文字列を返す。空文字列は空。
- `minimum_unique_repetition_counts`: O(L alpha(n))目安。順に各文字列を正整数回反復し、過去と異なる最小反復回数を返す。

`minimum_unique_repetition_counts(strings)`は入力順に各文字列`strings[i]`を正整数`k[i]`回繰り返し、
過去に作った文字列と異なるようにする最小の`k[i]`を返す。

## Include

```cpp
#include "string/periodic.hpp"

std::string root = poe::primitive_period(text);
auto counts = poe::minimum_unique_repetition_counts(strings);
```

同じ文字列になる可能性があるのは原始周期が同じ文字列だけである。
各文字列の長さを原始周期長で割った値を基礎反復数とし、同じgroup内で未使用の最小倍数を
successor集合で割り当てる。入力文字列は空でないこと。

## 計算量

入力文字列長の総和を`L`、割当数を`n`として、期待`O(L+n alpha(n))`時間、
`O(L+n)`領域を目安とする。hash tableを使用する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `primitive_period`

```cpp
inline std::string primitive_period(const std::string& text)
```

O(n)。primitive_period(text): textを構成する最短周期文字列を返す。空文字列は空。

### `minimum_unique_repetition_counts`

```cpp
inline std::vector<long long> minimum_unique_repetition_counts( const std::vector<std::string>& strings )
```

O(L alpha(n))目安。順に各文字列を正整数回反復し、過去と異なる最小反復回数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC312 Ex - snukesnuke](https://atcoder.jp/contests/abc312/tasks/abc312_h)は、
入力列をそのまま`minimum_unique_repetition_counts`へ渡せる。
`verify/atcoder_abc312_h.cpp`で公式sampleを確認済み。
短い二進文字列列について、実際の反復文字列を集合に入れる愚直法とも照合済み。
