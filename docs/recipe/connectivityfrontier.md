---
title: "盤面連結frontier DP recipe"
documentation_of: //cp/recipe/connectivityfrontier.hpp
---

# 盤面連結frontier DP recipe

- Header: `cp/recipe/connectivityfrontier.hpp`
- Symbol: `poe::connectivityfrontierdp`
- Status: experimental recipe

## recipeへ置く理由

`connectivityfrontierdp`は疎な状態管理だけでなく、各列のbitmask全列挙、選択マス間の上下左右辺、
注目成分がfrontierから消えた状態の破棄まで決めている。提出コードは非常に短くなる一方、辺の選択や
禁止マスが異なる問題にはそのまま使えない。このため汎用DPの`dp`ではなく、狭い問題族向けの完成形を
置く`recipe`へ隔離し、`all.hpp`からはincludeしない。

## 適用できる問題

- 幅`H`の盤面を列ごとに左から処理する。
- 選んだマス同士は上下左右に隣接すれば必ず接続する。
- 最初の列の`marked`を含む連結成分が、最後の指定位置まで届く配置数を数える。
- 各マスは自由に選ぶか選ばないかを決められる。

禁止マス、選択個数、辺そのものの選択、複数の注目成分を扱う場合は、`frontierdp`と
`connectivityprofile`を直接組み合わせて遷移を書く。

## 使い方

```cpp
#include "recipe/connectivityfrontier.hpp"

using mint = poe::modint1000000007;
poe::connectivityfrontierdp<mint> dp(height, 0);
for (int column = 1; column < width; ++column) dp.advance_all();
std::cout << dp.sum_marked(height - 1).val() << '\n';
```

状態数を`S`として、初期化は`O(2^H H^2 log S)`、一列の更新は
`O(S 2^H H^2 log S)`時間を目安とする。幅を指数部に持つため、縦横を交換できる問題では小さい方を
高さにする。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `connectivityfrontierdp`

```cpp
template<class Value> struct connectivityfrontierdp
```

connectivityfrontierdp<Value> dp(height, marked): markedを含む全初列から盤面連結DPを始める。
各列では選択マス間の上下辺と前列への横辺をすべて張る問題だけに使う。

### `connectivityfrontierdp`

```cpp
explicit connectivityfrontierdp(int height, int marked, Value value = Value
```

O(2^height * height^2 log states)。markedを必ず選ぶ全初列を列挙する。

### `advance_all`

```cpp
void advance_all()
```

状態数をSとしてO(S * 2^height * height^2 log S)。次の列を全列挙して進める。

### `sum_marked`

```cpp
Value sum_marked(int position) const
```

O(states)。dp.sum_marked(position): positionが注目成分に属する状態の総和を返す。

### `minimum_connected_grid_completion`

```cpp
inline int minimum_connected_grid_completion( std::vector<std::uint64_t> required, int width )
```

O(rows*S*2^width*width^2 log S)。必須マスを含む最小追加マス数の連結supersetを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[TDPC S - マス目](https://atcoder.jp/contests/tdpc/tasks/tdpc_grid)へ上記の形で直接使える。
`verify/atcoder_tdpc_grid.cpp`で公式サンプルと4x4以下の全盤面との比較を確認し、
2026-07-15にjudge AC確認済み。

## 必須マスを連結にする最小追加

```cpp
int answer = minimum_connected_grid_completion(required_masks, width);
```

`required_masks[row]`の1bitを必ず選び、上下左右で選択マス全体が連結になるように追加する
最小マス数を返す。先頭・末尾の必須マスがない行は自動で除く。必須マスは一つ以上必要。

各行では必須maskのsupersetを列挙し、frontier上の既存成分が一つも消えない遷移だけを許す。
最適解で途中消滅する成分は最終連結成分へ寄与せず、削除した方が安いため、この枝刈りで最適値を保つ。
行数を`R`、幅を`W`、状態数を`S`として
`O(R S 2^W W^2 log S)`時間が目安。`W<63`だが、実用上は一桁程度を想定する。

[AtCoder ABC296 Ex - Unite](https://atcoder.jp/contests/abc296/tasks/abc296_h)で直接使える。
`verify/atcoder_abc296_h.cpp`で公式sample 3件を確認済み。
`tests/property/dp/connectgrid.cpp`では4x4以下の全superset連結判定と比較する。
