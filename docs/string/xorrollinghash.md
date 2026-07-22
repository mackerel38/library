---
title: "xorrollinghash"
documentation_of: //cp/string/xorrollinghash.hpp
---

# `xorrollinghash`

<!-- API REFERENCE: COLLAPSED -->

## 概要

二つの部分列を要素ごとにXORした列について、hashと別の部分列とのLCPを直接求めます。
XORを加法とするnimber体上でRolling Hashを構築するため、XOR後の列を実際に生成しません。

## 厳密な定義

列$A=(A_0,\ldots,A_{n-1})$、実行時に選ぶnimber基数$x$に対し、半開部分列のhashを次で定義します。

$$
H(A[l:r])=\bigoplus_{i=l}^{r-1} A_i\otimes x^{r-1-i}
$$

$\oplus$はbitwise XOR、$\otimes$はNim積です。等長列$B,C$に対して線形性

$$
H(B\oplus C)=H(B)\oplus H(C)
$$

が成り立ちます。

## Include

```cpp
#include "string/xorrollinghash.hpp"
```

## 最短の使用例

```cpp
xorrollinghash hash(values);
auto value = hash.xor_hash(a, a + length, b, b + length);
int common = hash.xor_lcp(a, b, length, target_left, target_right);
```

## APIの選び方

| 欲しい値 | API |
| --- | --- |
| 通常の部分列hash | `hash(left, right)` |
| 二部分列を要素ごとにXORしたhash | `xor_hash(a, b, c, d)` |
| XOR列とtarget部分列のLCP | `xor_lcp(a, c, length, e, f)` |
| 異なる三列間でXOR列とLCPを比較 | `xor_lcp(a, b, other, c, d, target, e, f)` |
| 元の要素 | `hash[index]` |
| 使用中のnimber基数 | `base()` |

## 注意点・計算量

Nim積一回の計算量を$M$とします。

- 構築は$O(nM)$、部分列hashは$O(M)$、`xor_lcp`は$O(M\log n)$です。
- 同じ長さの二部分列だけを`xor_hash`へ渡せます。
- Rolling Hashなので、異なる列のhashが衝突する可能性はゼロではありません。
- 既定の基数は実行時に一度だけ乱択され、同じprocess内の全instanceで共有されます。
- 再現可能なテストや異なるprocess間でhashを共有する場合は、コンストラクタの第二引数へ同じ基数を明示します。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `xorrollinghash`

```cpp
struct xorrollinghash
```

要素ごとのXORと両立するnimber Rolling Hash: xorrollinghash hash(values)。

### `xorrollinghash`

```cpp
explicit xorrollinghash( const std::vector<std::uint64_t>& values, std::uint64_t base = default_base() ) : values_(values), base_(base), prefix_(values.size() + 1), power_(values.size() + 1, 1)
```

O(n M)。64-bit非負整数列valuesのhashを構築する。MはNim積の計算量。

### `size`

```cpp
int size() const noexcept
```

O(1)。列長を返す。

### `operator[]`

```cpp
std::uint64_t operator[](int index) const
```

O(1)。元の列のindex番目を返す。

### `hash`

```cpp
value_type hash(int left, int right) const
```

O(M)。半開区間[left,right)のnimber多項式hashを返す。

### `xor_hash`

```cpp
value_type xor_hash( int first_left, int first_right, int second_left, int second_right ) const
```

O(M)。等長二部分列を要素ごとにXORした列のhashを返す。

### `xor_hash`

```cpp
value_type xor_hash( int first_left, int first_right, const xorrollinghash& other, int second_left, int second_right ) const
```

O(M)。この列とotherの等長部分列を要素ごとにXORしたhashを返す。

### `xor_lcp`

```cpp
int xor_lcp( int first_left, int second_left, int xor_length, int target_left, int target_right ) const
```

O(M log length)。二部分列の要素ごとのXORとtarget部分列のLCP長を返す。

### `xor_lcp`

```cpp
int xor_lcp( int first_left, int first_right, const xorrollinghash& other, int second_left, int second_right, const xorrollinghash& target, int target_left, int target_right ) const
```

O(M log length)。この列とotherのXOR列とtarget部分列のLCP長を返す。

### `base`

```cpp
std::uint64_t base() const noexcept
```

O(1)。このhashが使うnimber基数を返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC274 Ex - XOR Sum of Arrays](https://atcoder.jp/contests/abc274/tasks/abc274_h)では、XOR列と比較対象列のLCPを求め、最初に異なる要素だけを比較します。

```cpp
int common = hash.xor_lcp(a, c, length, e, f);
bool less = e + common != f &&
    (common == length || (values[a + common] ^ values[c + common]) < values[e + common]);
```

## 検証

ランダム列の部分列hashを多項式定義から直接計算し、XOR列のLCPを要素ごとの愚直比較と照合しています。
