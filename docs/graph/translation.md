---
title: "一斉平行移動による点集合消去"
documentation_of: //cp/graph/translation.hpp
---

# 一斉平行移動による点集合消去

- Header: `cp/graph/translation.hpp`
- Symbol: `poe::minimum_safe_translation_clear_steps`
- Status: experimental

## どんな問題に使えるか

長方形grid内の点を、毎回すべて同じ方向へ一マス移動する。盤外へ出た点は消え、
禁止マスへ移動した点が一つでもあればその操作列は失敗とする。全点を消す最小操作数を求める。

```cpp
#include "graph/translation.hpp"

int answer = minimum_safe_translation_clear_steps(
    height, width, occupied, forbidden
);
```

既定の移動は上下左右。最後の引数へ`array<gridpoint,K>`を渡すと方向集合を変更できる。
初期状態が禁止マスと重なる場合や、安全な消去方法がない場合は`-1`を返す。

一斉移動後に残る点は、初期点集合を元座標上の長方形へ切り詰め、累積平行移動したものになる。
そこで状態を`top,bottom,left,right,row_shift,column_shift`の6整数へ圧縮する。
残存点の有無は初期盤面の二次元累積和、禁止点との衝突は平行移動の逆像で判定する。

## 計算量

方向数を`K`、禁止点数を`F`とすると、状態数の粗い上界は`O(h^3w^3)`。
時間`O(h^3w^3 K(F+1))`、領域`O(h^3w^3+hw)`。
高さ・幅が十数程度で、一斉移動により形が平行移動と切り取りだけで変化する問題に使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimum_safe_translation_clear_steps`

```cpp
template <std::size_t K> int minimum_safe_translation_clear_steps( int height, int width, const std::vector<gridpoint>& occupied, const std::vector<gridpoint>& forbidden, const std::array<gridpoint, K>& directions )
```

O(h^3 w^3 K(F+1))。点集合を同時移動し、禁止点を踏まず盤外へ全消去する最小回数を返す。

### `minimum_safe_translation_clear_steps`

```cpp
inline int minimum_safe_translation_clear_steps( int height, int width, const std::vector<gridpoint>& occupied, const std::vector<gridpoint>& forbidden )
```

O(h^3 w^3 F)。上下左右への一斉移動で全点を安全に消す最小回数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC427 E - Wind Cleaning](https://atcoder.jp/contests/abc427/tasks/abc427_e):
  ゴミを初期点、高橋君の位置を禁止点として使う。

`verify/atcoder_abc427_e.cpp`に提出用コードを収録し、公式sample 3件との一致を確認済み。
judge ACは未確認。

`tests/property/graph/translation.cpp`では、小さい盤面の点集合bit mask BFSと比較する。
