---
title: "rangesort"
documentation_of: //cp/algorithm/rangesort.hpp
---

# `rangesort`

## 概要

相異なる値の列へ、指定区間の昇順・降順sortを繰り返した後、注目する一要素がどこへ移動したかを求める。

## 厳密な定義

相異なる値からなる列 $A$ と $A$ に一度だけ現れる値 $x$ がある。
各queryで半開区間 $[l_i,r_i)$ を昇順または降順に並べ替える。
全queryを入力順に適用した後、$A_p=x$ となる添字 $p$ を返す。

## Include

```cpp
#include "algorithm/rangesort.hpp"
```

## API

```cpp
struct rangesortquery {
    int left;
    int right;
    bool ascending;
};

int position_after_range_sorts(values, target, queries);
```

- 区間は半開区間 `[left, right)`。
- `ascending == true` なら昇順、`false`なら降順。
- `values` の要素は相異なり、`target` はちょうど一度現れる必要がある。
- 返り値は0-indexedの位置。

各要素を `target` より小さいかだけで二値化する。区間内の小さい要素数が分かれば、sort後の二値列と、
区間内に `target` がある場合の新しい位置を一意に決められる。区間代入・区間和segtreeを用い、
列長を $n$、query数を $q$ とすると計算量は $O((n+q)\log n)$、使用メモリは $O(n)$。

## 使用例

[AtCoder ABC237 G - Range Sort Query](https://atcoder.jp/contests/abc237/tasks/abc237_g)では、query種別1を昇順、2を降順として渡す。

```cpp
cout << position_after_range_sorts(values, target, queries) + 1 << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rangesortquery`

```cpp
struct rangesortquery
```

range sort一回を表す半開区間と向き。ascending=trueなら昇順、falseなら降順。

### `position_after_range_sorts`

```cpp
template<class T> int position_after_range_sorts( const std::vector<T>& values, const T& target, const std::vector<rangesortquery>& queries )
```

O((n+q)log n)。相異なる値の列へrange sortを順に行った後のtarget位置を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
