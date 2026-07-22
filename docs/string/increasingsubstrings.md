---
title: "位置順・辞書順に増加する非重複substring列"
documentation_of: //cp/string/increasingsubstrings.hpp
---

# 位置順・辞書順に増加する非重複substring列

## 概要

一つの文字列から、互いに重ならないsubstringを左から順に選びます。
選んだ文字列も辞書順で真に増加するようにしたとき、最大で何個選べるかを求めます。

## 厳密な定義

長さ $n$ の文字列 $S$ に対して、半開区間

$$
[L_1,R_1),[L_2,R_2),\ldots,[L_K,R_K)
$$

が

$$
0\le L_i<R_i\le n,\qquad R_i\le L_{i+1}
$$

および

$$
S[L_i,R_i)<_{\mathrm{lex}}S[L_{i+1},R_{i+1})
$$

を満たすような最大の $K$ を返します。辞書順は文字の `unsigned char` 値を使います。

最適解では、$i$ 番目に選ぶsubstringの長さを高々 $i$ にできます。したがって
$1+2+\cdots+K\le n$ であり、長さ

$$
B=\max\left\{k\mid \frac{k(k+1)}2\le n\right\}=O(\sqrt n)
$$

までのsubstringだけを考えれば十分です。それらを明示的にsortせず、trieをDFSする順で
辞書順に生成します。各出現 $[L,R)$ に対する遷移は、$L$ より前で終わる解の最大値へ
$1$ を足すFenwick最大値DPです。同じ文字列の全出現は問い合わせを先に済ませてから更新し、
辞書順の「真に小さい」を保ちます。

時間計算量は $O(n\sqrt n\log n)$、領域計算量は $O(n\sqrt n)$ です。

## Include

```cpp
#include "string/increasingsubstrings.hpp"
```

## 使い方

```cpp
int answer = maximum_increasing_disjoint_substrings(text);
```

```cpp
int maximum_increasing_disjoint_substrings(string_view text);
```

空文字列では $0$、空でない文字列では少なくとも $1$ を返します。alphabetの種類数に
制限はありません。

## 頻出事故

- 選ぶsubstring同士は同じ位置を共有できませんが、間に未使用文字があっても構いません。
- 辞書順は真に増加します。同じ内容のsubstringを二回選べません。
- substringの出現位置は区別します。同じ内容でも異なる位置はDPの候補になります。
- 返すのは最大個数です。選んだ区間自体は復元しません。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `maximum_increasing_disjoint_substrings`

```cpp
inline int maximum_increasing_disjoint_substrings(std::string_view text)
```

O(n sqrt(n) log n)時間・O(n sqrt(n))領域。位置順・辞書順とも真に増加する非重複substring列の最大長を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC240 Ex - Sequence of Substrings](https://atcoder.jp/contests/abc240/tasks/abc240_h)は、
二進文字列に対してこの最大値をそのまま求める問題です。

```cpp
cout << maximum_increasing_disjoint_substrings(text) << '\n';
```
