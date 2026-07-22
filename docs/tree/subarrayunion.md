---
title: "subarrayunion"
documentation_of: //cp/tree/subarrayunion.hpp
---

# `subarrayunion`

<!-- API REFERENCE: COLLAPSED -->

## 概要

頂点列のすべての連続部分列について、選ばれた頂点から根までのパスの和集合に含まれる重みを合計します。
部分列を一つずつ処理せず、各木頂点が何個の部分列から使われるかをsmall-to-largeで数えます。

## 厳密な定義

根付き木の各頂点$v$に重み$w_v$があり、頂点列$A_0,\ldots,A_{m-1}$が与えられます。
半開部分列$A[l:r]$が使う頂点集合を

$$
U_{l,r}=\bigcup_{l\leq i<r}\operatorname{path}(A_i,\mathrm{root})
$$

とします。次の値を返します。

$$
\sum_{0\leq l<r\leq m}\ \sum_{v\in U_{l,r}}w_v
$$

頂点$v$の部分木に属する要素の位置集合を$P_v$とすると、$v$を使わない部分列は$P_v$に含まれる位置の間のgap内だけにあります。

## Include

```cpp
#include "tree/subarrayunion.hpp"
```

## 最短の使用例

```cpp
long long answer = sum_subarray_root_path_unions(
    parent, sequence_vertices, weight);
```

## APIの選び方

| 引数 | 内容 |
| --- | --- |
| `parent[v]` | 根だけ`-1`、他は親頂点 |
| `vertices[i]` | 列の$i$番目が選ぶ木頂点 |
| `weight[v]` | 和集合へ一度だけ加える重み |
| 戻り値 | 全非空部分列に対する根パス和集合重みの総和 |

## 注意点・計算量

- 木はちょうど一つの根を持つ必要があります。
- `vertices`には同じ頂点が複数回現れても構いません。
- 頂点数$n$、列長$q$に対して$O((n+q)\log^2q)$時間、$O(n+q)$空間です。
- 型`T`には加算、乗算、非負整数からの構築が必要です。`long long`や`modint`を使えます。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sum_subarray_root_path_unions`

```cpp
template <class T> T sum_subarray_root_path_unions( const std::vector<int>& parent, const std::vector<int>& vertices, const std::vector<T>& weight )
```

O((n+q)log^2 q)。各部分列が選ぶ根path和集合の頂点重み総和を全て足す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC273 Ex](https://atcoder.jp/contests/abc273/tasks/abc273_h)では、Stern–Brocot仮想木の圧縮辺長を子頂点の重みにします。

```cpp
for (int v = 0; v < tree.parent.size(); ++v) {
    if (tree.parent[v] != -1) {
        weight[v] = tree.depth[v] - tree.depth[tree.parent[v]];
    }
}
answer += sum_subarray_root_path_unions(tree.parent, leaves, weight);
```

## 検証

ランダムな小さい根付き木5,000件について、全部分列の根パスを実際に列挙する実装と比較しています。
