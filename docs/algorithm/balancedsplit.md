---
title: "均等二分の最終分布"
documentation_of: //cp/algorithm/balancedsplit.hpp
---

# 均等二分の最終分布

- Header: `cp/algorithm/balancedsplit.hpp`
- Symbol: `poe::balanced_split_leaves`
- Status: experimental

## どんな問題に使えるか

長さ`length`の一要素を、値が`threshold`以上である間、`floor(x/2)`と`ceil(x/2)`へ
分割し続けた最終multisetを求める。要素を一個ずつ生成せず、`{値, 個数}`へ圧縮して返す。
棒や資源の均等分割後に、一定値以上の個数、短い方から`K`個の総和、分割回数を調べる問題に使える。

```cpp
#include "algorithm/balancedsplit.hpp"

auto leaves = balanced_split_leaves(19, 9);
// {{4, 1}, {5, 3}}
```

返り値は値の昇順である。`length >= 1`、`threshold >= 2`が必要。
値の種類数は`O(log length)`で、計算量は`O(log^2 length)`時間、`O(log length)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `balanced_split_leaves`

```cpp
inline std::vector<std::pair<long long, long long>> balanced_split_leaves( long long length, long long threshold )
```

O(log^2 length)。length以上ではなくthreshold以上のpieceを半分へ分割し続けた{長さ,個数}を返す。

### `kth_largest_after_max_halving`

```cpp
template <std::ranges::input_range Range> long double kth_largest_after_max_halving( const Range& values, long long operations, long long rank )
```

O(n log A + n log n)。最大要素をK回半分へ分割した後のX番目に大きい値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC444 F - Half and Median](https://atcoder.jp/contests/abc444/tasks/abc444_f)では、
中央値候補を`X`として`threshold=max(2,2X-1)`まで分割する。返された分布から長さ`X`以上の
piece数と短い方から必要個数の総和を求める。さらに、必要本数へ増やす最小分割回数と、
残りの長さを1まで分割して消費できる最大回数の両側から、ちょうど`M`回が可能か判定する。
`verify/atcoder_abc444_f.cpp`で公式sampleを確認済み。

## 最大要素の反復二等分

`kth_largest_after_max_halving(values, operations, rank)`は、正の値のmultisetについて
「現在の最大要素を一つ取り出し、同じ大きさの半分二つへ置換する」操作を指定回数行った後、
大きい方から`rank`番目の値を返す。

```cpp
long double answer =
    kth_largest_after_max_halving(lengths, operations, rank);
```

最終最大値を二分探索し、各初期要素をその値以下へするための完全二分木の内部頂点数を数える。
その後、最終最大値と等しいpieceへ残りの操作をまとめて適用し、値と個数の圧縮分布から順位を選ぶ。
計算量は`O(n log A + n log n)`時間・`O(n)`領域。`log A`は固定120回の実数二分探索を表す。

[AtCoder ABC424 E - Cut in Half](https://atcoder.jp/contests/abc424/tasks/abc424_e)へ直接使える。
`verify/atcoder_abc424_e.cpp`で公式sample一致。
property testでは小さな整数列と操作回数をpriority queueの逐次実行と照合する。
