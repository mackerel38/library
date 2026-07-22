---
title: "完全多分木となる誘導部分木のprefix計数"
documentation_of: //cp/tree/perfectsubtree.hpp
---

# 完全多分木となる誘導部分木のprefix計数

## 概要

親が必ず子より先に現れる根付き木へ、頂点を番号順に追加する。各時点で、根を含む頂点集合が
完全$r$分木となる選び方を数える。二分木だけでなく、各内部頂点が同じ個数の子を持つ多分木に使える。

## 厳密な定義

`parent[0] = -1`で、各$v>0$について$0\le parent[v]<v$とする。
prefix $V_k=\{0,1,\ldots,k\}$ごとに、次をすべて満たす集合$S\subseteq V_k$の個数を返す。

- $0\in S$。
- $S$が誘導する部分グラフは連結である。
- $S$内の各頂点は、$S$内に子を0個またはちょうど$r$個持つ。
- 子を0個持つ全頂点は、根0から同じ深さにある。

返り値の`answer[k]`がprefix $V_k$に対する個数である。

## Include

```cpp
#include "tree/perfectsubtree.hpp"
```

## 使い方

```cpp
vector<int> parent = {-1, 0, 0, 1, 1, 2, 2};

// 完全二分木となる選び方を、各prefixについて数える。
auto binary = poe::perfect_ary_subtree_prefix_counts<998244353>(parent);

// 各内部頂点が3子を持つ完全三分木ならarity=3。
auto ternary = poe::perfect_ary_subtree_prefix_counts<998244353>(parent, 3);
```

`arity >= 2`が必要である。法はtemplate引数で指定する。
頂点番号が親子順になっていない場合は、先に親が子より小さくなるよう番号を付け直す。

## 計算量

頂点数を$n$、分岐数を$r$として、時間計算量は$O(nr^2\log_r n)$、
領域計算量は$O(nr\log_r n)$。

高さ$h$の完全$r$分木は$1+r+\cdots+r^h$頂点を必要とする。そのため、$n$頂点の入力で
保持すべき高さは$O(\log_r n)$だけでよい。各頂点追加では祖先方向へ差分を伝播し、子のDP値に
対する$r$次基本対称式を更新する。

## 使用例

[AtCoder ABC264 Ex - Perfect Binary Tree](https://atcoder.jp/contests/abc264/tasks/abc264_h)では、
入力の親を0-indexedへ直し、既定の`arity=2`で呼んだ返り値を順に出力できる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `perfect_ary_subtree_prefix_counts`

```cpp
template <int mod> std::vector<staticmodint<mod>> perfect_ary_subtree_prefix_counts( const std::vector<int>& parent, int arity = 2 )
```

O(n r^2 log_r n)時間・O(n r log_r n)領域。
parent順に頂点を追加した各prefixで、根0の完全r分木となる誘導部分木数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
