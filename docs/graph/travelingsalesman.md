---
title: "Traveling Salesman Problem"
documentation_of: //cp/graph/travelingsalesman.hpp
---

# Traveling Salesman Problem

- Header: `cp/graph/travelingsalesman.hpp`
- Symbol: `poe::travelingsalesman`, `poe::travelingsalesman_result`
- Status: experimental

## どんな問題に使えるか

開始頂点から全頂点を一度ずつ訪れ、開始頂点へ戻る最小費用閉路を求める部分問題に使う。
入力は距離行列なので、元問題が座標、最短路距離、状態間遷移費用のどれでも利用できる。
到達不能辺には呼び出し側で指定した`inf`を入れる。

```cpp
#include "graph/travelingsalesman.hpp"

auto result = poe::travelingsalesman(cost, inf, start);
if (result.reachable) {
    std::cout << result.cost << '\n';
    for (int vertex : result.path) std::cout << vertex << ' ';
}
```

`O(n^2 2^n)`時間、`O(n2^n)`領域。空行列は費用0、1頂点は`{start,start}`を返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `travelingsalesman_result`

```cpp
template<class T> struct travelingsalesman_result
```

Traveling Salesman DPの最小費用と閉路。到達不能ならreachable=false。

### `travelingsalesman`

```cpp
template<class T> travelingsalesman_result<T> travelingsalesman( const std::vector<std::vector<T>>& cost, T inf, int start = 0 )
```

O(n^2 2^n)時間・O(n 2^n)領域。travelingsalesman(cost, inf, start): 全頂点を巡る最小閉路。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ DPL_2_A - Traveling Salesman Problem](https://onlinejudge.u-aizu.ac.jp/problems/DPL_2_A)へ直接使える。
`verify/aoj_dpl_2_a.test.cpp`で2026-07-15にAOJ公式テスト全40ケースの自動verify成功。
