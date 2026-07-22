---
title: "antichain"
documentation_of: //cp/tree/antichain.hpp
---

# `antichain`

<!-- API REFERENCE: COLLAPSED -->

## 概要

根付き木から、互いに祖先・子孫の関係にない頂点集合を大きさ別に数えます。
通常の木DPをそのまま多項式で実装すると二乗時間になる木でも、Static Top TreeとNTTで高速に処理します。

## 厳密な定義

根付き木$T=(V,E)$で、頂点集合$S\subseteq V$がantichainであるとは、異なる任意の$u,v\in S$について
$u$が$v$の祖先でなく、$v$も$u$の祖先でないことです。

`tree_antichain_counts<mod>(graph, root)`は$n=\lvert V\rvert$として長さ$n+1$の列$C$を返します。

$$
C_k=\#\{S\subseteq V\mid |S|=k,\ S\text{ is an antichain}\}\pmod{mod}
$$

$C_0=1$で、空集合も数えます。

部分木$v$の母関数を$F_v(x)$とすると、定義は

$$
F_v(x)=x+\prod_{u:\,u\text{ is a child of }v}F_u(x)
$$

です。この木DPをStatic Top Treeの深さ$O(\log n)$のmerge木へ載せ、多項式積をNTTで行います。

## Include

```cpp
#include "tree/antichain.hpp"
```

## 最短の使用例

```cpp
undirected_graph<> graph(n);
// graphへn-1辺を追加
auto count = tree_antichain_counts(graph, root);
cout << count[k] << '\n';
```

## API

```cpp
template <int modulus = 998244353, undirected_graph_type Graph>
std::vector<staticmodint<modulus>> tree_antichain_counts(
    const Graph& graph,
    int root = 0
);
```

- `graph`: $n$頂点$n-1$辺の連結な`undirected_graph`です。重みは使いません。
- `root`: 祖先関係を定める0-indexedの根です。既定値は0です。
- template引数`modulus`: 係数を数える法です。NTTを直接または三つのNTT素数による畳み込みで実行できる必要があります。
- 戻り値: 長さ$n+1$で、添字$k$が大きさ$k$のantichain数です。空木では`{1}`を返します。
- 計算量: 多項式積を$M(n)$として$O(M(n)\log n)$時間、$O(n\log n)$領域です。NTTでは$O(n\log^2 n)$時間です。

## 注意点

- 祖先関係は`root`で変わるため、無根木の不変量ではありません。
- 空集合を含むので、非空集合だけ必要なら添字1から使います。
- 最大antichainサイズより大きい係数も、戻り値の長さを$n+1$に揃えるため0で埋めます。
- 一つの頂点の重み更新を繰り返すAPIではありません。動的木DPは`tree/statictoptree.hpp`を直接使います。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `tree_antichain_counts`

```cpp
template <int modulus = 998244353, undirected_graph_type Graph> std::vector<staticmodint<modulus>> tree_antichain_counts( const Graph& graph, int root = 0 )
```

O(M(n) log n)。根付き木で祖先・子孫を同時に含まないk頂点集合の個数をk=0..nで返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC269 Ex - Antichain](https://atcoder.jp/contests/abc269/tasks/abc269_h)では、根を0として返り値の添字1から$n$をそのまま出力できます。

## 検証

12頂点以下のランダム木とランダムな根について全頂点部分集合を列挙し、祖先関係を直接調べる愚直解と比較しています。公式4サンプルもverifyコードで確認しています。
