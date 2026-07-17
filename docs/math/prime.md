---
title: "素数・素因数分解"
documentation_of: //cp/math/prime.hpp
---

# 素数・素因数分解

- Header: `cp/math/prime.hpp`
- Symbol: `poe::primetable`, `poe::is_prime`, `poe::factorize`, `poe::divisors`, `poe::signed_divisors`
- Status: experimental

```cpp
#include "math/prime.hpp"
```

上限が小さい多数クエリには線形篩`primetable(limit)`、巨大な64bit整数の少数クエリには
決定的Miller–RabinとPollard–Rhoを使う。`table[value]`で素数判定、`table.least(value)`で
最小素因数、両方式の`factorize`で`{素因数,指数}`を昇順に取得する。

線形篩は構築`O(n)`、範囲内判定`O(1)`、分解`O(log n)`。64bit素数判定は概ね`O(log^3 n)`、
Pollard–Rho分解は乱択で期待`O(n^(1/4))`程度。`0`の分解や約数列挙は定義しない。
`signed_divisors(n)`は非零の符号付き整数について、負の約数、正の約数の順で全てを昇順に返す。
`LLONG_MIN`は正の絶対値を`long long`で表せないため対象外。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `primetable`

```cpp
struct primetable
```

primetable(n): 0..nの最小素因数をO(n)で前計算し、素数判定・因数分解・約数列挙に使う。

### `primetable`

```cpp
explicit primetable(int limit) : least_(static_cast<std::size_t>(limit) + 1)
```

O(n)。0<=value<=nの最小素因数を線形篩で構築する。

### `operator[]`

```cpp
bool operator[](int value) const
```

O(1)。table[value]: valueが素数か返す。

### `least`

```cpp
int least(int value) const
```

O(1)。table.least(value): valueの最小素因数を返す。value>=2。

### `factorize`

```cpp
std::vector<std::pair<int, int>> factorize(int value) const
```

O(log value)。table.factorize(value): {{素因数,指数}}を昇順で返す。value>=1。

### `divisors`

```cpp
std::vector<int> divisors(int value) const
```

O(tau(value) log tau(value))。table.divisors(value): 正の約数を昇順で返す。

### `is_prime`

```cpp
inline bool is_prime(unsigned long long value)
```

O(log^3 n)程度。is_prime(n): 64bit符号なし整数nが素数か決定的Miller–Rabin法で判定する。

### `factorize`

```cpp
inline std::vector<std::pair<unsigned long long, int>> factorize(unsigned long long value)
```

期待O(n^(1/4))程度。factorize(n): 64bit整数nの{{素因数,指数}}を昇順で返す。n>=1。

### `divisors`

```cpp
inline std::vector<unsigned long long> divisors(unsigned long long value)
```

O(tau(n) log tau(n))。divisors(n): 64bit整数nの正の約数を昇順で返す。n>=1。

### `signed_divisors`

```cpp
inline std::vector<long long> signed_divisors(long long value)
```

期待O(|n|^(1/4)+tau(|n|) log tau(|n|))程度。signed_divisors(n): 非零整数nの正負の約数を昇順で返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC149 C - Next Prime](https://atcoder.jp/contests/abc149/tasks/abc149_c)では`is_prime`、
[ABC284 D - Happy New Year 2023](https://atcoder.jp/contests/abc284/tasks/abc284_d)では`factorize`を使える。
Library Checker Factorizeのverifyコードを用意し。

[ABC420 G - sqrt(n²+n+X)](https://atcoder.jp/contests/abc420/tasks/abc420_g)では、
`(2m+2n+1)(2m-2n-1)=4X-1`へ変形し、`signed_divisors(4*X-1)`で候補を列挙する。
`verify/atcoder_abc420_g.cpp`で公式sampleを確認済み。
