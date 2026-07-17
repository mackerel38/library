---
title: "Li Chao Tree"
documentation_of: //cp/structure/linecontainer.hpp
---

# Li Chao Tree

- Header: `cp/structure/linecontainer.hpp`
- Symbol: `poe::line<T>`, `poe::minlinecontainer<T>`, `poe::maxlinecontainer<T>`
- Status: experimental

## どんな問題に使えるか

DP遷移が`dp[i]=min_j(a[j]*x[i]+b[j])`またはmax版に変形でき、問い合わせ座標を先に列挙できる問題に使う。
傾きの追加順や`x`の問い合わせ順に単調性を要求しない。直線が有効な添字区間だけの追加もできる。

問い合わせ座標を事前に集められない場合は動的座標版が必要。二次関数のままでは使えず、共通項を
外へ出して直線に変形できる必要がある。

## API・計算量

```cpp
#include "structure/linecontainer.hpp"

poe::minlinecontainer<long long> lines(x);
lines.add_line({slope, intercept});
long long best = lines.get(x[i]);
```

全域直線追加・一点取得`O(log n)`、区間直線追加`O(log^2 n)`、領域`O(n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `line`

```cpp
template <class T> struct line
```

y=ax+bを表す直線。

### `get`

```cpp
T get(const T& x) const
```

O(1)。xでの値を返す。

### `minlinecontainer`

```cpp
template <class T> using minlinecontainer = detail::discrete_linecontainer<T, std::less<T>>
```

最小値Li Chao Tree: minlinecontainer<long long> lines(xs); 追加O(log n)、取得O(log n)。

### `maxlinecontainer`

```cpp
template <class T> using maxlinecontainer = detail::discrete_linecontainer<T, std::greater<T>>
```

最大値Li Chao Tree: maxlinecontainer<long long> lines(xs); 追加O(log n)、取得O(log n)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[EDPC Z - Frog 3](https://atcoder.jp/contests/dp/tasks/dp_z)の二次式を展開して使える。
`verify/atcoder_dp_z.cpp`で2026-07-15にjudge AC確認済み。
ランダム直線を全走査する愚直解とも照合済み。
