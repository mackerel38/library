---
title: "wildcard列の狭義増加補完"
documentation_of: //cp/dp/wildcardorder.hpp
---

# wildcard列の狭義増加補完

  `poe::count_strictly_increasing_wildcard_numbers`

## 概要

同じ長さのwildcard列を独立に補完し、列全体を辞書順で狭義増加にする方法数を数える。
一般版は各位置を`0..alphabet_size-1`または指定したwildcard値で表す。

## 厳密な定義

- `count_strictly_increasing_wildcard_sequences`: O(m b n^3)。同長wildcard列を辞書順狭義増加へ補完する方法数を返す。
- `count_strictly_increasing_wildcard_numbers`: O(m n^3)。数字と?からなる同長文字列を整数として狭義増加へ補完する方法数を返す。

## Include

```cpp
#include "dp/wildcardorder.hpp"

auto answer =
    poe::count_strictly_increasing_wildcard_numbers<998244353>(patterns);
```

各桁で行の連続区間を同じ記号のgroupへ分割し、同じgroup内だけ次桁以降でも狭義増加を要求する。
数字版は`?`をwildcardとして扱い、先頭0も通常の桁に含める。

列数`n`、長さ`m`、alphabet数`b`として`O(mbn^3)`時間、`O(n^2)`領域。

## インターフェイス

```cpp
template <class Value>
Value count_strictly_increasing_wildcard_sequences(
    const vector<vector<int>>& patterns,
    int alphabet_size,
    int wildcard = -1);
```

各`patterns[i]`を1つの列とみなし、`patterns[0] < patterns[1] < ...`を辞書順で
満たす補完数を返す。全列の長さは同じで、各要素は`wildcard`または
`[0, alphabet_size)`でなければならない。`Value`には`0`・`1`からの構築と
`+`, `+=`, `*`, `==`が必要。空の列集合には`1`を返す。

```cpp
template <int modulus>
staticmodint<modulus> count_strictly_increasing_wildcard_numbers(
    const vector<string>& patterns);
```

数字版の短縮形。各文字は`0`から`9`または`?`で、`?`を任意の数字へ置き換える。
文字列は数値変換せず固定長の辞書順で比較するため、先頭の`0`も通常の桁として扱う。
戻り値は補完数を`modulus`で割った値。計算量は`O(mn^3)`、領域は`O(n^2)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_strictly_increasing_wildcard_sequences`

```cpp
template <class Value> Value count_strictly_increasing_wildcard_sequences( const std::vector<std::vector<int>>& patterns, int alphabet_size, int wildcard = -1)
```

O(m b n^3)。同長wildcard列を辞書順狭義増加へ補完する方法数を返す。

### `count_strictly_increasing_wildcard_numbers`

```cpp
template <int modulus> staticmodint<modulus> count_strictly_increasing_wildcard_numbers( const std::vector<std::string>& patterns)
```

O(m n^3)。数字と?からなる同長文字列を整数として狭義増加へ補完する方法数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC292 G - Count Strictly Increasing Sequences](https://atcoder.jp/contests/abc292/tasks/abc292_g)
へ直接使える。`verify/atcoder_abc292_g.cpp`で公式sample 3件を確認済み。

## 検証

- `tests/api/dp/wildcardorder.cpp`: 数字版と一般alphabet版
- `tests/property/dp/wildcardorder.cpp`: 小入力の全wildcard補完との比較
