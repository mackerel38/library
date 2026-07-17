---
title: "XOR線形基底"
documentation_of: //cp/math/xorbasis.hpp
---

# XOR線形基底

- Header: `cp/math/xorbasis.hpp`
- Symbol: `poe::xorbasis`
- Status: experimental

```cpp
#include "math/xorbasis.hpp"
```

非負整数をGF(2)上のbit vectorとして扱う。`add`は独立性、`contains`は表現可能性を返し、
`min(x)`・`max(x)`で`x XOR 部分集合XOR`の最小最大、`kth(k)`で作れる値の昇順k番目を得る。
`merge`で二つの空間を統合できる。

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

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC141 F - Xor Sum 3](https://atcoder.jp/contests/abc141/tasks/abc141_f)で最大化、
[ABC236 F - Spices](https://atcoder.jp/contests/abc236/tasks/abc236_f)で価格順の独立性判定に使える。
両方のverifyコードを用意し、公開サンプル確認済み・。
