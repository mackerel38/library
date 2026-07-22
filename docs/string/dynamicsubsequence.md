---
title: "動的な異なる部分列数"
documentation_of: //cp/string/dynamicsubsequence.hpp
---

# 動的な異なる部分列数

## 概要

各位置で使える文字が一つまたは複数ある列について、作れる相異なる非空部分列の個数を管理する。
一位置の許容文字集合を変更した後も、列全体を数え直さずに答えられる。

## 厳密な定義

alphabetを $\{0,1,\ldots,A-1\}$ とし、各位置 $i$ に許容文字集合 $S_i$ が与えられる。
添字列 $0\le i_1<i_2<\cdots<i_k<n$ と文字 $c_j\in S_{i_j}$ を選んで得られる
非空列 $(c_1,c_2,\ldots,c_k)$ のうち、相異なるものの個数を返す。

位置 $i$ の集合 $S_i$ はbit maskで表し、bit $c$ が立っていることと $c\in S_i$ が同値である。

## Include

```cpp
#include "string/dynamicsubsequence.hpp"
```

## 使い方

```cpp
using mint = poe::modint998244353;
poe::dynamicsubsequencecount<mint, 2> sequence({1, 3, 2});

sequence.set(0, 3);          // 位置0で0と1の両方を許す
mint answer = sequence.count();
std::uint64_t mask = sequence[0];
```

## APIの選び方

| 操作 | API | 計算量 |
|---|---|---|
| 許容文字集合列から構築 | constructor | $O(nA^3)$ |
| 一位置の許容集合を変更 | `set(index, mask)` | $O(A^3\log n)$ |
| 一位置の許容集合を取得 | `operator[](index)` | $O(1)$ |
| 相異なる非空部分列数 | `count()` | $O(A)$ |

末尾の文字ごとに異なる部分列数を持つ。文字$c$を現在位置で選べるとき、新しく末尾を$c$にする
列は、それ以前の全ての部分列と空列へ$c$を追加したものになる。このaffine変換を
$(A+1)\times(A+1)$行列で表し、位置順の積をSegment Treeで管理する。
領域計算量は$O(nA^2)$である。

## 頻出事故

- `mask` のbit番号が文字番号であり、文字コードそのものではない。
- `count()`は空列を含まない。
- `Alphabet`は$64$以下で、文字種類数が大きい問題には適さない。
- `T`には加算・乗算と`T{1}`が必要で、通常はmodintを使う。
- maskが$0$の位置は、どの部分列にも使用されない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `dynamicsubsequencecount`

```cpp
template<class T, int Alphabet> struct dynamicsubsequencecount
```

小alphabetの各位置の許容文字を更新し、異なる非空部分列数を管理する。
構築O(n A^3)、set O(A^3 log n)、count O(A)、領域O(n A^2)。

### `dynamicsubsequencecount`

```cpp
explicit dynamicsubsequencecount(const std::vector<mask_type>& allowed) : n_(static_cast<int>(allowed.size())), allowed_(allowed)
```

O(n A^3)。allowed[i]の立っているbitを位置iで選べる文字として構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。位置数を返す。

### `set`

```cpp
void set(int index, mask_type mask)
```

O(A^3 log n)。位置indexで選べる文字集合をmaskへ変更する。

### `operator[]`

```cpp
mask_type operator[](int index) const
```

O(1)。位置indexで選べる文字集合のbit maskを返す。

### `count`

```cpp
T count() const
```

O(A)。現在作れる相異なる非空部分列の個数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC246 Ex - 01? Queries](https://atcoder.jp/contests/abc246/tasks/abc246_h)では、
`0`をmask `1`、`1`をmask `2`、`?`をmask `3`として渡す。各更新後の`count()`が答えになる。
`verify/atcoder_abc246_h.cpp`に提出用コードを用意している。
