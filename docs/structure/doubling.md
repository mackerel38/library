---
title: "ダブリング"
documentation_of: //cp/structure/doubling.hpp
---

# ダブリング

- Header: `cp/structure/doubling.hpp`
- Symbols: `poe::doubling`, `poe::weighteddoubling`
- Status: experimental

## どんな問題に使えるか

各状態から次状態が一意に決まる関数グラフで、巨大回数先の状態を求める。さらに、遷移先に対する条件が
経路上でtrueからfalseへ一度だけ変わるなら、条件内で進める最大回数も二分累乗表上で求められる。

辺ごとの値も必要なら`weighteddoubling`で、和・積・文字列結合などの結合的な演算を通過順に集約できる。
次状態が複数ある場合や、経路上でpredicateが再びtrueになる場合は対象外。

## API・計算量

```cpp
#include "structure/doubling.hpp"

poe::doubling table(next, max_step);
int destination = table.jump(start, step);

poe::weighteddoubling<long long> weighted(next, weight, max_step);
auto [vertex, sum] = weighted.jump(start, step);
```

どちらも構築`O(n log K)`時間・領域、`jump`と`max_jump`は`O(log K)`。
`weighteddoubling`の`next`は行き止まりのない全域関数にし、`weight[v]`は`v -> next[v]`の値とする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `weighteddoublingresult`

```cpp
template <class T> struct weighteddoublingresult
```

重み付きダブリングの到着頂点と経路集約値。

### `doubling`

```cpp
struct doubling
```

関数グラフの二分累乗表: doubling table(next, max_step); 構築O(n log K)。

### `doubling`

```cpp
explicit doubling(const std::vector<int>& next, unsigned long long max_step) : size_(static_cast<int>(next.size()))
```

O(n log max_step)。next[v]は次の頂点、-1は行き止まり。

### `jump`

```cpp
int jump(int start, unsigned long long step) const
```

O(log step)。startからstep回進んだ頂点を返す。行き止まりなら-1。

### `max_jump`

```cpp
template <class Predicate> std::pair<unsigned long long, int> max_jump(int start, Predicate predicate) const
```

O(log K)。到着頂点がpredicateを満たす範囲で進める最大回数と頂点を返す。
start自身は条件を満たし、遷移に沿ってpredicateがtrueからfalseへ一度だけ変わること。

### `weighteddoubling`

```cpp
template <class T, class Operation = std::plus<T>> struct weighteddoubling
```

関数グラフ上の遷移値も集約する: weighteddoubling table(next, weight, max_step); 構築O(n log K)。

### `weighteddoubling`

```cpp
explicit weighteddoubling(const std::vector<int>& next, const std::vector<T>& weight, unsigned long long max_step, Operation operation = Operation
```

O(n log max_step)。weight[v]はvからnext[v]へ進む一回分の値。nextは全頂点で定義する。

### `jump`

```cpp
weighteddoublingresult<T> jump(int start, unsigned long long step) const
```

O(log step)。startからstep回進んだ到着頂点と、通過順に畳み込んだ値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[NDPC E - 夏休み](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_e)では、次に参加できるイベントへの
遷移を作り、終了日が`R`以下の範囲で`max_jump`できる。
`verify/atcoder_ndpc2026_e.cpp`で2026-07-15にjudge AC確認済み。

[AtCoder ABC438 E - Heavy Buckets](https://atcoder.jp/contests/abc438/tasks/abc438_e)では、
人の移動を`next`、現在の人番号を一回分の`weight`として水量を集約する。
`verify/atcoder_abc438_e.cpp`は公式サンプル一致を確認済み、judge未提出。
