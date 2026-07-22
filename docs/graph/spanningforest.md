---
title: "重み付き全域森の辺数別計数"
documentation_of: //cp/graph/spanningforest.hpp
---

# 重み付き全域森の辺数別計数

## 概要

頂点数が小さい重み付き無向グラフについて、全頂点を含む森を辺数ごとに数える。
辺の重みを平行辺の本数とみなせば、多重グラフから森を選ぶ方法数も得られる。

## 厳密な定義

$n$頂点の対称行列$W$を受け取る。$W_{i,i}=0$とし、無向辺$\{i,j\}$の重みを$W_{i,j}$とする。
辺集合$F$の重みを

$$
w(F)=\prod_{\{i,j\}\in F}W_{i,j}
$$

と定める。返り値`answer`は長さ$n$で、$0\leq k<n$について

$$
\mathit{answer}_k=\sum_{\substack{F:\ |F|=k\\(V,F)\text{ is a forest}}}w(F)
$$

を満たす。森は全$n$頂点を含み、孤立頂点を許す。

各頂点部分集合$S$の全域木重みを行列木定理で求める。その後、最小番号頂点を含む連結成分を選ぶ
集合分割DPにより、連結成分数別、すなわち辺数別の森重みへ変換する。

## Include

```cpp
#include "graph/spanningforest.hpp"
```

## 使い方

```cpp
using mint = poe::modint998244353;
poe::matrix<mint> weight({
    {0, 2, 0},
    {2, 0, 3},
    {0, 3, 0}
});
auto answer = poe::spanning_forest_counts(weight);
// answer = {1, 5, 6}
```

## `spanning_forest_counts`

```cpp
template<class T>
vector<T> spanning_forest_counts(const matrix<T>& edge_weight);
```

`edge_weight`は正方対称行列で、対角成分が$0$でなければならない。`T`には体としての四則演算と比較が必要で、
通常は`modint`を使う。入力を変更しない。$n=0$では、空の森一つを表す`{1}`を返す。

時間計算量は$O(2^n n^3+3^n n)$、領域計算量は$O(2^n n)$。
指数計算量なので、頂点数が20前後以下の問題を対象とする。

## 使用例

[AtCoder ABC253 Ex - We Love Forest](https://atcoder.jp/contests/abc253/tasks/abc253_h)では、
`edge_weight[u][v]`を入力に現れる平行辺本数とする。`answer[k]`へ$k!$を掛けると、$k$回の辺選択が
森になる順序付き列の個数になる。さらに全列数$M^k$で割れば求める確率を得られる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `spanning_forest_counts`

```cpp
template<class T> std::vector<T> spanning_forest_counts(const matrix<T>& edge_weight)
```

O(2^n n^3 + 3^n n)時間・O(2^n n)領域。重み付き無向グラフの森の重み和を辺数別に返す。

<!-- END AUTO-GENERATED API REFERENCE -->
