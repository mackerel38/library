---
title: "色ごとの選択XOR"
documentation_of: //cp/algorithm/groupxor.hpp
---

# 色ごとの選択XOR

## 概要

いくつかの候補グループから一つずつ整数を選び、選んだ値すべてのXORを取る。
全選び方のXORを大きい順に並べたときの、指定順位の値を求める。

## 厳密な定義

`groups[c]`から要素 $x_c$ を一つずつ選ぶ全組合せについて、

$$
x_0\mathbin{\mathrm{xor}}x_1\mathbin{\mathrm{xor}}\cdots
\mathbin{\mathrm{xor}}x_{C-1}
$$

を重複込みで大きい順に並べ、その $k$ 番目を返す。$k$ は1-indexedである。
同じ値を作る異なる選び方は、別の組合せとして数える。

## Include

```cpp
#include "algorithm/groupxor.hpp"
```

```cpp
auto answer = kth_largest_group_xor<unsigned long long, 60>(groups, k);
```

各グループは空でない必要がある。`Bits`は値を表すbit数で、全要素が $2^{Bits}$ 未満であること。
大きさ2以上のグループ数を $g$ とする。グループ集合を半分ずつ列挙して組合せ数が最も均衡する
二群へ分け、それぞれのXORを列挙する。二群の選び方の数を $L,R$ とすると、時間計算量は
$O(2^{g/2}+(L+R)Bits)$、領域計算量も $O(2^{g/2}+(L+R)Bits)$ である。

## 頻出事故

- `k`は0-indexedではなく1-indexedである。
- 同じ整数が同じグループに複数回現れても、候補を勝手に重複排除してはいけない。
- 全組合せ数と`k`は`unsigned long long`に収まる必要がある。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `kth_largest_group_xor`

```cpp
template<std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits> UInt kth_largest_group_xor( const std::vector<std::vector<UInt>>& groups, unsigned long long k )
```

O(2^(g/2)+(L+R) bits)時間・O(2^(g/2)+(L+R) bits)領域。各groupから一つ選んだXORのk番目に大きい値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC252 Ex - K-th beautiful Necklace](https://atcoder.jp/contests/abc252/tasks/abc252_h)では、
色ごとに宝石の美しさをまとめ、そのまま本関数へ渡して$K$番目に大きいXORを求める。
`verify/atcoder_abc252_h.cpp`に提出用コードを用意し、公式sampleを確認している。
