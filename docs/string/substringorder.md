---
title: "全部分文字列の辞書順順位"
documentation_of: //cp/string/substringorder.hpp
---

# 全部分文字列の辞書順順位

## 概要

一つまたは複数の文字列に現れる、空でない全部分文字列を辞書順に並べる。
同じ文字列が別の場所に現れた場合も別々に数え、指定した順位にある部分文字列の出現位置を返す。

全部分文字列を列挙すると二乗個になるが、この構造は入力文字列の合計長に比例する領域だけを使う。

## 厳密な定義

文字列を $S_0,\ldots,S_{K-1}$ とし、出現の集合を

$$
\mathcal O=\{(i,l,r)\mid 0\le i<K,\ 0\le l<r\le |S_i|\}
$$

とする。各出現 $(i,l,r)$ の値は $S_i[l,r)$ である。`substringorder` は $\mathcal O$ を
値の辞書順に並べた列を表す。同じ値を持つ出現同士の順序は区別しない。

`kth(k)` は、0-indexedで $k$ 番目にある値を持つ出現を一つ返す。同値な出現が複数ある場合、
そのうちどれを返すかは未規定であり、異なる順位に同じ出現を返すこともある。

## Include

```cpp
#include "string/substringorder.hpp"
```

## 使い方

```cpp
vector<string> strings = {"aba", "a"};
substringorder order(strings);

long long count = order.size();
auto occurrence = order.kth(k);  // kは0-indexed
string value = strings[occurrence.string_index].substr(
    occurrence.left, occurrence.right - occurrence.left);
```

`occurrence.left`と`occurrence.right`は0-indexedの半開区間である。`operator[]`も`kth`と同じ働きをする。

## 計算量

入力文字列数を $K$、合計長を $L$ とする。

- 構築: $O(L+K)$ 時間、$O(L+K)$ 領域
- `size()`: $O(1)$
- `kth(k)`、`operator[](k)`: $O(\log L)$

`size()`の返り値は、重複する出現を区別した
$\sum_i |S_i|(|S_i|+1)/2$ である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `substringoccurrence`

```cpp
struct substringoccurrence
```

一つの部分文字列の出現位置。strings[string_index][left, right)を表す。

### `substringorder`

```cpp
struct substringorder
```

構築O(L+K)、追加領域O(L+K)。K個の文字列の全部分文字列を辞書順で順位検索する。

### `substringorder`

```cpp
explicit substringorder(const std::vector<std::string>& strings)
```

O(L+K)。stringsの非空な全部分文字列の出現を辞書順に並べる索引を構築する。

### `substringorder`

```cpp
explicit substringorder(const std::string& text) : substringorder(std::vector<std::string>
```

O(L)。一つの文字列textの非空な全部分文字列を辞書順に並べる索引を構築する。

### `size`

```cpp
long long size() const
```

O(1)。重複する出現を区別した非空部分文字列の総数を返す。

### `kth`

```cpp
substringoccurrence kth(long long index) const
```

O(log L)。0-indexedでindex番目の辞書順部分文字列を表す出現位置を返す。

### `operator[]`

```cpp
substringoccurrence operator[](long long index) const
```

O(log L)。kth(index)と同じ。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC280 Ex - Substring Sort](https://atcoder.jp/contests/abc280/tasks/abc280_h)では、
queryの1-indexed順位を`rank`として、次のように出力できる。

```cpp
const substringorder order(strings);
const auto occurrence = order[rank - 1];
cout << occurrence.string_index + 1 << ' '
     << occurrence.left + 1 << ' '
     << occurrence.right << '\n';
```

提出用コードは`verify/atcoder_abc280_h.cpp`にあり、公式sample 3件を確認済み。
