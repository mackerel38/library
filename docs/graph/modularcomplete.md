---
title: "剰余和重みの完全有向グラフ"
documentation_of: //cp/graph/modularcomplete.hpp
---

# 剰余和重みの完全有向グラフ

## 概要

全頂点対の辺を明示すると二乗本になる完全有向グラフのうち、辺重みが
「出発頂点の値と到着頂点の値の和を法で割った余り」である場合の単一始点最短路を求める。

## 厳密な定義

$n$頂点の有向グラフについて、$i\ne j$の辺$i\to j$の重みを

$$
w(i,j)=(A_i+B_j)\bmod M
$$

とする。`modular_complete_distances(A, B, M, start)`は`start`から各頂点への最短距離を返す。
実装上は自己辺も許した補助グラフへ変換するが、重みが非負なので最短距離は変わらない。

## Include

```cpp
#include "graph/modularcomplete.hpp"
```

## API

```cpp
template <std::signed_integral T>
std::vector<T> modular_complete_distances(
    const std::vector<T>& outgoing,
    const std::vector<T>& incoming,
    T modulus,
    int start = 0
);
```

- `outgoing[i]`: $A_i$。
- `incoming[i]`: $B_i$。二列の長さは同じ正の値$n$。
- `modulus`: 正の法$M$。全要素は$[0,M)$に正規化済みでなければならない。
- `start`: 0-indexedの始点。既定値は`0`。
- 返り値: 長さ$n$の最短距離列。完全グラフなので全頂点へ到達できる。
- 中間値、辺重み、距離が`T`の範囲を超えないことが必要。
- $O(n\log n)$時間・$O(n)$辺・$O(n)$領域。

```cpp
auto distance = poe::modular_complete_distances(a, b, modulus);
long long answer = distance[target];
```

## 仕組み

$B_j$を昇順に並べ、剰余$M$の円周上で隣へ進む補助辺を張る。各頂点$i$からは
$-A_i\pmod M$以上で最初の$B_j$へ一本だけ辺を張れば、その後の円周移動で全ての到着先を表せる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `modular_complete_distances`

```cpp
template <std::signed_integral T> std::vector<T> modular_complete_distances( const std::vector<T>& outgoing, const std::vector<T>& incoming, T modulus, int start = 0 )
```

O(n log n)時間・O(n)辺。辺重み(out[i]+in[j]) mod modulusの完全有向グラフで最短距離を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC232 G - Modulo Shortest Path](https://atcoder.jp/contests/abc232/tasks/abc232_g)では、
入力の二列と$M$をそのまま渡し、返り値の`n-1`番目を出力する。
