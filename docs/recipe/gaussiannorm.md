---
title: "ガウス整数のnorm剰余分布"
documentation_of: //cp/recipe/gaussiannorm.hpp
---

# ガウス整数のnorm剰余分布


## 概要

素因数分解`N=product(p^e)`から、`x^2+y^2=N`を満たす整数組を法`C`の剰余ごとに数える。
返り値の`result[x*C+y]`が`x mod C, y mod C`となる解数である。二平方和の総数だけでなく、
格子円の中心が有理数で合同条件が付く場合にも使える。

## 厳密な定義

- `gaussian_norm_residue_distribution`: O(r C^4 + sum sqrt(p))。norm(z)=Nとなるガウス整数zの法Cでの剰余分布を返す。

## Include

```cpp
#include "recipe/gaussiannorm.hpp"

vector<pair<int, unsigned long long>> factors{{2, 1}, {5, 2}};
auto count = gaussian_norm_residue_distribution<modint998244353>(factors, C);
```

4で割って3余る素数の指数が奇数なら全要素0を返す。入力は相異なる素数の正しい素因数分解を
仮定する。法を`C`、4で割って1余る素因数の種類数を`r`として、時間計算量は
`O(r C^4 + sum sqrt(p))`、領域は`O(C^4)`。巨大指数は有限monoidの周期へ圧縮する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `gaussian_norm_residue_distribution`

```cpp
template <class Mint> std::vector<Mint> gaussian_norm_residue_distribution( const std::vector<std::pair<int, unsigned long long>>& factorization, int modulus )
```

recipe。O(r C^4 + sum sqrt(p))。norm(z)=Nとなるガウス整数zの法Cでの剰余分布を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC444 G - Kyoen](https://atcoder.jp/contests/abc444/tasks/abc444_g)では、
中心の分母を払うと`x^2+y^2=N`と二つの合同条件になる。
`verify/atcoder_abc444_g.cpp`は本APIで分布を一度求め、対象剰余を参照するだけである。
公式4サンプルを確認済み。
