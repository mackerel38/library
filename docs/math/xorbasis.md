---
title: "XOR線形基底"
documentation_of: //cp/math/xorbasis.hpp
---

# XOR線形基底

## 概要

XOR線形基底の挿入・所属・最小最大・k番目を扱う。二成分ベクトルの部分集合XORについて、
第一成分へ上限を課しながら第二成分を最大化することもできる。

## 厳密な定義

- `xorbasis`: xorbasis<UInt>: XOR線形基底。add・contains・min・max・kthを各O(bit)で行う。
- `maximum_paired_xor_under`: 非空部分集合$S$について、$\bigoplus_{i\in S}A_i\le K$のもとで
  $\bigoplus_{i\in S}B_i$を最大化する。



## Include

```cpp
#include "math/xorbasis.hpp"
```

非負整数をGF(2)上のbit vectorとして扱う。`add`は独立性、`contains`は表現可能性を返し、
`min(x)`・`max(x)`で`x XOR 部分集合XOR`の最小最大、`kth(k)`で作れる値の昇順k番目を得る。
`merge`で二つの空間を統合できる。

```cpp
optional<unsigned> answer
    = maximum_paired_xor_under<unsigned, 30>(pairs, upper);
```

`pairs[i]={A_i,B_i}`とする。条件を満たす非空部分集合がなければ`nullopt`を返す。同じXOR値を
作る非空部分集合と空部分集合を区別するため、入力ベクトルの線形従属性も考慮する。
`Bits`は各成分のbit数で、$2Bits\le64$が必要。時間計算量は
$O(nBits+Bits^3)$、領域計算量は$O(Bits^2)$である。

二成分を連結したGF(2)ベクトルを行基本変形し、第一成分が`upper`と一致する場合と、最初に
小さくなるbitごとの場合を調べる。固定prefixを線形方程式として解き、その解空間上で第二成分を
通常のXOR基底により最大化する。

通常操作はbit数を`B`として`O(B)`、`vectors`と`kth`は`O(B^2)`、領域`O(B)`。
`kth`の添字は64bitなので、rank 64では全範囲の添字が有効になる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `xorbasis`

```cpp
template<std::unsigned_integral UInt = unsigned long long> struct xorbasis
```

xorbasis<UInt>: XOR線形基底。add・contains・min・max・kthを各O(bit)で行う。

### `add`

```cpp
bool add(UInt value)
```

O(bit)。basis.add(x): xが独立なら追加してtrue、従属ならfalseを返す。

### `contains`

```cpp
bool contains(UInt value) const
```

O(bit)。basis.contains(x): 基底の部分集合XORでxを作れるか返す。

### `rank`

```cpp
int rank() const noexcept
```

O(1)。basis.rank(): 基底のrankを返す。

### `max`

```cpp
UInt max(UInt value = 0) const
```

O(bit)。basis.max(x): xと部分集合XORのXORとして作れる最大値を返す。

### `min`

```cpp
UInt min(UInt value = 0) const
```

O(bit)。basis.min(x): xと部分集合XORのXORとして作れる最小値を返す。

### `vectors`

```cpp
std::vector<UInt> vectors() const
```

O(bit^2)。basis.vectors(): 縮約した基底をpivot昇順で返す。

### `kth`

```cpp
UInt kth(unsigned long long index) const
```

O(bit^2)。basis.kth(k): 作れる値を昇順に並べた0-indexed k番目を返す。

### `merge`

```cpp
void merge(const xorbasis& other)
```

O(bit^2)。basis.merge(other): otherの張る空間を追加する。

### `maximum_paired_xor_under`

```cpp
template<std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits / 2> std::optional<UInt> maximum_paired_xor_under( const std::vector<std::pair<UInt, UInt>>& values, UInt upper )
```

O(n bits+bits^3)時間・O(bits^2)領域。非空部分集合のfirst XOR<=upperでsecond XORを最大化し、存在しなければnullopt。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC141 F - Xor Sum 3](https://atcoder.jp/contests/abc141/tasks/abc141_f)で最大化、
[ABC236 F - Spices](https://atcoder.jp/contests/abc236/tasks/abc236_f)で価格順の独立性判定に使える。
公開サンプル確認済み。

[AtCoder ABC249 G - Xor Cards](https://atcoder.jp/contests/abc249/tasks/abc249_g)では、
各カードの表裏をpairにして`maximum_paired_xor_under`へ渡す。
`verify/atcoder_abc249_g.cpp`に提出用コードを用意し、公式sampleを確認している。
