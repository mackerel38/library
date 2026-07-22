---
title: "頂点彩色付き無ラベルグラフの数え上げ"
documentation_of: //cp/graph/unlabeledcount.hpp
---

# 頂点彩色付き無ラベルグラフの数え上げ

## 概要

頂点番号の付け替えで一致する単純無向グラフを同じものとみなし、異なるグラフを数えます。
各頂点には色を付け、指定したすべての色が少なくとも一度現れることも要求できます。

## 厳密な定義

$n$頂点の単純無向グラフ $G$ と全射彩色 $c:V(G)\to\{0,1,\ldots,k-1\}$ の組を考えます。
彩色を保つグラフ同型で移り合う組を同一視した同値類の個数を返します。

置換のcycle長が $a,b$ の二つの頂点cycle間にある辺は $\gcd(a,b)$ 個の軌道に分かれます。
同じ長さ $a$ の一つのcycle内部にある辺は $\lfloor a/2\rfloor$ 個の軌道に分かれます。
この辺軌道数を $e$ とすると、置換で不変なグラフは $2^e$ 個です。彩色は頂点cycleごとに一定であり、
cycle集合から$k$色への全射数を掛けてBurnsideの補題を適用します。

## Include

```cpp
#include "graph/unlabeledcount.hpp"
```

## API

```cpp
template<class mint>
mint count_surjectively_colored_unlabeled_graphs(
    int vertex_count,
    int color_count
);
```

- `0 <= color_count <= vertex_count`が必要です。
- $p(n)$を$n$の分割数として、計算量は $O(p(n)n^2+nk)$ です。
- 追加メモリは $O(n+k)$ です。
- `mint`の法の下で $1,2,\ldots,n$が可逆である必要があります。
- 無彩色の$n$頂点グラフ数は`color_count=1`で求められます。

## 使用例

```cpp
using mint = staticmodint<998244353>;
mint answer = count_surjectively_colored_unlabeled_graphs<mint>(3, 2);
// answer == 12
```

[ABC284 Ex - Count Unlabeled Graphs](https://atcoder.jp/contests/abc284/tasks/abc284_h)では、入力の
`N`,`K`をそのまま渡します。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_surjectively_colored_unlabeled_graphs`

```cpp
template<class mint> mint count_surjectively_colored_unlabeled_graphs(int vertex_count, int color_count)
```

O(p(n)・n^2+n k)。全k色を使う頂点彩色付きn頂点無ラベル単純無向グラフ数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
