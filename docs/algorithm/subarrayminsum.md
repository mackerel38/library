---
title: "区間最小値と区間和による部分配列計数"
documentation_of: //cp/algorithm/subarrayminsum.hpp
---

# 区間最小値と区間和による部分配列計数

## 概要

二つの同じ長さの数列について、「一方の区間最小値」と「もう一方の区間和」の合計が上限以下になる
非空部分配列を数えます。区間最小値が条件へ入るため通常の尺取り法を直接使えない場合に使います。

## 厳密な定義

数列 $A,B$と上限 $S$に対して、$B_i\geq0$を仮定し、

$$
\min_{l\leq i<r}A_i+\sum_{l\leq i<r}B_i\leq S
$$

を満たす整数組 $0\leq l<r\leq n$ の個数を返します。

$A$のmin Cartesian Treeを作ると、各部分配列を左端側の最小値位置へ一意に割り当てられます。
各頂点では、その位置を含む部分配列だけを考え、左右の候補数が少ない側を列挙します。
$B$のprefix和は単調非減少なので、反対側の端点数を二分探索で求められます。

## Include

```cpp
#include "algorithm/subarrayminsum.hpp"
```

## API

```cpp
template<class Minimum, class Weight, class Limit>
long long count_subarrays_min_plus_sum_at_most(
    const vector<Minimum>& minimums,
    const vector<Weight>& weights,
    Limit limit
);
```

- 二つの列の長さは同じである必要があります。
- `weights`の全要素は非負である必要があります。
- prefix和、`minimums[i] + prefix和`、`limit`は共通型の範囲に収めてください。
- 計算量は $O(n\log^2n)$、追加メモリは $O(n)$ です。
- 空列では`0`を返します。

## 使用例

```cpp
vector<long long> minimums{9, 2, 6, 5};
vector<long long> weights{3, 5, 8, 9};
long long answer = count_subarrays_min_plus_sum_at_most(
    minimums, weights, 15LL);
// answer == 6
```

[ABC282 Ex - Min + Sum](https://atcoder.jp/contests/abc282/tasks/abc282_h)では、二つの入力列と`S`を
そのまま渡せます。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_subarrays_min_plus_sum_at_most`

```cpp
template<class Minimum, class Weight, class Limit> long long count_subarrays_min_plus_sum_at_most( const std::vector<Minimum>& minimums, const std::vector<Weight>& weights, Limit limit )
```

O(n log^2 n)。非負weightsについてmin(minimums[l:r])+sum(weights[l:r])<=limitとなる非空区間数。

<!-- END AUTO-GENERATED API REFERENCE -->
