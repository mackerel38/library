---
title: "和と XOR の二次元畳み込み"
documentation_of: //cp/math/multiconvolution.hpp
---

# 和と XOR の二次元畳み込み

## 概要

一方の添字を通常の加算、もう一方のbitmask添字をXORで合成する二次元分布を、同じ分布から
指定回数選んだ結果へ高速にべき乗する。総和とXORを同時に管理するDPを一括計算できる。

## 厳密な定義

入力`base[s][x]`を$f_{s,x}$、非負整数`exponent`を$e$とする。返り値$g$は

$$
g_{s,x}=\sum_{\substack{s_1+\cdots+s_e=s\\x_1\mathbin{\mathrm{xor}}\cdots
\mathbin{\mathrm{xor}}x_e=x}}\prod_{i=1}^e f_{s_i,x_i}
$$

で定義される。$e=0$では`g[0][0] = 1`だけが非零となる。

## Include

```cpp
#include "math/multiconvolution.hpp"
```

## 使い方

```cpp
using mint = poe::modint998244353;
vector<vector<mint>> base = {
    {1, 2}, // sum=0, xor=0,1
    {3, 4}  // sum=1, xor=0,1
};

auto result = poe::sum_xor_convolution_power<998244353>(base, 2);
cout << result[1][0] << '\n'; // 22
```

`base`は空でなく、全行の長さは同じ2冪でなければならない。和添字は0以上である。
返り値の行数は`(base.size() - 1) * exponent + 1`、列数は`base[0].size()`となる。
必要なNTT長が`mod - 1`を割り切るNTT可能な素数法を使う。

## 計算量

XOR添字数を$K$、返り値の和添字数以上の最小の2冪を$L$として、
時間計算量は$O(KL(\log L+\log K+\log e))$、領域計算量は$O(KL)$。

## 使用例

[AtCoder ABC265 Ex - No-capture Lance Game](https://atcoder.jp/contests/abc265/tasks/abc265_h)では、
一行の局面を超現実数成分$s$とGrundy数$g$の分布にする。$s$を非負添字へ平行移動して
`sum_xor_convolution_power(row, H)`を呼ぶと、$H$行の$\sum s_i$と$\bigoplus g_i$の
同時分布が得られる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sum_xor_convolution_power`

```cpp
template <int mod> std::vector<std::vector<staticmodint<mod>>> sum_xor_convolution_power( const std::vector<std::vector<staticmodint<mod>>>& base, long long exponent )
```

O(KL(log L+log K+log exponent))時間・O(KL)領域。
sum添字は加算、mask添字はXORする二次元分布のexponent乗を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
