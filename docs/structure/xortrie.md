---
title: "XOR Trie"
documentation_of: //cp/structure/xortrie.hpp
---

# XOR Trie

- Header: `cp/structure/xortrie.hpp`
- Symbol: `poe::xortrie`, `poe::sum_min_xor`, `poe::count_pairs_xor_at_most`
- Status: experimental

## どんな問題に使えるか

非負整数のmultisetに対し、値の追加・削除・個数取得と、指定値とのXORが上限未満・以下になる
要素数を上位bitから数える。指定値との最小XOR、整数区間全体に対する最小XORの総和、列内の
二要素のXORが上限以下となる組数も求められる。

```cpp
#include "structure/xortrie.hpp"

xortrie<unsigned, 30> trie;
trie.add(x);
long long count = trie.count_xor_at_most(y, limit);
unsigned minimum = trie.min_xor(y);
unsigned long long total = sum_min_xor<unsigned, 30>(values, left, right);
long long pairs = count_pairs_xor_at_most<unsigned, 30>(values, limit);
```

各操作`O(bits)`、領域`O(追加された相異prefix数)`。値と上限は指定bit数に収まること。
`sum_min_xor(values,upper)`は`0<=x<upper`、三引数版は`left<=x<right`について
`min_a(x xor a)`を合計する。静的Trieと各部分木の全域和を`O(n bits)`で作り、集約自体は`O(bits)`。
返り値を64bitに収めるため`Bits<=32`とする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `xortrie`

```cpp
template <std::unsigned_integral UInt = unsigned, int Bits = std::numeric_limits<UInt>::digits> struct xortrie
```

整数multisetのXOR検索: xortrie<unsigned,30> trie; 各操作O(bits)。

### `xortrie`

```cpp
xortrie()
```

O(1)。空のtrieを作る。

### `add`

```cpp
void add(UInt value, int count = 1)
```

O(bits)。valueをcount個追加する。負数countは既存要素の削除に使える。

### `erase`

```cpp
void erase(UInt value)
```

O(bits)。valueを一つ削除する。

### `count`

```cpp
int count(UInt value) const
```

O(bits)。valueの個数を返す。

### `count_xor_less`

```cpp
long long count_xor_less(UInt value, UInt limit) const
```

O(bits)。value xor xがlimit未満となる格納値xの個数を返す。

### `count_xor_at_most`

```cpp
long long count_xor_at_most(UInt value, UInt limit) const
```

O(bits)。value xor xがlimit以下となる格納値xの個数を返す。

### `min_xor`

```cpp
UInt min_xor(UInt value) const
```

O(bits)。格納値xに対するvalue xor xの最小値を返す。空でないことが必要。

### `size`

```cpp
int size() const noexcept
```

O(1)。重複込み要素数を返す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。空ならtrueを返す。

### `sum_min_xor`

```cpp
template<std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits> unsigned long long sum_min_xor(const std::vector<UInt>& values, unsigned long long upper)
```

O(n bits)前計算・O(bits)集約。x=0..upper-1についてmin_a(x xor a)の総和を返す。

### `sum_min_xor`

```cpp
template<std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits> unsigned long long sum_min_xor( const std::vector<UInt>& values, unsigned long long left, unsigned long long right )
```

O(n bits)前計算・O(bits)集約。x=left..right-1についてmin_a(x xor a)の総和を返す。

### `count_pairs_xor_at_most`

```cpp
template <std::unsigned_integral UInt, int Bits = std::numeric_limits<UInt>::digits> long long count_pairs_xor_at_most(const std::vector<UInt>& values, UInt limit)
```

O(n bits)。i<jかつvalues[i] xor values[j]<=limitとなる組数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC451 G - Minimum XOR Walk](https://atcoder.jp/contests/abc451/tasks/abc451_g)では、
閉路XORの線形基底で各頂点の木path XORを最小化した後、頂点対を本関数で数える。
`verify/atcoder_abc451_g.cpp`に提出用コードを用意し、公式sampleを確認済み。judge ACは未確認。

[AtCoder ABC425 G - Sum of Min of XOR](https://atcoder.jp/contests/abc425/tasks/abc425_g)では、
`sum_min_xor(values,M)`で`0<=x<M`の最近傍XOR距離をまとめて求める。
`verify/atcoder_abc425_g.cpp`で公式sampleを確認済み。judge ACは未確認。
