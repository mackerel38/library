---
title: "部分配列の値域余剰"
documentation_of: //cp/algorithm/subarrayrange.hpp
---

# 部分配列の値域余剰

## 概要

相異なる整数からなる列について、部分配列が占める値の幅が、その要素数に比べて高々`k`だけ
広いものを数える。`k`が小さく列が長い場合にも使える。

## 厳密な定義

相異なる整数列$A_0,A_1,\ldots,A_{n-1}$に対し、全ての$0\le l\le r<n$のうち

$$
\max_{l\le i\le r}A_i-\min_{l\le i\le r}A_i-(r-l)\le k
$$

を満たす組$(l,r)$の個数を返す。整数が相異なるため左辺は常に非負である。

## Include

```cpp
#include "algorithm/subarrayrange.hpp"
```

```cpp
long long count = count_subarrays_range_excess_at_most(values, k);
```

`k`は非負で、全要素は相異なる必要がある。値および二要素の差は`long long`に収まること。
時間計算量は$O(n(k+1)\log n)$、領域計算量は$O(n(k+1))$である。

右端を伸ばしながら、各左端に対する
$\max-\min-(\text{長さ}-1)$を区間加算で管理する。最大値・最小値が変わる左端区間はそれぞれ
単調stackで列挙する。Segment Treeには小さい方から`k+1`種類の値と個数だけを保持する。

## 頻出事故

- 同じ値を複数含む列には使えない。
- `k`に対して線形なため、`k`が大きい問題には適さない。
- 部分配列の長さは$r-l+1$なので、式から引く値は$r-l$である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_subarrays_range_excess_at_most`

```cpp
template<std::integral Integer> long long count_subarrays_range_excess_at_most( const std::vector<Integer>& values, int k )
```

O(n(k+1)log n)時間・O(n(k+1))領域。相異なる整数列でmax-min-(長さ-1)<=kとなる部分配列数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC248 Ex - Beautiful Subsequences](https://atcoder.jp/contests/abc248/tasks/abc248_h)では、
順列と$0\le K\le3$をそのまま渡す。
`verify/atcoder_abc248_h.cpp`に提出用コードを用意し、公式sampleを確認している。
