---
title: "min-plus・max-plus畳み込み"
documentation_of: //cp/math/tropicalconvolution.hpp
---

# min-plus・max-plus畳み込み

- Header: `cp/math/tropicalconvolution.hpp`
- Symbol: `poe::maxplus_convolution`, `poe::maxplus_convolution_concave`, `poe::minplus_convolution`, `poe::minplus_convolution_convex`
- Status: experimental

## どんな問題に使えるか

二つのDPを「選んだ個数の和」を添字として併合し、価値の最大または費用の最小を取る問題に使う。
一般列版は`O(nm)`だが、両列が離散凹なmax-plus、離散凸なmin-plusなら傾きをmergeして`O(n+m)`になる。

高速版は差分列の単調性が必要である。片方だけが凹凸、穴のある到達不能状態、添字制約が複雑な場合は
高速版をそのまま使えない。

## API・計算量

```cpp
#include "math/tropicalconvolution.hpp"

auto merged = poe::maxplus_convolution_concave(left, right);
```

一般版は`O(nm)`、凹凸列の高速版は`O(n+m)`時間・領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

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

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[NDPC O - ゲーム](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_o)の部分点はmax-plus畳み込みへの
帰着が公式テーマとして示されている。満点ではAliens trickも必要。judge ACは未確認。
