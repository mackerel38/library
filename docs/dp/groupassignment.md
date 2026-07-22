---
title: "種類別人数を指定する二群割当DP"
documentation_of: //cp/dp/groupassignment.hpp
---

# 種類別人数を指定する二群割当DP

## 概要

順番が固定された要素を第一群・第二群のどちらかへ割り当てる。
各種類について第一群へ入れる個数を指定し、それを満たす割当ての追加費用合計を最小化する。

「ここまでに各群へ何種類ずつ置いたか」で追加費用が決まる、quota付きの配置DPに使える。
要素の並び替えまで行う問題や、三群以上へ分ける問題には使えない。

## 厳密な定義

要素$i$の種類を$c_i\in\{0,\ldots,K-1\}$とする。各要素へ群$g_i\in\{0,1\}$を割り当て、
各種類$c$について
$$
\left|\{i\mid c_i=c,\ g_i=0\}\right|=t_c
$$
を満たす割当てのうち、追加費用の総和が最小のものを返す。

要素$i$の追加費用はcallback `cost(i, group, first_count, second_count)`で与える。
二つのcountは要素$i$を置く直前、すなわち添字$i$未満の要素だけを数えた種類別人数である。

## Include

```cpp
#include "dp/groupassignment.hpp"
```

## 使用例

```cpp
std::vector<int> category = {0, 1, 0, 1};
std::array<int, 2> first_target = {1, 1};
long long answer = poe::minimum_two_group_assignment<long long>(
    category,
    first_target,
    [&](int index, int group, const auto& first, const auto& second) {
        const auto& count = group == 0 ? first : second;
        return weight[index] * count[1 - category[index]];
    },
    (1LL << 62)
);
```

## 前提と計算量

種類数を$K$、要素数を$n$、第一群の種類別人数を$t_c$、
$S=\prod_{c=0}^{K-1}(t_c+1)$とする。

- 時間計算量: $O((n+K)S)$
- 空間計算量: $O(KS)$

`T`には零初期化、加算、比較、`infinity`との等値比較が必要である。
到達可能な費用に追加費用を加えても`T`の範囲を超えない`infinity`を渡す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimum_two_group_assignment`

```cpp
template <class T, std::size_t Categories, class Cost> T minimum_two_group_assignment( const std::vector<int>& category, const std::array<int, Categories>& first_target, Cost cost, T infinity )
```

O((n+k) prod(target[c]+1))。順番固定のk種類の要素を、種類別人数を指定して二群へ最小費用で分ける。
cost(index, group, first_count, second_count)はindexを置く直前の人数から追加費用を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC290 Ex - Bow Meow Optimization](https://atcoder.jp/contests/abc290/tasks/abc290_h)では、
係数順に処理する犬と猫を二つの半列へ分けるDPに使う。犬・猫の各半数を第一群へ置くquotaと、
同じ群へ先に置いた反対種類の個数から追加費用を計算するcallbackを渡す。

`verify/atcoder_abc290_h.cpp`に提出用コードを収録している。
