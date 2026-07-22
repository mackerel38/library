---
title: "sternbrocot"
documentation_of: //cp/math/sternbrocot.hpp
---

# `sternbrocot`

<!-- API REFERENCE: COLLAPSED -->

## 概要

既約分数へのStern–Brocot木上の移動を、同じ向きに連続する回数で圧縮して求めます。
また、多数の分数をすべて含む最小限の仮想木を、深さが$10^9$以上でも明示的に展開せず構築します。

## 厳密な定義

Stern–Brocot木は境界$0/1,1/0$から始め、区間$[a/b,c/d]$の中点として媒介分数

$$
\frac{a+c}{b+d}
$$

を置きます。目的の分数が媒介分数より小さければ左、大きければ右へ進みます。

`stern_brocot_path(p, q)`は、既約分数$p/q$へ到達する`L/R`列を同方向runとして返します。
$0/1$と$1/0$は最初から存在する境界なので、`depth = -1`です。正の有限分数では$1/1$を深さ$0$とします。

`stern_brocot_virtual_tree(points)`は、各`points[i]`をleaf `i`に対応させた圧縮木を返します。
辺`parent -> child`がまとめているStern–Brocot頂点数は

$$
\mathrm{depth}[child]-\mathrm{depth}[parent]
$$

です。

## Include

```cpp
#include "math/sternbrocot.hpp"
```

## 最短の使用例

```cpp
auto path = stern_brocot_path(numerator, denominator);
for (auto [right, count] : path.runs) {
    // right=false: Lをcount回、right=true: Rをcount回
}

auto tree = stern_brocot_virtual_tree(points);
int leaf_of_point_i = i;
```

## APIの選び方

| 欲しいもの | API |
| --- | --- |
| 一分数への圧縮L/R path | `stern_brocot_path(p, q)` |
| pathの展開後の辺数 | `path.depth` |
| 複数分数だけを含む圧縮木 | `stern_brocot_virtual_tree(points)` |
| 元の分数$i$のleaf | 頂点`i` |
| 圧縮辺が表す頂点数 | `depth[child] - depth[parent]` |

## 注意点・計算量

- 入力は$p,q\geq0$かつ$\gcd(p,q)=1$である必要があります。
- `(0, 0)`や非既約分数は受け取りません。
- 一分数のpathは$O(\log\max(p,q))$個のrunになります。
- $n$分数の仮想木は$O(n\log n+n\log V)$時間、$O(n\log V)$補助空間です。$V=\max(p_i,q_i)$です。
- `depth`は展開後の深さなので`long long`です。仮想木の頂点数は$O(n)$です。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sternbrocotpath`

```cpp
struct sternbrocotpath
```

Stern--Brocot木上のL/R移動を同方向runごとに持つ。

### `stern_brocot_path`

```cpp
inline sternbrocotpath stern_brocot_path(long long numerator, long long denominator)
```

O(log max(p,q))。既約なp/qへのStern--Brocot pathをrun-lengthで返す。

### `sternbrocotvirtualtree`

```cpp
struct sternbrocotvirtualtree
```

指定した既約分数だけを含む圧縮Stern--Brocot木。leaf iはpoints[i]に対応する。

### `stern_brocot_virtual_tree`

```cpp
inline sternbrocotvirtualtree stern_brocot_virtual_tree( const std::vector<std::pair<long long, long long>>& points )
```

O(n log n+n log V)。既約分数pointsを含むStern--Brocot仮想木を返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC273 Ex - Inv(0,1)ving Insert(1,0)n](https://atcoder.jp/contests/abc273/tasks/abc273_h)の挿入操作はStern–Brocot木の構築と一致します。非既約な点で入力を分割し、各区間の既約分数だけを含む仮想木を構築できます。

## 検証

$p,q\leq20$の既約分数についてL/R列を展開して実際に媒介分数を更新し、目的の$p/q$へ到達することを確認しています。
さらにランダムな分数集合1,000件について、仮想木の根パス長と全leaf対のLCA深さを展開したpathと比較しています。
