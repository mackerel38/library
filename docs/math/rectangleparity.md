---
title: "rectangleparity"
documentation_of: //cp/math/rectangleparity.hpp
---

# `rectangleparity`

<!-- API REFERENCE: COLLAPSED -->

## 概要

未知の0/1行列に「この長方形内のXORは0または1」という条件を追加し、条件をすべて満たす行列を一つ作ります。
セルをすべて変数にせず、条件に現れる長方形の四隅だけでGF(2)連立方程式を解きます。

## 厳密な定義

$H\times W$の0/1行列$A$を未知数とします。半開長方形$[t,b)\times[l,r)$と$v\in\{0,1\}$に対し、次の制約を追加できます。

$$
\bigoplus_{t\leq i<b}\ \bigoplus_{l\leq j<r} A_{i,j}=v
$$

2次元XOR累積$S$を用いると、この左辺は次の四変数だけで表せます。

$$
S_{t,l}\oplus S_{t,r}\oplus S_{b,l}\oplus S_{b,r}
$$

したがって$q$個の制約に必要な累積和変数は高々$4q$個です。

## Include

```cpp
#include "math/rectangleparity.hpp"
```

## 最短の使用例

```cpp
rectangleparitysystem system(height, width);
system.add_constraint(top, left, bottom, right, value);
if (system.satisfiable()) {
    int cell = system(row, column);
    auto matrix = system.answer();
}
```

## APIの選び方

| 操作 | API |
| --- | --- |
| $H\times W$の未知行列を作る | `rectangleparitysystem(H, W)` |
| 長方形XORを$v$に固定 | `add_constraint(t, l, b, r, v)` |
| 解の存在判定と復元 | `satisfiable()` |
| 行列全体を取得 | `answer()` |
| 一つのセルを取得 | `system(row, column)` |

## 注意点・計算量

- 長方形は半開区間で指定します。
- $q$個の制約に対し、`satisfiable()`は$O(q^3/64+HW)$時間、$O(q^2/64+HW)$空間です。
- `answer()`と`operator()`は、直前の`satisfiable()`が`true`を返した後だけ呼べます。
- 通常の和や任意modの長方形制約ではなく、GF(2)上のXOR制約専用です。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rectangleparitysystem`

```cpp
struct rectangleparitysystem
```

0/1行列へ長方形XOR制約を追加する: rectangleparitysystem system(h,w)。

### `rectangleparitysystem`

```cpp
rectangleparitysystem(int height, int width) : height_(height), width_(width)
```

O(1)。height行width列の未知0/1行列を作る。

### `add_constraint`

```cpp
void add_constraint( int top, int left, int bottom, int right, int value )
```

O(1)。半開長方形[top,bottom)x[left,right)のXORをvalueにする。

### `satisfiable`

```cpp
bool satisfiable()
```

O(q^2 min(q,hw)/64+hw)。充足可能ならtrueを返し、0/1行列を復元する。

### `answer`

```cpp
const std::vector<std::vector<int>>& answer() const
```

O(1)。直前のsatisfiable()で得た0/1行列を返す。

### `operator()`

```cpp
int operator()(int row, int column) const
```

O(1)。直前に得た(row,column)の値を返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC276 Ex - Construct a Matrix](https://atcoder.jp/contests/abc276/tasks/abc276_h)では、非零要素を$1\mapsto0, 2\mapsto1$と写すと、長方形の積が1か2かは長方形XORになります。

```cpp
rectangleparitysystem parity(n, n);
for (auto [top, left, bottom, right, product] : constraints) {
    if (product != 0) {
        parity.add_constraint(top, left, bottom, right, product - 1);
    }
}
if (!parity.satisfiable()) cout << "No\n";
```

## 検証

$3\times3$以下の全0/1行列を列挙し、ランダムな長方形制約の充足可能性と復元結果を比較しています。
