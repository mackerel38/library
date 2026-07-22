---
title: "decreasingconvex"
documentation_of: //cp/dp/decreasingconvex.hpp
---

# `decreasingconvex`

<!-- API REFERENCE: COLLAPSED -->

## 概要

非負整数上で単調に減少する離散凸関数を、値が曲がる位置だけで持ちます。
DP関数同士の加算と、「追加費用を払って状態を右へ進める」min-plus遷移を高速に繰り返せます。

## 厳密な定義

関数$f:\mathbb Z_{\geq0}\to T$について、差分

$$
d_x=f(x)-f(x+1)
$$

が非負かつ単調非増加である場合を扱います。すなわち$f$は単調減少な離散凸関数です。

`merge`は二関数の点ごとの和$h(x)=f(x)+g(x)$を作ります。
`suffix_min(cost, required)`は、次の関数へ更新します。

$$
g(x)=\min_{y\geq\max(x,\mathrm{required})}
\bigl(f(y)+(y-x)\mathrm{cost}\bigr)
$$

## Include

```cpp
#include "dp/decreasingconvex.hpp"
```

## 最短の使用例

```cpp
decreasingconvexsequence<long long> left(required_left, cost_left);
decreasingconvexsequence<long long> right(required_right, cost_right);
left.merge(std::move(right));
left.suffix_min(unit_cost, required);
long long answer = left.zero();
```

## APIの選び方

| 欲しい操作 | API |
| --- | --- |
| $f(x)=0$ | `decreasingconvexsequence<T>()` |
| $f(x)=c\max(a-x,0)$ | `decreasingconvexsequence<T>(a, c)` |
| 二つの関数の点ごとの和 | `merge(other)` |
| suffix min-plus遷移 | `suffix_min(cost, required)` |
| $f(x)$ | `function[x]` |
| $f(0)$ | `zero()` |

## 注意点・計算量

- 定義域は非負整数です。`required`と`cost`も非負である必要があります。
- `merge`は折れ点数が少ない側を多い側へ併合し、$O(p\log q)$時間です。
- 木DP全体ではsmall-to-largeにより、折れ点の挿入に合計$O(n\log^2n)$時間かかります。
- `suffix_min`で削除された折れ点は戻らないため、一連の遷移に対する削除回数は追加した折れ点数以下です。
- `operator[]`は$O(p)$です。答えが$f(0)$なら$O(1)$の`zero()`を使います。
- `add_abs`など一般の凸関数操作が必要なら`dp/slopetrick.hpp`を使います。こちらは関数同士のmergeと上記min-plus遷移に特化しています。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `decreasingconvexsequence`

```cpp
template <class T> struct decreasingconvexsequence
```

非負整数上の単調減少な離散凸関数を折れ点で持つ: decreasingconvexsequence<long long> f。

### `decreasingconvexsequence`

```cpp
decreasingconvexsequence() = default
```

O(1)。f(x)=0 (x>=0)を作る。

### `decreasingconvexsequence`

```cpp
decreasingconvexsequence(long long required, T cost) : value_zero_(static_cast<T>(required) * cost), difference_zero_(required > 0 ? cost : T
```

O(1)。f(x)=cost*max(required-x,0) (x>=0)を作る。

### `merge`

```cpp
void merge(decreasingconvexsequence other)
```

O(smaller log(larger))。f(x)へother(x)を点ごとに加える。

### `suffix_min`

```cpp
void suffix_min(T cost, long long required)
```

ならしO(p log p)。g(x)=min_{y>=max(x,required)}((y-x)cost+f(y))へ更新する。

### `operator[]`

```cpp
T operator[](long long x) const
```

O(p)。非負整数xにおけるf(x)を返す。

### `zero`

```cpp
T zero() const
```

O(1)。f(0)を返す。

### `pieces`

```cpp
int pieces() const noexcept
```

O(1)。保持している傾き変化点数を返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC275 Ex - Monster](https://atcoder.jp/contests/abc275/tasks/abc275_h)では、バリア強度のmax Cartesian Tree上で子のDP関数をmergeし、各頂点で次の遷移を行います。

```cpp
dp[vertex].merge(std::move(dp[left_child]));
dp[vertex].merge(std::move(dp[right_child]));
dp[vertex].suffix_min(barrier[vertex], stamina[vertex]);
```

根の`zero()`が必要な最小費用です。

## 検証

短い関数列を配列として愚直に持つ実装と、5,000系列について各20回のmerge・min-plus遷移を比較しています。
さらに小さいモンスター列の全体力状態をDijkstraで探索し、Cartesian Tree上のDPと2,000件比較しています。
