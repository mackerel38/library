---
title: "RLE十進整数"
documentation_of: //cp/math/decimal.hpp
---

# RLE十進整数

- Header: `cp/math/decimal.hpp`
- Symbol: `poe::decimalrun`, `poe::decimal_runs_mod`, `poe::decimal_runs_div_mod`,
  `poe::count_divisible_concatenations`, `poe::smallest_multiple_containing`
- Status: experimental

## どんな問題に使えるか

同じ数字が非常に長く連続する十進整数を、文字列へ復元せずに剰余計算する。
各runを`decimalrun{digit,length}`で渡し、整数そのものの剰余に加えて
`floor(N/divisor) mod modulus`も求められる。

```cpp
#include "math/decimal.hpp"

vector<decimalrun> runs{{3, 1}, {1, 1}, {6, 1}, {2, 2}};
auto remainder = decimal_runs_mod(runs, mod);
auto quotient = decimal_runs_div_mod(runs, divisor, mod);
```

長さ`L`の同一数字追加をaffine変換`x -> 10^L x + digit * 11...1`として二分累乗する。
run数を`K`、最大run長を`L`として`O(K log L)`時間、`O(1)`追加領域。
`divisor * modulus`が`unsigned long long`に収まること。

`count_divisible_concatenations(values, modulus, base)`は、正整数をbase進表記で二つ連結した値が
`modulus`の倍数になる順序付きpair数を返す。最大桁数を`d`として`O(n d log n)`時間、`O(n)`領域。

`smallest_multiple_containing(modulus,pattern)`は、先頭が0でない数字列`pattern`を部分文字列として含む
最小の正の十進`modulus`倍数を文字列で返す。`modulus<=1e9`が必要。答えの前後に追加する高々`d`桁を
prefixとsuffixへ分け、短い側を列挙して合同式を解く。`d=ceil(log10 modulus)`として
`O(|pattern|d+d^2+10^(d/2) log modulus)`時間、`O(1)`追加領域が目安。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `decimalrun`

```cpp
struct decimalrun
```

十進表記の同一数字run: decimalrun{digit,length}。

### `decimal_runs_mod`

```cpp
inline unsigned long long decimal_runs_mod( const std::vector<decimalrun>& runs, unsigned long long modulus )
```

O(K log L)。RLE十進整数をmodulusで割った余りを返す。

### `decimal_runs_div_mod`

```cpp
inline unsigned long long decimal_runs_div_mod( const std::vector<decimalrun>& runs, unsigned long long divisor, unsigned long long modulus )
```

O(K log L)。RLE十進整数Nについてfloor(N/divisor) mod modulusを返す。

### `count_divisible_concatenations`

```cpp
inline long long count_divisible_concatenations( const std::vector<unsigned long long>& values, unsigned long long modulus, unsigned long long base = 10 )
```

O(n d log n)。base進表記を連結した値がmodulusの倍数となる順序付きpair数を返す。

### `smallest_multiple_containing`

```cpp
inline std::string smallest_multiple_containing(int modulus, const std::string& pattern)
```

O(|pattern|d+d 10^(d/2)log modulus)。patternを含む最小の正の十進modulus倍数を返す。modulus<=1e9。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC448 E - Simple Division](https://atcoder.jp/contests/abc448/tasks/abc448_e)へ
`decimal_runs_div_mod(runs, M, 10007)`を直接使える。
`verify/atcoder_abc448_e.cpp`に提出用コードを用意し、公式sampleを確認済み。

[AtCoder ABC433 D - 183183](https://atcoder.jp/contests/abc433/tasks/abc433_d)へ
`count_divisible_concatenations(values, M)`を直接使える。
`verify/atcoder_abc433_d.cpp`で公式sampleを確認済み。

[AtCoder ABC423 G - Small Multiple 2](https://atcoder.jp/contests/abc423/tasks/abc423_g)へ
`smallest_multiple_containing(K,S)`を直接使える。
`verify/atcoder_abc423_g.cpp`で公式sampleを確認済み。
