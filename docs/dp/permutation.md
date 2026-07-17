---
title: "順列挿入DP"
documentation_of: //cp/dp/permutation.hpp
---

# 順列挿入DP

- Header: `cp/dp/permutation.hpp`
- Symbol: `poe::insertion_component_counts`
- Status: experimental

## どんな問題に使えるか

相異なる値を小さい順に最終順列へ挿入し、すでに挿入した要素同士の隣接辺でできる
連結成分数だけを状態にする順列数え上げに使う。新要素は次のいずれかになる。

- 既存成分へ接続せず、新成分を作る。挿入位置は`components+1`通り。
- 一つの既存成分の左端または右端へ接続する。`2*components`通り。
- 二つの隣接成分をつなぐ。`components-1`通り。

```cpp
#include "dp/permutation.hpp"

auto counts = poe::insertion_component_counts<mint>(
    n,
    [&](int inserted, int components, int joined) {
        return allowed(inserted, components, joined);
    }
);
```

戻り値の`counts[c]`は全要素挿入後に`c`成分となる許可済み順列数。
callbackは各遷移について一度呼ばれ、値や閾値など問題側の制約だけを判定する。
成分間を分離する未挿入要素が足りない状態はライブラリが自動で除外する。
同値要素を持つ多重集合にはそのまま使えない。

## 計算量

`O(n^2)`時間、`O(n)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `insertion_component_counts`

```cpp
template<class T, class Allow> std::vector<T> insertion_component_counts(int size, Allow allow)
```

O(n^2)。値を昇順に順列へ挿入し、隣接済み成分数ごとの並べ方数を返す。
allow(i,c,j): i個挿入済み・c成分から、新要素がj個の成分へ接続する遷移を許すか。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC313 Ex - Group Photo](https://atcoder.jp/contests/abc313/tasks/abc313_h)では、
前列の身長を昇順に挿入する。挿入済み`i`人が`j`成分なら、後列との比較に必要な
閾値列の確定数が`i+j`だけで決まるため、callbackでその遷移を許可するか判定できる。
`verify/atcoder_abc313_h.cpp`で公式sampleを確認済み。judge ACは未確認。
小さい順列を全列挙し、ランダムな遷移許可表との比較も実施済み。
