---
title: "二列間の絶対差和"
documentation_of: //cp/algorithm/pairdistance.hpp
---

# 二列間の絶対差和

- Header: `cp/algorithm/pairdistance.hpp`
- Symbol: `poe::sum_cross_absolute`
- Status: experimental

二列から一要素ずつ選ぶ全組について絶対差を合計する。片方をsortし、各要素の左右を二分探索する。

```cpp
#include "algorithm/pairdistance.hpp"
long long answer = poe::sum_cross_absolute(a, b);
```

`O((n+m) log n)`時間・`O(n)`領域。返り値型`T`は積と総和を保持できる幅にする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `sum_cross_absolute`

```cpp
template <class T> T sum_cross_absolute(std::vector<T> left, const std::vector<T>& right)
```

二列から一つずつ選ぶ全組の絶対差和を返す: sum_cross_absolute(a, b); O((n+m)log n)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC437 D - Sum of Differences](https://atcoder.jp/contests/abc437/tasks/abc437_d)の
全組絶対差を直接計算できる。`verify/atcoder_abc437_d.cpp`は公式サンプル一致。
