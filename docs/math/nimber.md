---
title: "nimber"
documentation_of: //cp/math/nimber.hpp
---

# `nimber`

<!-- API REFERENCE: COLLAPSED -->

## 概要

bitwise XORを加法、Nim積を乗法とする64-bit nimberを扱います。
XORと両立するRolling Hashや、Nim積が現れるゲーム・代数計算に使います。

## 厳密な定義

非負整数$a,b$のNim積$a\otimes b$はmexを用いて次のように定義されます。

$$
a\otimes b=\operatorname{mex}
\left\{
(a'\otimes b)\oplus(a\otimes b')\oplus(a'\otimes b')
\mid 0\leq a'<a,\ 0\leq b'<b
\right\}
$$

$[0,2^{64})$上では、加法をXOR、乗法をNim積とすると体になります。
`nimber`はこの加法と乗法を提供します。

## Include

```cpp
#include "math/nimber.hpp"
```

## 最短の使用例

```cpp
uint64_t product = nim_product(a, b);
nimber x = a;
nimber y = b;
nimber value = x * y + nimber{c};
```

## APIの選び方

| 用途 | API |
| --- | --- |
| 整数としてNim積だけ計算 | `nim_product(a, b)` |
| XOR加法とNim積を式で組み立てる | `nimber` |
| 元の64-bit表現を取得 | `value.value` |

## 注意点・計算量

- 64-bit幅を固定しているため、`nim_product`は$O(1)$です。
- 初回呼び出し時だけ、8-bit同士のNim積表を構築します。
- `nimber`の減法は加法と同じXORです。
- 現在の型は加減算・乗算を提供します。除算や逆元は提供していません。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `nim_product`

```cpp
inline std::uint64_t nim_product(std::uint64_t left, std::uint64_t right)
```

O(1)。64-bit nimberのNim積left tensor rightを返す。

### `nimber`

```cpp
struct nimber
```

XORを加法、Nim積を乗法とする64-bit nimber体の要素。

### `nimber`

```cpp
constexpr nimber(std::uint64_t value = 0) : value(value)
```

O(1)。整数valueを同じbit表現のnimberにする。

### `operator+=`

```cpp
nimber& operator+=(nimber other)
```

O(1)。加法はbitwise XOR。

### `operator*=`

```cpp
nimber& operator*=(nimber other)
```

O(1)。乗法はNim積。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC274 Ex - XOR Sum of Arrays](https://atcoder.jp/contests/abc274/tasks/abc274_h)では、nimber体上でRolling Hashを作ることで、要素ごとのXORをhash値のXORとして計算できます。

## 検証

$0\leq a,b<32$のNim積をmex定義から直接計算して比較し、64-bit乱数10,000組で可換性・分配法則・単位元を確認しています。
