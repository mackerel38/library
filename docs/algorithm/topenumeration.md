---
title: "上位解列挙"
documentation_of: //cp/algorithm/topenumeration.hpp
---

# 上位解列挙

- Header: `cp/algorithm/topenumeration.hpp`
- Symbol: `poe::top_multiset_sums`
- Status: experimental

## どんな問題に使えるか

各種類を何回でも選べる値の集合から、ちょうど`picks`個選んだ多重集合の和を
大きい順に`count`個まで列挙する。異なる種類は値が同じでも区別するため、同じ和も重複して返る。

```cpp
#include "algorithm/topenumeration.hpp"

auto answer = poe::top_multiset_sums(values, picks, count);
```

計算量は値の種類数を`n`、実際に返す個数を`x`として
`O(n log n + x log x)`時間、`O(n+x)`領域。
全多重集合数が`count`未満なら存在するものだけを返す。`picks=0`では空多重集合の和`0`だけが存在する。

## 仕組み

値を降順に並べ、最大値だけを`picks`個選ぶ状態から開始する。
各状態の「末尾の非0種類」から一個を次の種類へ移す操作を親子関係として定めると、
各多重集合はただ一つの親を持つ。したがって訪問済み集合なしのbest-first searchで列挙できる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `top_multiset_sums`

```cpp
template <class T> std::vector<T> top_multiset_sums( std::vector<T> values, long long picks, int count )
```

O(n log n+x log x)。valuesからpicks個重複選択した多重集合和を大きい順にcount個まで返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC440 E - Cookies](https://atcoder.jp/contests/abc440/tasks/abc440_e)は、
クッキー種類ごとの美味しさを`values`としてそのまま利用できる。
`verify/atcoder_abc440_e.cpp`で公式sampleを確認済み。

property testでは小さな入力の全多重集合列挙と、重複を含む出力列全体を比較している。
