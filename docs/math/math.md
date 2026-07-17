---
title: "整数数学関数"
documentation_of: //cp/math/math.hpp
---

# 整数数学関数

## Include

```cpp
#include "math/math.hpp"
```

- `pow_mod`: `O(log exponent)`。
- `inv_mod`: `O(log mod)`、互いに素であること。
- `crt`: `O(k log lcm)`、解なしは`{0,0}`。
- `floor_sum`: `O(log mod)`、負の係数にも対応。
- `floor_sqrt`: `O(1)`、非負64bit整数の平方根のfloor。
- `is_square`: `O(1)`、非負64bit整数が平方数か判定。

`floor_sqrt`は浮動小数点平方根を初期値に使い、`unsigned __int128`で前後を補正するため、
64bit境界付近でも丸め誤差を残さない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `pow_mod`

```cpp
inline long long pow_mod(long long base, long long exponent, int mod)
```

O(log exponent)。base^exponent mod modを返す。exponent>=0、mod>=1。

### `inv_mod`

```cpp
inline long long inv_mod(long long x, long long mod)
```

O(log mod)。xのmod逆元を[0, mod)で返す。gcd(x, mod)=1、mod>=1。

### `crt`

```cpp
inline std::pair<long long, long long> crt(const std::vector<long long>& remainders, const std::vector<long long>& moduli)
```

O(n log lcm)。x≡remainders[i](mod moduli[i])を解き、{x,lcm}を返す。解なしは{0,0}。

### `floor_sum`

```cpp
inline long long floor_sum(long long n, long long mod, long long a, long long b)
```

O(log mod)。sum floor((a*i+b)/mod)をmod 2^64で返す。0<=n<2^32、1<=mod<2^32。

### `floor_sqrt`

```cpp
inline unsigned long long floor_sqrt(unsigned long long x)
```

O(1)。非負64bit整数xに対してfloor(sqrt(x))を誤差なく返す。

### `is_square`

```cpp
inline bool is_square(unsigned long long x)
```

O(1)。xが平方数ならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Sum of Floor of Linear](https://judge.yosupo.jp/problem/sum_of_floor_of_linear)。

```cpp
while (test_count--) {
    long long n, mod, a, b;
    std::cin >> n >> mod >> a >> b;
    std::cout << poe::floor_sum(n, mod, a, b) << '\n';
}
```

`verify/math.test.cpp`で2026-07-15に全ケースAC確認済み。

[AtCoder ABC428 D - 183184](https://atcoder.jp/contests/abc428/tasks/abc428_d)では、
連結後の値域に含まれる平方数の個数を、両端の`floor_sqrt`の差で求める。
`verify/atcoder_abc428_d.cpp`で公式sampleとの一致を確認済み。
