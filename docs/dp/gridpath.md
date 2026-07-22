---
title: "グループ別右下単調グリッド経路"
documentation_of: //cp/dp/gridpath.hpp
---

# グループ別右下単調グリッド経路

## 概要

グリッド上の点が複数のグループに分かれているとき、同じグループの点を始点・終点にする右・下方向の経路数を合計する。
点が少ないグループには全点対、多いグループには盤面DPを自動的に使うため、ラベル頻度の偏りを意識して実装を分ける必要がない。

## 厳密な定義

高さ $H$、幅 $W$ のグリッドと、相異なる点からなる点集合 $S_1,\ldots,S_c$を考える。
点 $u=(r_u,c_u)$ から $v=(r_v,c_v)$ へ、行または列を1増やす移動だけで到達する経路数を $P(u,v)$ とする。
到達不能なら $P(u,v)=0$、$u=v$なら空の移動を一つと数えて $P(u,u)=1$ とする。

`count_grouped_monotone_grid_paths`は

$$
\sum_{g=1}^{c}\sum_{u\in S_g}\sum_{v\in S_g}P(u,v)
$$

を法`mod`で返す。異なるグループに同じ座標が現れてもよいが、一つのグループ内では座標を重複させてはならない。

## Include

```cpp
#include "dp/gridpath.hpp"
```

## 使い方

```cpp
vector<vector<poe::gridpathpoint>> groups = {
    {{0, 0}, {1, 1}},
    {{0, 1}, {1, 0}},
};

auto answer = poe::count_grouped_monotone_grid_paths<998244353>(
    2, 2, groups
);
cout << answer << '\n'; // 6
```

座標は0-indexedで、`gridpathpoint{row, column}`の順に渡す。
二点が固定された場合の経路数には

$$
P(u,v)=\binom{(r_v-r_u)+(c_v-c_u)}{r_v-r_u}
$$

を使う。

## 計算量

盤面面積を $A=HW$、全グループの点数の和を $p$ とする。
疎密の閾値を $\lfloor\sqrt A\rfloor$ として、時間計算量は

$$
O\left(p\sqrt A+\frac{pA}{\sqrt A}\right),
$$

空間計算量は $O(A+p)$。$N\times N$盤面の全マスがいずれかのグループに属する場合は $O(N^3)$時間、$O(N^2)$領域になる。
また、$H+W-2<\mathrm{mod}$が必要である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `gridpathpoint`

```cpp
struct gridpathpoint
```

count_grouped_monotone_grid_pathsへ渡す0-indexedの行・列座標。

### `count_grouped_monotone_grid_paths`

```cpp
template <int mod> staticmodint<mod> count_grouped_monotone_grid_paths( int height, int width, const std::vector<std::vector<gridpathpoint>>& groups )
```

O(p sqrt(hw)+p hw/sqrt(hw))時間・O(hw+p)領域。同じgroup内の二点を結ぶ右下単調path数の総和。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC259 Ex - Yet Another Path Counting](https://atcoder.jp/contests/abc259/tasks/abc259_h)では、
同じラベルのマスを一つのグループとして渡す。

`verify/atcoder_abc259_h.cpp`に提出用コードを収録している。
