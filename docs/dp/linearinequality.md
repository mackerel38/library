---
title: "重み付き非負整数解の個数"
documentation_of: //cp/dp/linearinequality.hpp
---

# 重み付き非負整数解の個数

- Header: `cp/dp/linearinequality.hpp`
- Symbol: `poe::count_nonnegative_weighted_sum_at_most`
- Status: experimental

## どんな問題に使えるか

正の重み`a[i]`と巨大な上限`M`に対し、`sum(a[i] * x[i]) <= M`を満たす非負整数列`x`を数える。
各変数を`x=2q+r`へ分け、`r`の重み分布との畳み込みを行いながら上限を半分へ縮約する。
重みの総和が小さい場合に向き、負の重み・変数上限・複数不等式には対応しない。

## API・計算量

```cpp
#include "dp/linearinequality.hpp"

auto count = poe::count_nonnegative_weighted_sum_at_most<998244353>(weights, limit);
```

重み総和を`S`として概ね`O(S log S log M)`時間・`O(S)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_nonnegative_weighted_sum_at_most`

```cpp
template <int mod = 998244353> staticmodint<mod> count_nonnegative_weighted_sum_at_most( const std::vector<int>& weights, unsigned long long limit)
```

非負整数列xでsum(weights[i]*x[i])<=limitとなる個数を返す; sum(weights)=SとしてO(S log S log limit)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC436 G - Linear Inequation](https://atcoder.jp/contests/abc436/tasks/abc436_g)へ直接使える。
`verify/atcoder_abc436_g.cpp`は公式サンプル一致を確認済み、judge未提出。
