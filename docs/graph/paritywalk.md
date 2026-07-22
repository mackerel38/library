---
title: "頂点訪問回数の偶奇を指定するwalk"
documentation_of: //cp/graph/paritywalk.hpp
---

# 頂点訪問回数の偶奇を指定するwalk

## 概要

連結な無向グラフで、各頂点をwalk中に訪れる回数の偶奇を自由に指定し、それを満たすwalkを構成する。
walkの長さは$4n$未満に収まり、深い全域木でも再帰を使わない。

## 厳密な定義

各頂点$v$について目標値$t_v\in\{0,1\}$が与えられる。返す頂点列
$W=(w_1,w_2,\ldots,w_k)$は、連続する二頂点が必ず辺で結ばれ、全ての$v$について

$$
\#\{i\mid w_i=v\}\equiv t_v\pmod2
$$

を満たす。さらに$0\le k<4n$である。空のwalkも許す。

## Include

```cpp
#include "graph/paritywalk.hpp"
```

```cpp
std::vector<int> walk = vertex_parity_walk(graph, target, root);
```

`target[v]`は`0`または`1`でなければならない。`root`は構成に使う全域木の根で、
省略時は頂点$0$を使う。返す頂点番号は0-indexedである。

全域木をDFSし、子から戻った時点で子の偶奇が違えば`parent, child`を追加して修正する。
最後に根だけが違う場合はwalkの先頭の根を除く。時間計算量は$O(n+m)$、領域計算量は$O(n)$である。

## 頻出事故

- `graph`は連結な無向グラフである必要がある。
- ここでのwalkは頂点や辺の重複を許す。
- 長さは辺数ではなく、出力する頂点列の要素数である。
- 全ての目標が偶数でも、返り値が必ず空になるとは限らない。
- 出力時に1-indexedへ直す必要がある問題では各頂点へ$1$を足す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `vertex_parity_walk`

```cpp
template<undirected_graph_type Graph> std::vector<int> vertex_parity_walk( const Graph& graph, const std::vector<int>& target, int root = 0 )
```

O(n+m)。連結無向graphで各頂点の訪問回数をtarget parityにする長さ4n未満のwalkを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC244 G - Construct Good Path](https://atcoder.jp/contests/abc244/tasks/abc244_g)では、
二進文字列を`0`・`1`の`target`へ変換して渡すだけでよい。
`verify/atcoder_abc244_g.cpp`に提出用コードを用意している。
