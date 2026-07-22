---
title: "permutationthreshold"
documentation_of: //cp/dp/permutationthreshold.hpp
---

# `permutationthreshold`

<!-- API REFERENCE: COLLAPSED -->

## 概要

順列の各位置に下限を置き、「割り当てた値が下限以上」という条件をちょうど$k$個満たす順列数を、すべての$k$について求めます。
二次元のHakone Ekiden DPを、多項式積・多点評価・二項反転へ変換して高速化します。

## 厳密な定義

長さ$n$の整数列$B$に対し、$0,1,\ldots,n-1$の順列$p$を考えます。
各$0\leq k\leq n$について、次の値を返します。

$$
F_k=\#\left\{p\in S_n\mid
\#\{i\mid p_i\geq B_i\}=k\right\}
$$

$0\leq B_i\leq n$を仮定します。$B_i=n$なら、その位置の条件は決して成立しません。

内部では$B$を降順に並べ、$C_i=n-B_i-i$として

$$
h(x)=\prod_{i=0}^{n-1}(x+C_i)
$$

を構築します。$h(0),\ldots,h(n)$の多点評価から条件を明示した部分集合の個数を求め、上側二項反転で$F_k$を復元します。

## Include

```cpp
#include "dp/permutationthreshold.hpp"
```

## 最短の使用例

```cpp
auto counts = permutation_threshold_counts<998244353>(thresholds);
// counts[k]: 条件をちょうどk個満たす順列数
```

## APIの選び方

| 欲しいもの | 取得方法 |
| --- | --- |
| 条件をちょうど$k$個満たす順列数 | `counts[k]` |
| 条件数が偶数の順列数 | `counts[0] + counts[2] + ...` |
| 条件数の分布全体 | 戻り値全体 |

## 注意点・計算量

- 閾値列の順番は答えに影響しないため、関数内で降順にsortします。
- 法`mod`はNTTを含む既存FPS演算が利用でき、$n<mod$である必要があります。
- 計算量は$O(M(n)\log^2n)$、空間は$O(n\log n)$です。
- 順列の値は0-indexedの$0,\ldots,n-1$です。1-indexed順位を使う場合は閾値を変換してください。
- 一つの$k$だけを小さい$n$で求める場合は、通常の$O(n^2)$ DPの方が軽いことがあります。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `permutation_threshold_counts`

```cpp
template <int mod> std::vector<staticmodint<mod>> permutation_threshold_counts( std::vector<int> thresholds )
```

O(M(n)log^2 n)。順列pでp[i]>=threshold[i]をちょうどk個満たす個数を全k返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC272 Ex - Flipping Coins 2](https://atcoder.jp/contests/abc272/tasks/abc272_h)では、長さ$A_i+1$の操作が対象コインを裏返す条件を$B_i=n-1-A_i$と表せます。

```cpp
for (int i = 0; i < n; ++i) thresholds[i] = n - 1 - a[i];
auto counts = permutation_threshold_counts<998244353>(thresholds);
modint998244353 even = 0;
for (int k = 0; k <= n; k += 2) even += counts[k];
```

## 検証

$n\leq8$のランダムな閾値列について全順列を列挙し、条件成立数ごとの個数を比較しています。
