---
title: "木上パスの畳み込み集計"
documentation_of: //cp/tree/pathfold.hpp
---

# 木上パスの畳み込み集計

## 概要

木の各頂点に値があるとき、全ての頂点対についてパス上の値を `gcd`、`min`、bitwise AND
などでまとめる。同じ結果になったパスを集約し、パスの本数と長さの総和を返す。

## 厳密な定義

頂点 $v$ に値 $A_v$ があり、結合的かつ可換な二項演算 $\circ$ が与えられるとする。
相異なる二頂点 $s,t$ の単純パス上の頂点を $p_1,p_2,\ldots,p_k$ とするとき、

$$
X(s,t)=A_{p_1}\circ A_{p_2}\circ\cdots\circ A_{p_k}
$$

と定める。各値 $x$ について、$X(s,t)=x$ となる unordered pair $\{s,t\}$ の個数と、
そのようなパスの頂点数 $k$ の総和を返す。端点が同じ組は含めない。

## Include

```cpp
#include "tree/pathfold.hpp"
```

## 使い方

```cpp
auto stats = tree_path_fold_statistics(
    graph, values,
    [](int left, int right) { return std::gcd(left, right); });

for (const auto& [gcd_value, stat] : stats) {
    // stat.count: パス上のgcdがgcd_valueとなる頂点対の個数
    // stat.length_sum: それらのパスに含まれる頂点数の総和
}
```

`graph` は連結な無向木、`values.size()` は頂点数と一致する必要がある。`combine` は結合的かつ
可換で、同じ入力には常に同じ値を返す必要がある。結果のキーには `std::map` で比較できる型を使う。
独自の順序を使う場合は第4引数に比較関数を渡せる。

部分木を併合するときの状態数を $a,b$ とすると、その併合は $O(ab)$ 回の `combine` と
map操作を行う。全体の計算量は

$$
O\left(\sum_{\mathrm{merge}} ab(\operatorname{cost}(\circ)+\log S)\right)
$$

で、$S$ は一つの部分木が持つ畳み込み値の種類数である。領域計算量は、保持中の状態数の総和に
比例する。gcd、min、bitwise AND のようにパスを伸ばすと値の種類が急速に減る演算に適する。

## 頻出事故

- 頂点対は unordered で、単一頂点のパスは集計しない。
- `length_sum` は辺数ではなく、端点を含む頂点数の総和である。
- 加算や文字列結合のように状態数が大きく増える演算では計算量が悪化する。
- `count` と `length_sum` は `long long` なので、木の大きさに対する総和が範囲内か確認する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `treepathfoldstat`

```cpp
struct treepathfoldstat
```

O(1)で参照できる、同じ畳み込み値を持つ木上パスの本数と頂点数で測った長さの総和。

### `tree_path_fold_statistics`

```cpp
template<undirected_graph_type Graph, class Value, class Combine, class Compare = std::less<Value>> std::map<Value, treepathfoldstat, Compare> tree_path_fold_statistics( const Graph& graph, const std::vector<Value>& values, Combine combine, Compare compare = Compare())
```

O(Σ ab(cost(combine)+log S))。全ての相異なる二頂点間パスを畳み込み結果ごとに集計する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC248 G - GCD cost on the tree](https://atcoder.jp/contests/abc248/tasks/abc248_g)では、
`combine` に `std::gcd` を渡す。各 `gcd_value * stat.length_sum` の総和が答えになる。
`verify/atcoder_abc248_g.cpp` に提出用コードを用意している。
