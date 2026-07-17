---
title: "禁止値を含まない相異なる部分文字列"
documentation_of: //cp/string/distinctsubstring.hpp
---

# 禁止値を含まない相異なる部分文字列


## 概要

列に現れる相異なる空でない部分文字列のうち、指定した禁止値を一つも含まないものを数える。
文字列だけでなく整数列にも使え、区切り値で分割された複数列をまとめて処理するときにも便利。

## 厳密な定義

- `count_distinct_substrings_without`: O(n log n)。forbiddenを含まない相異なる空でない部分文字列の個数を返す。

suffix array順に各suffixが初めて追加するprefixだけを数え、禁止値までの長さで打ち切る。
出現位置を区別して数える問題や、複数種類の禁止値を直接指定する用途には使えない。

## Include

```cpp
#include "string/distinctsubstring.hpp"

long long answer = count_distinct_substrings_without(sequence, delimiter);
```

`O(n log n)`時間、`O(n)`領域。比較可能な値型と、列に使っていない区切り値を渡す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_distinct_substrings_without`

```cpp
template <class T> long long count_distinct_substrings_without( const std::vector<T>& sequence, const T& forbidden )
```

O(n log n)。forbiddenを含まない相異なる空でない部分文字列の個数を返す。

### `count_distinct_substrings_without`

```cpp
inline long long count_distinct_substrings_without( const std::string& text, char forbidden )
```

O(n log n)。forbiddenを含まない相異なる空でない部分文字列の個数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC452 G - 221 Substring](https://atcoder.jp/contests/abc452/tasks/abc452_g)では、
run-length encodingを有効なrun列と区切り値へ変換した後、本関数で相異なる有効区間を数えられる。
`verify/atcoder_abc452_g.cpp`に提出用コードを用意し、公式sampleを確認済み。
