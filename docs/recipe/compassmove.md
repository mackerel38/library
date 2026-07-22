---
title: "compassmove"
documentation_of: //cp/recipe/compassmove.hpp
---

# `compassmove`

<!-- API REFERENCE: COLLAPSED -->

## 概要

東・北東・北・北西・西・南西・南・南東のうち、許可された方向だけを何度でも使って原点から目的地へ進む最小手数を求めます。
座標の絶対値が大きくてもグリッドを探索しません。

## 使える条件

- 一回の移動は8近傍のいずれかです。
- 方向ごとの費用はすべて1です。
- 障害物と盤面境界はありません。
- 許可された方向は何回でも使えます。

移動量、費用、障害物が異なる場合はこのrecipeではなく、`geometry/latticemove.hpp`やグラフ最短路を使います。

## 厳密な定義

方向番号$0,1,\ldots,7$は順に

$$
(1,0),(1,1),(0,1),(-1,1),(-1,0),(-1,-1),(0,-1),(1,-1)
$$

を表します。許可されたベクトルを一回加える操作を繰り返し、$(0,0)$から$(x,y)$へ到達する操作回数の最小値を返します。
到達不能なら`std::nullopt`です。

## Include

```cpp
#include "recipe/compassmove.hpp"
```

## 最短の使用例

```cpp
compassmoves moves("10101010"); // 東・北・西・南だけ許可
long long answer = moves.distance(x, y).value_or(-1);
```

## API

### コンストラクタ

```cpp
explicit compassmoves(std::string_view allowed);
explicit compassmoves(unsigned int allowed_mask);
```

- 文字列版は長さ8で、文字`'1'`の方向を許可し、`'0'`の方向を禁止します。
- mask版はbit $i$が1なら0-indexed方向$i$を許可します。bit 8以上は0である必要があります。
- 構築は文字列版が$O(8)$、mask版が$O(1)$です。

### `allowed`

```cpp
bool allowed(int index) const;
```

0-indexed方向`index`が許可されているかを$O(1)$で返します。

### `distance`

```cpp
std::optional<long long> distance(long long x, long long y) const;
std::optional<long long> distance(const point<long long>& target) const;
```

原点から目的地への最小手数を返します。到達不能なら`std::nullopt`です。
目的地が原点なら、許可方向がなくても0を返します。方向数は8で固定なので$O(1)$時間、$O(1)$追加領域です。

## 注意点

- 文字列の方向順は時計回りで、最初が東です。問題文の方向順を必ず確認してください。
- 斜め移動も一回として数えます。Euclid距離や辺の重み付き最短路ではありません。
- `recipe`は`all.hpp`へ含めないため、個別にincludeしてください。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `compassmoves`

```cpp
struct compassmoves
```

compassmoves(s).distance(x,y): sで許可した東・北東・北・北西・西・南西・南・南東移動の最短手数。

### `compassmoves`

```cpp
explicit compassmoves(std::string_view allowed)
```

O(8)。'1'を許可、'0'を禁止とする長さ8の文字列から作る。

### `compassmoves`

```cpp
explicit compassmoves(unsigned int allowed_mask) : mask(allowed_mask)
```

O(1)。bit iが方向iを許可するmaskから作る。

### `allowed`

```cpp
bool allowed(int index) const
```

O(1)。0-indexed方向indexが許可されているか返す。

### `distance`

```cpp
std::optional<long long> distance(long long x, long long y) const
```

O(1)。原点から(x,y)への最小手数を返し、到達不能ならnullopt。

### `distance`

```cpp
std::optional<long long> distance(const point<long long>& target) const
```

O(1)。原点からtargetへの最小手数を返し、到達不能ならnullopt。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC271 Ex - General General](https://atcoder.jp/contests/abc271/tasks/abc271_h)は、入力文字列の方向順がこの型と一致します。各test caseは`compassmoves(s).distance(A, B).value_or(-1)`で処理できます。

## 検証

256通りすべての許可maskと原点近傍の全目的地について、有限グリッド上のBFSと比較しています。また、公式サンプルをverifyコードで確認しています。
