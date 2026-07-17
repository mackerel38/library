---
title: "submodular"
documentation_of: //cp/flow/submodular.hpp
---

# `submodular`

## 概要

graph cutによる二値二項劣モジュラ関数最小化を扱う。

## 厳密な定義

- `submodularresult`: 二値劣モジュラ最小化の解。value[i]は変数iの0/1、costは最小値。
- `submodular`: 二値単項・二項劣モジュラ関数をgraph cutで最小化する: submodular<long long> f(n)。


## Include

```cpp
#include "flow/submodular.hpp"
```

二値変数の単項・二項劣モジュラコストを最小カットへ変換する。
`add_pair`には`cost00 + cost11 <= cost01 + cost10`が必要。計算量は構築したグラフ上の最大流と同じ。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `submodularresult`

```cpp
template<class Cost> struct submodularresult
```

二値劣モジュラ最小化の解。value[i]は変数iの0/1、costは最小値。

### `submodular`

```cpp
template<std::signed_integral Cost> struct submodular
```

二値単項・二項劣モジュラ関数をgraph cutで最小化する: submodular<long long> f(n)。

### `submodular`

```cpp
explicit submodular(int n) : size_(n), source_(n), sink_(n + 1), graph_(n + 2)
```

O(n)。n個の二値変数を持つ目的関数を作る。

### `add_constant`

```cpp
void add_constant(Cost cost)
```

O(1)。目的関数へ定数costを加える。

### `add_unary`

```cpp
void add_unary(int i, Cost cost0, Cost cost1)
```

償却O(1)。変数iが0ならcost0、1ならcost1を目的関数へ加える。

### `add_pair`

```cpp
void add_pair( int i, int j, Cost cost00, Cost cost01, Cost cost10, Cost cost11 )
```

償却O(1)。i=x、j=yのときcostxyを加える。cost00+cost11<=cost01+cost10。

### `add_neq_cost`

```cpp
void add_neq_cost(int i, int j, Cost cost)
```

償却O(1)。i!=jのときcostを加える。cost>=0。

### `add_implication_cost`

```cpp
void add_implication_cost(int i, int j, Cost cost)
```

償却O(1)。i=1かつj=0のときcostを加える。cost>=0。

### `minimize`

```cpp
submodularresult<Cost> minimize() const
```

最大流計算1回。目的関数の最小値とそれを達成する0/1割当を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。変数数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ARC085 E - MUL](https://atcoder.jp/contests/arc085/tasks/arc085_c)の最大重み閉包として使える。
`value[i]=1`を宝石を残す選択とし、倍数を残すなら約数も残す制約を十分大きい罰金で表す。

```cpp
poe::submodular<long long> objective(n);
for (int i = 0; i < n; ++i) objective.add_unary(i, 0, -value[i]);
for (int divisor = 1; divisor <= n; ++divisor)
    for (int multiple = divisor * 2; multiple <= n; multiple += divisor)
        objective.add_implication_cost(multiple - 1, divisor - 1, infinity);
auto result = objective.minimize();
std::cout << -result.cost << '\n';
```

`verify/atcoder_arc085_c.cpp`で2026-07-15にjudge AC確認済み。
一般集合関数ではなく二値二項形式を対象とする。
