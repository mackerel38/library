---
title: "latticemove"
documentation_of: //cp/geometry/latticemove.hpp
---

# `latticemove`

<!-- API REFERENCE: COLLAPSED -->

## 概要

整数格子上で二種類までの移動ベクトルを何回ずつ使えば目的地へ着くかを、探索範囲を広げずに求めます。
斜めを含む移動、ボタンごとに移動量が異なる盤面、二種類まで使える操作の最小回数に使えます。

## 厳密な定義

目標ベクトルを$T$、二つの移動ベクトルを$P,Q$とします。
`minimum_lattice_moves(T, P, Q)`は

$$
T=xP+yQ,\qquad x,y\in\mathbb Z_{\geq0}
$$

を満たす$x+y$の最小値を返します。存在しない場合は`std::nullopt`です。

`minimum_two_kind_lattice_moves(T, steps)`は、`steps`から高々二種類$P,Q$を選んだときの答えの最小値を返します。
一種類だけを使う解と、$T=(0,0)$で一度も移動しない解も含みます。三種類以上の併用は調べません。

## Include

```cpp
#include "geometry/latticemove.hpp"
```

## 最短の使用例

```cpp
auto count = minimum_lattice_moves(
    point<long long>{5, 3},
    point<long long>{1, 0},
    point<long long>{1, 1}
); // 5

vector<point<long long>> steps{{1, 0}, {0, 1}, {1, 1}};
auto best = minimum_two_kind_lattice_moves({5, 3}, steps); // 5
```

## API

### `minimum_lattice_moves`

```cpp
std::optional<long long> minimum_lattice_moves(
    const point<long long>& target,
    const point<long long>& first,
    const point<long long>& second
);
```

- `target`: 原点から目的地へのベクトルです。
- `first`, `second`: 一回で加える移動ベクトルです。零ベクトルや同一直線上の組も扱えます。
- 戻り値: 到達できるなら最小手数、できないなら`std::nullopt`です。
- 計算量: $O(1)$時間、$O(1)$追加領域です。

### `minimum_two_kind_lattice_moves`

```cpp
std::optional<long long> minimum_two_kind_lattice_moves(
    const point<long long>& target,
    std::span<const point<long long>> steps
);
```

- `steps`: 使用候補の移動ベクトル列です。同じベクトルが重複していても構いません。
- 戻り値: 高々二種類を使う最小手数です。到達できなければ`std::nullopt`です。
- 計算量: $k=\lvert\mathrm{steps}\rvert$として$O(k^2)$時間、$O(1)$追加領域です。

## 注意点

- `minimum_two_kind_lattice_moves`は、三種類以上を使う方が短い一般の移動集合には使えません。
- 内部の行列式と一次不定方程式は`__int128_t`で計算します。返す最小手数は`long long`に収まる必要があります。
- 到達不能を`-1`で出力するときは`answer.value_or(-1)`と書けます。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimum_lattice_moves`

```cpp
inline std::optional<long long> minimum_lattice_moves( const point<long long>& target, const point<long long>& first, const point<long long>& second )
```

O(1)。target=x*first+y*second、x,y>=0を満たす最小x+yを返し、存在しなければnullopt。

### `minimum_two_kind_lattice_moves`

```cpp
inline std::optional<long long> minimum_two_kind_lattice_moves( const point<long long>& target, std::span<const point<long long>> steps )
```

O(k^2)。stepsのうち高々2種類を反復してtargetへ到達する最小手数を返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC271 Ex - General General](https://atcoder.jp/contests/abc271/tasks/abc271_h)では、8方向移動の候補を絞った後の最小手数計算に使えます。完成形は`recipe/compassmove.hpp`にあります。

## 検証

成分が$-2$以上$2$以下の全移動ベクトル対と、狭い範囲の全目標点について、係数を列挙する愚直解と比較しています。
