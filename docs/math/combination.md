---
title: "組合せ"
documentation_of: //cp/math/combination.hpp
---

# 組合せ

- Header: `cp/math/combination.hpp`
- Symbol: `poe::combination<T>`, `poe::pascalcombination`
- Status: experimental

## どんな問題に使えるか

法が素数のmodintなど、階乗が割り算可能な体上で`nCk`を多数回求める問題に向く。
順列の混ぜ合わせ、障害点を避ける格子経路、木DPの子部分木の順序付けに使える。

前計算上限が法以上になる場合は階乗が0になり逆元を持たないため使えない。
法が合成数の場合や逆元を使えない場合は`pascalcombination`を使う。
巨大な`n`でも`k`が小さければ`choose_large(n, k)`を使える。

## API・計算量

```cpp
#include "math/combination.hpp"

poe::combination<mint> comb(limit);
mint ways = comb.choose(n, k);
mint sparse_ways = comb.choose_large(1'000'000'000LL, k);
```

構築`O(limit)`、`factorial`・`permutation`・`choose`・`multichoose`は`O(1)`、
`choose_large(n, k)`は`O(k)`。後者も`k`までの逆階乗を使うため、`limit >= k`が必要。

## 任意の法に対するPascal表

```cpp
poe::pascalcombination comb(limit, modulus);
int binomial = comb.choose(n, k);
int ways = comb.multinomial(counts);
```

Pascalの漸化式だけで前計算するため、`modulus`は素数でなくてもよい。
構築は`O(limit^2)`時間・空間、`choose`は`O(1)`、長さ`m`の`multinomial`は`O(m)`。
多項係数は二項係数の積として計算し、割り算を使わない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `pascalcombination`

```cpp
struct pascalcombination
```

合成数法でも使えるPascal二項係数表: pascalcombination comb(n, modulus)。

### `pascalcombination`

```cpp
pascalcombination(int size, int modulus) : size_(size), modulus_(modulus), values_(static_cast<std::size_t>(size + 1) * (size + 2) / 2)
```

O(n^2)時間・O(n^2)空間。0<=a<=nのaCbを任意の正の法で前計算する。

### `choose`

```cpp
int choose(int n, int k) const
```

O(1)。nCkを返す。範囲外では0。

### `multinomial`

```cpp
template <std::ranges::input_range Range> int multinomial(const Range& counts) const
```

O(m)。countsを出現数とする多項係数(sum counts)!/prod(count!)を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。前計算上限を返す。

### `mod`

```cpp
int mod() const noexcept
```

O(1)。法を返す。

### `combination`

```cpp
template <class T> struct combination
```

modint等で二項係数を前計算する: combination<mint> comb(n); 構築O(n)、取得O(1)。

### `combination`

```cpp
explicit combination(int size)
```

O(n)。0<=k<=nの階乗と逆階乗を前計算する。

### `factorial`

```cpp
T factorial(int n) const
```

O(1)。n!を返す。0<=n<=前計算上限。

### `permutation`

```cpp
T permutation(int n, int k) const
```

O(1)。nPkを返す。範囲外のkでは0を返す。

### `choose`

```cpp
T choose(int n, int k) const
```

O(1)。nCkを返す。範囲外のkでは0を返す。

### `choose_large`

```cpp
template <std::integral Integer> T choose_large(Integer n, int k) const
```

O(k)。巨大なnに対するnCkを返す。0<=k<=前計算上限かつk!が可逆であること。

### `multichoose`

```cpp
T multichoose(int n, int k) const
```

O(1)。重複組合せnHkを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[EDPC Y - Grid 2](https://atcoder.jp/contests/dp/tasks/dp_y)で二点間の最短格子経路数を
`comb.choose(delta_row + delta_column, delta_row)`として求められる。
`verify/atcoder_dp_y.cpp`で2026-07-15にjudge AC確認済み。

[ABC459 E - Select from Subtrees](https://atcoder.jp/contests/abc459/tasks/abc459_e)では、
部分木内の巨大な個数から高々`D_i`個を選ぶ係数を`choose_large`で求める。
`verify/atcoder_abc459_e.cpp`で公式sample一致を確認している。

[ABC425 E - Count Sequences 2](https://atcoder.jp/contests/abc425/tasks/abc425_e)では、
共通の法で上限5000まで一度構築し、各テストケースを`multinomial(counts)`で処理できる。
`verify/atcoder_abc425_e.cpp`で公式sample 2件一致。
