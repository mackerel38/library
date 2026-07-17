---
title: "線形剰余指数の冪和"
documentation_of: //cp/math/modularsum.hpp
---

# 線形剰余指数の冪和

- Header: `cp/math/modularsum.hpp`
- Symbol: `poe::sum_power_mod_linear`
- Status: experimental

## どんな問題に使えるか

`sum_power_mod_linear(n,m,a,b,x,mod)`は

`sum_{i=0}^{n-1} x^((a*i+b) mod m) mod mod`

を求める。`mod`が合成数で`x`が逆元を持たない場合も使用できる。

法を、`x`と互いに素な部分と、十分大きい冪で`x^k=0`になる部分へ分ける。前者は
`floor_product`、後者は非零になる低い指数だけを逆像列挙し、最後にCRTで統合する。
`O(log(n+m+a+b+mod))`時間、`O(log)`再帰領域。`m,mod>=1`、`0<=a,b<m`、`0<=x<mod`。

```cpp
#include "math/modularsum.hpp"

cout << poe::sum_power_mod_linear(n, m, a, b, x, mod) << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sum_power_mod_linear`

```cpp
inline long long sum_power_mod_linear( unsigned long long n, unsigned long long m, unsigned long long a, unsigned long long b, unsigned long long base, int modulus )
```

O(log(n+m+a+b+mod))。sum base^((a*i+b) mod m), 0<=i<nをmodで返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC429 G - Sum of Pow of Mod of Linear](https://atcoder.jp/contests/abc429/tasks/abc429_g)へ
直接使う。`verify/atcoder_abc429_g.cpp`に収録し、公式sample確認済み。

## 検証

- `tests/property/math/modularsum.cpp`: 小さい合成数法を含む全引数を愚直累乗和と比較
- `tests/api/math/modularsum.cpp`: 逆元なし・`a=0`・`mod=1`を確認
