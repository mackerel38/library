---
title: "Prefix木上の多重集合変換"
documentation_of: //cp/algorithm/multisettransform.hpp
---

# Prefix木上の多重集合変換

## 概要

二つのword多重集合をprefix木へ置き、末尾の一文字を削除する操作で同じ多重集合へ揃える最小回数を求める。
左右で削除できる文字を別々に制限できる。整数を二進表記したときの「2倍または2で切り捨て除算」には、
整数列を直接渡せるshortcutを使える。

## 厳密な定義

各wordを根からそのwordまでのpathとみなすprefix木を考える。left側とright側の要素は、現在の頂点から
親へ一辺移動するたびに費用$1$を払う。辺の文字$c$を削除できるかは、left側では$P_L(c)$、right側では
$P_R(c)$で定める。全left要素と全right要素を同じ頂点で一対一に対応付ける最小費用を返す。

部分木内の要素はその根を通らないと外へ出られないため、深い頂点から同じ場所の左右要素を最大限対応付け、
余りだけを親へ送ることで最適になる。許可されていない辺を余った要素が越える必要がある場合は不可能とする。

## Include

```cpp
#include "algorithm/multisettransform.hpp"
```

## `prefixmultisetmatcher`

```cpp
template<int Alphabet>
struct prefixmultisetmatcher;
```

文字を整数$0,1,\ldots,\mathit{Alphabet}-1$で表すprefix木を構築する。

```cpp
prefixmultisetmatcher<3> matcher;
matcher.add_left(vector<int>{1, 2});
matcher.add_right(vector<int>{1});

auto answer = matcher.minimum_moves(
    [](int symbol) { return true; },
    [](int symbol) { return symbol == 0; }
);
```

- `add_left(word,count)` / `add_right(word,count)`: 対応する側へ同じwordを`count`個追加する。
  `count > 0`が必要で、時間計算量は$O(|word|)$。
- `minimum_moves(left_movable,right_movable)`: predicateは親へ移動するときに削除する末尾文字を受け取る。
  最小移動回数を返し、不可能なら`nullopt`。時間・追加領域はprefix木の頂点数に対して$O(V)$。
- `size()`: prefix木の頂点数を$O(1)$で返す。

空wordは根へ追加される。左右の総要素数が異なる場合も`minimum_moves`は`nullopt`を返す。

## `minimum_double_halve_multiset_operations`

```cpp
template<unsigned_integral UInt>
optional<long long> minimum_double_halve_multiset_operations(
    const vector<UInt>& source,
    const vector<UInt>& target
);
```

`source`の一要素$x$へ、次のいずれかを一回の操作として何度でも適用する。

$$
x\leftarrow 2x,\qquad
x\leftarrow\left\lfloor\frac{x}{2}\right\rfloor.
$$

`source`を`target`と同じ多重集合へ変える最小操作回数を返し、不可能なら`nullopt`を返す。
要素数を$n$、`UInt`のbit数を$B$として時間・領域計算量は$O(nB)$。
`0`の二進表記は空wordとして扱うため、`0`を2倍して正数へ変えることはできない。

## 使用例

[AtCoder ABC254 Ex - Multiply or Divide by 2](https://atcoder.jp/contests/abc254/tasks/abc254_h)は、
入力した二つの整数列を`minimum_double_halve_multiset_operations`へ渡すだけで変換可能性と最小回数を求められる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `prefixmultisetmatcher`

```cpp
template<int Alphabet> struct prefixmultisetmatcher
```

prefix木上の二多重集合matching: add_left/add_right後、minimum_movesで親移動の最小回数を求める。

### `prefixmultisetmatcher`

```cpp
prefixmultisetmatcher() : nodes_(1)
```

O(1)。空列だけを表す根を作る。

### `add_left`

```cpp
template<std::ranges::input_range Range> void add_left(const Range& word, long long count = 1)
```

O(length)。left側へwordをcount個追加する。各symbolは[0,Alphabet)。

### `add_right`

```cpp
template<std::ranges::input_range Range> void add_right(const Range& word, long long count = 1)
```

O(length)。right側へwordをcount個追加する。各symbolは[0,Alphabet)。

### `minimum_moves`

```cpp
template<class LeftMovable, class RightMovable> std::optional<long long> minimum_moves( LeftMovable left_movable, RightMovable right_movable ) const
```

O(nodes)。各辺を親へ越えられる条件の下で全要素を対応付ける最小移動回数を返し、不可能ならnullopt。

### `size`

```cpp
int size() const noexcept
```

O(1)。prefix木の頂点数を返す。

### `minimum_double_halve_multiset_operations`

```cpp
template<std::unsigned_integral UInt> std::optional<long long> minimum_double_halve_multiset_operations( const std::vector<UInt>& source, const std::vector<UInt>& target )
```

O(n bits)時間・領域。sourceの一要素を2倍またはfloor(x/2)にしてtarget多重集合へ変える最小回数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
