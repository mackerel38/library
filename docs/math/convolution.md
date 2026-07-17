---
title: "畳み込み"
documentation_of: //cp/math/convolution.hpp
---

# 畳み込み

## 概要

NTTと3法CRTによる畳み込みを扱う。

## 厳密な定義

- `convolution`: O(n log n)。modint列a,bの畳み込みを返す。modはNTT可能な素数。
- `convolution_ll`: O(n log n)。long long列を整数として畳み込む。全係数がlong long範囲内であること。


## Include

```cpp
#include "math/convolution.hpp"
```

`convolution`はNTT可能な素数modでは直接NTTを行う。NTTできないstatic modintも
3個のNTT素数とCRTを使って`O(n log n)`で畳み込めるが、各係数を非負整数として
計算した積和が3法の積未満であることを実行時に検査する。`convolution_ll`は3法CRTで
符号付き64bit整数の畳み込みを行い、全係数が`long long`範囲内であること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `convolution`

```cpp
template <int mod> std::vector<staticmodint<mod>> convolution(std::vector<staticmodint<mod>> a, std::vector<staticmodint<mod>> b)
```

O(n log n)。modint列a,bの畳み込みを返す。modはNTT可能な素数。

### `convolution`

```cpp
template <int mod = 998244353, class T, std::enable_if_t<std::is_integral_v<T>, int> = 0> std::vector<T> convolution(const std::vector<T>& a, const std::vector<T>& b)
```

O(n log n)。整数列a,bをmodで畳み込み、[0,mod)の整数列で返す。modはNTT可能な素数。

### `convolution_ll`

```cpp
inline std::vector<long long> convolution_ll(const std::vector<long long>& a, const std::vector<long long>& b)
```

O(n log n)。long long列を整数として畳み込む。全係数がlong long範囲内であること。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Convolution Mod](https://judge.yosupo.jp/problem/convolution_mod)。

```cpp
std::vector<poe::modint998244353> left(n), right(m);
auto answer = poe::convolution(left, right);
for (auto value : answer) std::cout << value.val() << ' ';
```

`verify/convolution.test.cpp`で2026-07-15に全ケースAC確認済み。
