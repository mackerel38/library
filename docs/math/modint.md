---
title: "`modint`"
documentation_of: //cp/math/modint.hpp
---

# `modint`

## Include

```cpp
#include "math/modint.hpp"
```

固定法の`staticmodint<mod>`と実行時法の`dynamicmodint<id>`を提供する。
四則演算は`O(1)`、逆元と除算は`O(log mod)`。除数と法は互いに素であること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `staticmodint`

```cpp
template <int modulus> struct staticmodint
```

固定modの整数: staticmodint<998244353> x; 四則演算O(1)、除算O(log mod)。

### `staticmodint`

```cpp
staticmodint() = default
```

O(1)。0で初期化する。

### `staticmodint`

```cpp
template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0> staticmodint(T value)
```

O(1)。valueをmodulusで正規化して代入する。

### `raw`

```cpp
static staticmodint raw(unsigned int value)
```

O(1)。剰余を取らずvalueを格納する。0<=value<mod()が必要。

### `mod`

```cpp
static constexpr int mod()
```

O(1)。法を返す。

### `val`

```cpp
unsigned int val() const
```

O(1)。[0, mod())の値を返す。

### `operator++`

```cpp
staticmodint& operator++()
```

O(1)。++xを行う。

### `operator++`

```cpp
staticmodint operator++(int)
```

O(1)。x++を行う。

### `operator--`

```cpp
staticmodint& operator--()
```

O(1)。--xを行う。

### `operator--`

```cpp
staticmodint operator--(int)
```

O(1)。x--を行う。

### `pow`

```cpp
staticmodint pow(long long exponent) const
```

O(log exponent)。x^exponentを返す。exponent>=0。

### `inv`

```cpp
staticmodint inv() const
```

O(log mod)。乗法逆元を返す。gcd(val(), mod())=1が必要。

### `dynamicmodint`

```cpp
template <int id> struct dynamicmodint
```

実行時modの整数: dynamicmodint<id>::set_mod(mod); 四則演算O(1)、除算O(log mod)。

### `dynamicmodint`

```cpp
dynamicmodint() = default
```

O(1)。0で初期化する。

### `dynamicmodint`

```cpp
template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0> dynamicmodint(T value)
```

O(1)。valueをmod()で正規化して代入する。

### `set_mod`

```cpp
static void set_mod(int modulus)
```

O(1)。以後の法を設定する。modulus>=1。

### `mod`

```cpp
static int mod()
```

O(1)。現在の法を返す。

### `raw`

```cpp
static dynamicmodint raw(unsigned int value)
```

O(1)。剰余を取らずvalueを格納する。0<=value<mod()が必要。

### `val`

```cpp
unsigned int val() const
```

O(1)。[0, mod())の値を返す。

### `pow`

```cpp
dynamicmodint pow(long long exponent) const
```

O(log exponent)。x^exponentを返す。exponent>=0。

### `inv`

```cpp
dynamicmodint inv() const
```

O(log mod)。乗法逆元を返す。gcd(val(), mod())=1が必要。

### `modint998244353`

```cpp
using modint998244353 = staticmodint<998244353>
```

法998244353の固定modint: modint998244353 value; 各演算量はstaticmodintと同じ。

### `modint1000000007`

```cpp
using modint1000000007 = staticmodint<1000000007>
```

法1000000007の固定modint: modint1000000007 value; 各演算量はstaticmodintと同じ。

### `modint`

```cpp
using modint = dynamicmodint<-1>
```

既定idの実行時modint: modint::set_mod(modulus); 各演算量はdynamicmodintと同じ。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum)で
一次変換と区間和を法`998244353`で保持する。

```cpp
using mint = poe::modint998244353;
mint value = input;
value = multiply * value + add;
std::cout << value.val() << '\n';
```

`verify/lazysegtree.test.cpp`と`verify/preset_range_affine_range_sum.test.cpp`で使用し、
2026-07-15に両方の全ケースAC確認済み。
