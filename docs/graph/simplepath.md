---
title: "長さ別単純パス数"
documentation_of: //cp/graph/simplepath.hpp
---

# 長さ別単純パス数

## 概要

余剰辺数が小さい無向グラフの長さ別単純パス数え上げを扱う。

## 厳密な定義

- `simple_path_length_counts`: O(n+m+K 2^K)程度。余剰辺数Kが小さい無向グラフでsource-sink単純パス数を辺数別に返す。



## Include

```cpp
#include "graph/simplepath.hpp"
```

余剰辺数`K=m-n+c`が小さい重みなし無向グラフについて、`source`から`sink`への単純パス数を
使用辺数別に返す。答えの`result[length]`が長さ`length`の本数となる。

始終点以外の葉を除去し、残った次数2の道を重み付き辺へ縮約してから小さいグラフ上でDFSする。
計算量は概ね`O(n+m+K 2^K)`、返り値を含むメモリは`O(n+m)`。自己辺は対象外で、多重辺には対応する。
パス数は`long long`で返すため、一般の大きな`K`には向かない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `simple_path_length_counts`

```cpp
template<undirected_graph_type Graph> requires (!Graph::is_weighted) std::vector<long long> simple_path_length_counts( const Graph& graph, int source, int sink )
```

O(n+m+K 2^K)程度。余剰辺数Kが小さい無向グラフでsource-sink単純パス数を辺数別に返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC419 G - Count Simple Paths 2](https://atcoder.jp/contests/abc419/tasks/abc419_g)では、
`M-N+1<=20`のグラフで頂点`0`から`N-1`までの答えをそのまま取得できる。

```cpp
auto count = poe::simple_path_length_counts(graph, 0, n - 1);
for (int length = 1; length < n; ++length) cout << count[length] << ' ';
```

`verify/atcoder_abc419_g.cpp`で公式3 sampleを確認済み。
