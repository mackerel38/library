---
title: "min-plus・max-plus畳み込み"
documentation_of: //cp/math/tropicalconvolution.hpp
---

# min-plus・max-plus畳み込み

<!-- API REFERENCE: COLLAPSED -->

## 概要

二つのDP表を、「合計で何個選んだか」を保ったまま一つにまとめる。
費用を最小化するときはmin-plus畳み込み、価値を最大化するときはmax-plus畳み込みを使う。

## 厳密な定義

長さ$n$の数列$A$と長さ$m$の数列$B$に対し、min-plus畳み込みは
長さ$n+m-1$の数列$C$を

$$
C_k = \min_{i+j=k}(A_i+B_j)
$$

で定義する。ただし$0 \le i < n$, $0 \le j < m$を満たす範囲で最小値を取る。
max-plus畳み込みは

$$
C_k = \max_{i+j=k}(A_i+B_j)
$$

で定義する。
どちらかの入力が空なら空列を返す。

`A[i]-A[i-1]`が単調非減少なら`A`は離散凸、単調非増加なら離散凹である。
両列が離散凸なmin-plus畳み込みと、両列が離散凹なmax-plus畳み込みは、差分列を併合して
`O(n+m)`で計算できる。

## Include

```cpp
#include "math/tropicalconvolution.hpp"
```

## 最短の使用例

```cpp
vector<long long> a = {0, 3, 8};
vector<long long> b = {0, 2, 7};

auto c = poe::minplus_convolution(a, b);
// c[k] = min_{i+j=k}(a[i] + b[j])
```

## APIの選び方

| 条件 | 出力 | 関数 | 計算量 |
| --- | --- | --- | --- |
| $A, B$は一般の数列 | $C_k=\min_{i+j=k}(A_i+B_j)$ | `minplus_convolution` | `O(nm)` |
| $A, B$は上に凸な数列（差分が単調非減少） | $C_k=\min_{i+j=k}(A_i+B_j)$ | `minplus_convolution_convex` | `O(n+m)` |
| $A, B$は一般の数列 | $C_k=\max_{i+j=k}(A_i+B_j)$ | `maxplus_convolution` | `O(nm)` |
| $A, B$は下に凸な数列（差分が単調非増加） | $C_k=\max_{i+j=k}(A_i+B_j)$ | `maxplus_convolution_concave` | `O(n+m)` |

## 注意点

高速版は差分列の単調性が必要である。片方だけが凹凸、穴のある到達不能状態、添字制約が複雑な場合は
高速版をそのまま使えない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `maxplus_convolution`

```cpp
template <class T> std::vector<T> maxplus_convolution(const std::vector<T>& left, const std::vector<T>& right)
```

O(nm)。maxplus_convolution(a,b)[k]=max_{i+j=k}(a[i]+b[j])を返す。

### `maxplus_convolution_concave`

```cpp
template <class T> std::vector<T> maxplus_convolution_concave(const std::vector<T>& left, const std::vector<T>& right)
```

O(n+m)。両列が離散凹ならmax-plus畳み込みを傾きのmergeで返す。

### `minplus_convolution`

```cpp
template <class T> std::vector<T> minplus_convolution(const std::vector<T>& left, const std::vector<T>& right)
```

O(nm)。minplus_convolution(a,b)[k]=min_{i+j=k}(a[i]+b[j])を返す。

### `minplus_convolution_convex`

```cpp
template <class T> std::vector<T> minplus_convolution_convex(const std::vector<T>& left, const std::vector<T>& right)
```

O(n+m)。両列が離散凸ならmin-plus畳み込みを傾きのmergeで返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[NDPC O - ゲーム](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_o)の部分点はmax-plus畳み込みへの
帰着が公式テーマとして示されている。満点ではAliens trickも必要。
