---
title: "隣接和による列の縮約"
documentation_of: //cp/math/binomialsequence.hpp
---

# 隣接和による列の縮約

## 概要

列の隣り合う二要素を足して長さを一つ減らす操作を、非常に多く繰り返す。
小さい素数を法とし、元の列が連長圧縮されている場合に、展開せず結果も連長圧縮して返す。

## 厳密な定義

列$A^{(0)}=(A_0,A_1,\ldots,A_{n-1})$に対し、

$$
A^{(t+1)}_i=A^{(t)}_i+A^{(t)}_{i+1}\pmod p
\qquad(0\le i<n-t-1)
$$

と定める。`adjacent_sum_reduce_rle<Prime>(runs,steps)`は、$A^{(0)}$の連長圧縮`runs`から
$A^{(steps)}$の連長圧縮を返す。

## Include

```cpp
#include "math/binomialsequence.hpp"
```

```cpp
vector<pair<int, long long>> reduced = adjacent_sum_reduce_rle<7>(runs, steps);
```

`Prime`は素数である必要がある。各valueは$[0,Prime)$、各run長は正で、
$0\le steps<n$でなければならない。長さと`steps`は`long long`で扱う。

$steps$を`Prime`進表記し、Frobenius写像
$(1+x)^{p^q}=1+x^{p^q}$を用いて、距離$p^q$の二要素を足すskipへ分解する。
時間計算量は各skip直前のrun数の総和、領域計算量は処理中のrun数である。

## 頻出事故

- 合成数を`Prime`へ指定してはいけない。
- `runs`には隣接する同値runを統合した通常の連長圧縮を渡す。
- 結果は展開列ではなく`(value,count)`の列である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `adjacent_sum_reduce_rle`

```cpp
template<int Prime> std::vector<std::pair<int, long long>> adjacent_sum_reduce_rle( std::vector<std::pair<int, long long>> runs, long long steps )
```

O(各skip時のrun数の総和)時間・領域。mod Primeで隣接和による長さ1減少をsteps回行ったRLEを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC251 Ex - Fill Triangle](https://atcoder.jp/contests/abc251/tasks/abc251_h)では、
最下段のRLEを法7で$N-K$回縮約し、得られた第$K$段だけを展開して出力する。
`verify/atcoder_abc251_h.cpp`に提出用コードを用意し、公式sampleを確認している。
