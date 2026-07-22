---
title: "integerconstraints"
documentation_of: //cp/graph/integerconstraints.hpp
---

# `integerconstraints`

<!-- API REFERENCE: COLLAPSED -->

## 概要

範囲の決まった整数変数に「二つの変数の和はこの範囲内」という条件を追加し、条件をすべて満たす値を求めます。
各整数を値ごとの真偽変数へ変換する処理を隠すため、2-SATを直接組み立てる必要がありません。

## 厳密な定義

整数変数 $x_0,\ldots,x_{n-1}$ と、その定義域

$$
l_i\leq x_i\leq r_i
$$

を持ちます。次の形の制約を任意個追加できます。

$$
L\leq x_i+x_j\leq R
$$

下限だけの $L\leq x_i+x_j$、上限だけの $x_i+x_j\leq R$ も追加できます。
同じ変数を二回指定してよく、その場合は $L\leq 2x_i\leq R$ を表します。

内部では各 $l_i<t\leq r_i$ に対して命題 $[x_i\geq t]$ を作ります。
例えば和の下限は、各整数 $t$ に対する次の2-SAT節へ変換されます。

$$
[x_i\geq t]\lor[x_j\geq L-t+1]
$$

## Include

```cpp
#include "graph/integerconstraints.hpp"
```

## 最短の使用例

```cpp
boundedsumsat sat(4, 5); // x_i in [0, 5]
sat.add_sum_constraint(0, 2, 5, 7);
sat.add_sum_lower(0, 3, 1);
sat.add_sum_upper(1, 1, 8);

if (sat.satisfiable()) {
    vector<int> x = sat.answer();
}
```

## APIの選び方

| 欲しい条件 | 呼び出し |
| --- | --- |
| 全変数が $0\leq x_i\leq M$ | `boundedsumsat(n, M)` |
| 変数ごとに $l_i\leq x_i\leq r_i$ | `boundedsumsat(bounds)` |
| $L\leq x_i+x_j\leq R$ | `add_sum_constraint(i, j, L, R)` |
| $L\leq x_i+x_j$ | `add_sum_lower(i, j, L)` |
| $x_i+x_j\leq R$ | `add_sum_upper(i, j, R)` |
| 条件を満たす値が存在するか | `satisfiable()` |
| 得られた値を一括取得 | `answer()` |
| $x_i$だけ取得 | `sat[i]` |

## 注意点・計算量

$W_i=r_i-l_i$、$T=\sum_i W_i$ とします。

- 構築は $O(T)$ 時間・空間です。
- 変数 $i,j$ の和制約一つは $O(\min(W_i,W_j))$ 個の節を追加します。
- 生成した節の総数を $C$ とすると、`satisfiable()` は $O(T+C)$ 時間です。
- `answer()`と`operator[]`は、直前の`satisfiable()`が`true`を返した後だけ呼べます。
- 一般の整数線形制約を解くものではありません。係数がともに $1$ の二変数和制約に使います。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `boundedsumsat`

```cpp
struct boundedsumsat
```

有界整数変数へ二変数和の上下限制約を追加する: boundedsumsat sat(bounds)。

### `boundedsumsat`

```cpp
explicit boundedsumsat(std::vector<std::pair<int, int>> bounds) : bounds_(std::move(bounds)), offsets_(make_offsets(bounds_)), sat_(offsets_.back()), answer_(bounds_.size())
```

O(sum(upper-lower))。各変数iの定義域を整数閉区間bounds[i]にする。

### `boundedsumsat`

```cpp
boundedsumsat(int variable_count, int maximum_value) : boundedsumsat(make_uniform_bounds(variable_count, maximum_value))
```

O(nM)。n変数をすべて整数閉区間[0,maximum_value]にする。

### `add_sum_constraint`

```cpp
void add_sum_constraint(int i, int j, long long lower, long long upper)
```

O(min(width_i,width_j))。lower<=x_i+x_j<=upperを追加する。

### `add_sum_lower`

```cpp
void add_sum_lower(int i, int j, long long lower)
```

O(min(width_i,width_j))。x_i+x_j>=lowerを追加する。

### `add_sum_upper`

```cpp
void add_sum_upper(int i, int j, long long upper)
```

O(min(width_i,width_j))。x_i+x_j<=upperを追加する。

### `satisfiable`

```cpp
bool satisfiable()
```

O(V+E)。充足可能ならtrueを返し、整数割当てanswer()を構築する。

### `answer`

```cpp
const std::vector<int>& answer() const
```

O(1)。直前のsatisfiable()で得た整数割当てを返す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(1)。直前に得た変数indexの整数値を返す。

### `variables`

```cpp
int variables() const noexcept
```

O(1)。整数変数の個数を返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC277 Ex - Constrained Sums](https://atcoder.jp/contests/abc277/tasks/abc277_h)では、全変数の定義域が$[0,M]$で、入力の各条件をそのまま追加できます。

```cpp
int n, maximum_value, q;
cin >> n >> maximum_value >> q;
boundedsumsat sat(n, maximum_value);
while (q--) {
    int a, b, lower, upper;
    cin >> a >> b >> lower >> upper;
    sat.add_sum_constraint(a - 1, b - 1, lower, upper);
}
if (!sat.satisfiable()) cout << -1 << '\n';
else for (int i = 0; i < n; ++i) cout << sat[i] << " \n"[i + 1 == n];
```

## 検証

小さい定義域を全探索し、充足可能性と復元した各値を2-SAT版と比較しています。
