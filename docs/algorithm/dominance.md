---
title: "三次元 dominance chain"
documentation_of: //cp/algorithm/dominance.hpp
---

# 三次元 dominance chain

## 概要

三つの座標がすべて減らないように点を並べ、選んだ点の重み総和を最大化する。
最大値だけでなく、各点で終わる最大値と実際に選ぶ点の入力添字も返す。
三属性の順序DPや、時刻・位置を座標変換した移動DPに使える。

## 厳密な定義

点$p_i=(x_i,y_i,z_i)$と非負重み$w_i$が与えられる。相異なる入力添字列
$i_1,\ldots,i_k$で、すべての$j<k$について

$$
x_{i_j}\le x_{i_{j+1}},\qquad
y_{i_j}\le y_{i_{j+1}},\qquad
z_{i_j}\le z_{i_{j+1}}
$$

を満たすもののうち、$\sum_{j=1}^k w_{i_j}$が最大の列を求める。
入力配列上の順序は制約に含めない。空列も許し、その重みは0とする。

## Include

```cpp
#include "algorithm/dominance.hpp"
```

## 使い方

```cpp
vector<poe::weightedpoint3d<int, long long>> points = {
    {1, 1, 3, 4},
    {2, 2, 2, 10},
    {3, 3, 3, 5}
};

auto result = poe::maximum_weight_dominance_chain_3d(points);
cout << result.weight << '\n'; // 15
for (int index : result.indices) cout << index << ' '; // 1 2
```

`ending[i]`は点$i$を末尾に選ぶchainの最大重み、`parent[i]`はその直前の入力添字である。
直前がなければ`parent[i] == -1`となる。`indices`は先頭から末尾の順で復元済みである。

## 計算量

点数を$n$として、時間計算量は$O(n\log^2 n)$、領域計算量は$O(n\log n)$。
内部では第一座標で平面走査し、残る二座標を疎な`fenwicktree2d`で処理する。

## 使用例

[AtCoder ABC266 Ex - Snuke Panic (2D)](https://atcoder.jp/contests/abc266/tasks/abc266_h)では、
イベント$(t,x,y)$を

$$
(t-x-y,\ t+x-y,\ y)
$$

へ変換する。三座標がすべて非負なイベントだけを残せば、移動可能性が三次元dominanceと一致する。
各イベントの大きさを重みにして、この関数の`weight`を出力できる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `weightedpoint3d`

```cpp
template <class Coordinate, class Weight> struct weightedpoint3d
```

三次元座標と重みを持つ点。maximum_weight_dominance_chain_3dへ渡す。

### `dominancechainresult`

```cpp
template <class Weight> struct dominancechainresult
```

最大重みdominance chainの結果。indicesは入力添字による最適chain。

### `maximum_weight_dominance_chain_3d`

```cpp
template <class Coordinate, class Weight> dominancechainresult<Weight> maximum_weight_dominance_chain_3d( const std::vector<weightedpoint3d<Coordinate, Weight>>& points )
```

O(n log^2 n)時間・O(n log n)領域。三座標が全て非減少な部分列の最大重みと復元を返す。
入力順には依存せず、weightは非負であること。空chainの重みは0。

<!-- END AUTO-GENERATED API REFERENCE -->
