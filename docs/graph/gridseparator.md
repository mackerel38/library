---
title: "グリッドの最小頂点separatorと最適解数"
documentation_of: //cp/graph/gridseparator.hpp
---

# グリッドの最小頂点separatorと最適解数

## 概要

すべて通行可能な長方形グリッドで、一部のマスだけを壁にできます。
スタートとゴールを行き来できなくする最小壁数と、その最小壁集合が何通りあるかを求めます。

## 厳密な定義

$h\times w$ の各マスを頂点とし、辺を共有する二マスを結んだ4近傍グラフを考えます。
`removable[r][c] == true` のマスだけを削除できます。指定された二頂点 $s,t$ を異なる
連結成分へ分ける削除可能頂点集合 $D$ のうち、$|D|$ の最小値と、それを達成する異なる
集合 $D$ の個数を返します。個数は `mod` で割った余りです。
`mod` は奇数でなければなりません。

削除不能なマスも通行可能です。最初から通れないマスを表す入力ではありません。
削除可能頂点だけでは分離できない場合、`possible()` は `false` になります。

平面上でseparatorが作る8近傍cycleを、$s$ から $t$ への基準路と奇数回交差するcycleへ
言い換えます。基準路上の削除可能マスを始点として、交差偶奇を加えた二層グラフで
最短路数を数えます。

時間計算量は

$$
O\bigl((h+w)(hw+(h+w)^2)\bigr)
$$

で、正方形グリッドでは一辺を $n$ として $O(n^3)$ です。領域計算量は
$O(hw+(h+w)^2)$ です。

## Include

```cpp
#include "graph/gridseparator.hpp"
```

## 使い方

```cpp
vector removable(height, vector<bool>(width));
// 壁にしてよいマスだけtrueにする。
auto result = minimum_grid_vertex_separator(removable, start, goal);
if (result.possible()) {
    cout << result.size << ' ' << result.count.val() << '\n';
}
```

### `gridseparatorresult<mod>`

- `size`: 最小削除頂点数。分離不能なら `-1`。
- `count`: 最小削除頂点集合の個数を `mod` で割った余り。
- `possible()`: 分離可能なら `true`。

### `minimum_grid_vertex_separator`

```cpp
template<int mod = 998244353>
gridseparatorresult<mod> minimum_grid_vertex_separator(
    const vector<vector<bool>>& removable,
    pair<int, int> start,
    pair<int, int> goal);
```

座標は0-indexedです。`start != goal` で、始点と終点自身は削除不能、すなわち
`removable[start] == removable[goal] == false` でなければなりません。
`mod` は奇数で、既定値は `998244353` です。

## 頻出事故

- `false` は通行不能ではなく、壁に変更できない通行可能マスです。
- 移動は4近傍です。内部で使う8近傍cycleは平面双対による計算方法です。
- 同じ枚数の壁を置く順序は区別せず、壁にするマスの集合だけを区別します。
- 一般グラフの最小頂点cut数え上げへは使えません。長方形グリッドの平面性を使います。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `gridseparatorresult`

```cpp
template<int mod> struct gridseparatorresult
```

最小頂点separatorの大きさと、その最適集合数を表す。

### `possible`

```cpp
bool possible() const noexcept
```

O(1)。指定可能マスだけで分離できるならtrueを返す。

### `minimum_grid_vertex_separator`

```cpp
template<int mod = 998244353> gridseparatorresult<mod> minimum_grid_vertex_separator( const std::vector<std::vector<bool>>& removable, std::pair<int, int> start, std::pair<int, int> goal )
```

O((h+w)(hw+(h+w)^2))時間・O(hw+(h+w)^2)領域。4近傍格子の最小頂点separatorと個数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC243 Ex - Builder Takahashi (Enhanced version)](https://atcoder.jp/contests/abc243/tasks/abc243_h)では、
`.` を `true`、`S`、`G`、`O` を `false` として渡します。`O` も通行可能ですが、
壁にはできないという意味なので、このAPIの `false` と一致します。

```cpp
const auto result = minimum_grid_vertex_separator(removable, start, goal);
if (!result.possible()) cout << "No\n";
else cout << "Yes\n" << result.size << ' ' << result.count.val() << '\n';
```
