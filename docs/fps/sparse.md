---
title: "疎な形式的冪級数"
documentation_of: //cp/fps/sparse.hpp
---

# 疎な形式的冪級数


## 概要

非零係数が少ない多項式の巨大な整数乗や、`1+c*x^d`の乗除を打ち切り次数まで処理する。
密なNewton法より疎な項数`K`を使う`O(nK)`が小さい場合に選ぶ。

## 厳密な定義

- `sparse_power`: O(n * nonzero(f))。sparse_power(f,k,n): 非零項が少ないfのf^k mod x^nを返す。 k>=0、先頭非零係数を除いた後の定数項が非0、n<mod。
- `multiply_binomial`: O(n)。multiply_binomial(f,d,c,n): f(x)(1+c*x^d) mod x^nを返す。
- `divide_binomial`: O(n)。divide_binomial(f,d,c,n): f(x)/(1+c*x^d) mod x^nを返す。

```cpp
auto power = poe::sparse_power<998244353>(f, exponent, size);
dp = poe::divide_binomial(std::move(dp), coin, mint{-1}, size);
```

`sparse_power`は`n<mod`を要求する。指数と先頭次数の積が`n`以上なら0を返す。
binomial乗除の`degree`は正であること。

## Include

```cpp
#include "fps/sparse.hpp"
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sparse_power`

```cpp
template<int mod> fps<mod> sparse_power(const fps<mod>& polynomial, long long exponent, int size)
```

O(n * nonzero(f))。sparse_power(f,k,n): 非零項が少ないfのf^k mod x^nを返す。
k>=0、先頭非零係数を除いた後の定数項が非0、n<mod。

### `multiply_binomial`

```cpp
template<int mod> fps<mod> multiply_binomial(fps<mod> series, int degree, staticmodint<mod> coefficient, int size)
```

O(n)。multiply_binomial(f,d,c,n): f(x)(1+c*x^d) mod x^nを返す。

### `divide_binomial`

```cpp
template<int mod> fps<mod> divide_binomial(fps<mod> series, int degree, staticmodint<mod> coefficient, int size)
```

O(n)。divide_binomial(f,d,c,n): f(x)/(1+c*x^d) mod x^nを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [FPS 24題 A - お菓子](https://atcoder.jp/contests/fps-24/tasks/fps_24_a): 4項多項式の整数乗。
- [FPS 24題 G - 硬貨](https://atcoder.jp/contests/fps-24/tasks/fps_24_g): 使用可能硬貨の追加・削除。

密な`fps::pow`および愚直更新と比較する。
