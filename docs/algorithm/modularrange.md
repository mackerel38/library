---
title: "剰余類の最小幅"
documentation_of: //cp/algorithm/modularrange.hpp
---

# 剰余類の最小幅


## 概要

各整数へ同じmodulusの任意の倍数を個別に足し引きできるとき、最終値の`max-min`を最小化する。
剰余を円周上の点とみなし、全点を含む最短弧、すなわちmodulusから最大空隙を引いた長さを返す。

## 厳密な定義

- `minimum_modular_range`: O(n log n)。各値にmodulusの倍数を足し引きしてmax-minを最小化した値を返す。

## Include

```cpp
#include "algorithm/modularrange.hpp"

long long answer = poe::minimum_modular_range(values, modulus);
```

`O(n log n)`時間、`O(n)`領域。負の入力も正規化する。空列・一要素列は0を返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimum_modular_range`

```cpp
template <std::integral T> T minimum_modular_range(std::vector<T> values, T modulus)
```

O(n log n)。各値にmodulusの倍数を足し引きしてmax-minを最小化した値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC450 D - Minimize Range](https://atcoder.jp/contests/abc450/tasks/abc450_d)へ直接使える。
`verify/atcoder_abc450_d.cpp`に提出用コードを用意し、公式sampleを確認済み。
