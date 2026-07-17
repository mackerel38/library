---
title: "Grid BFS・連結成分"
documentation_of: //cp/graph/grid.hpp
---

# Grid BFS・連結成分

- Header: `cp/graph/grid.hpp`
- Symbol: `poe::gridpoint`, `poe::gridcomponentsresult`, `poe::layeredgridcomponentsresult`,
  `poe::grid4`, `poe::grid8`, `poe::inside_grid`, `poe::gridbfs`, `poe::grid_components`,
  `poe::layered_grid_components`, `poe::sparse_grid_path_exists`
- Status: experimental

## どんな問題に使えるか

長方形grid上で、一つまたは複数の始点から各マスへの最短移動回数を求める。
壁判定と近傍方向を差し替えられる。

```cpp
#include "graph/grid.hpp"

vector<gridpoint> sources{{0, 0}, {h - 1, w - 1}};
auto distance = gridbfs(h, w, sources, [&](int row, int column) {
    return grid[row][column] != '#';
});
auto diagonal = gridbfs(h, w, sources, passable, grid8);
```

`grid4`は上下左右、`grid8`は斜めを含む8近傍である。独自の`std::array<gridpoint,K>`も渡せる。
始点が重複しても一度だけqueueへ入り、通行不能な始点は無視する。通行不能または到達不能なマスは`-1`。

`grid_components`は通行可能マスの成分番号、各成分サイズ、外周に接するかを返す。

```cpp
auto components = grid_components(h, w, [&](int r, int c) { return grid[r][c] == '.'; });
int enclosed = std::ranges::count(components.touches_border, false);
```

巨大な盤面で障害物だけが疎な場合、左上から右下への到達性は
`sparse_grid_path_exists(height,width,blocked)`で判定できる。盤面全体は確保しない。
障害物を8近傍で連結し、「左辺または下辺」と「上辺または右辺」を結ぶ障害物成分が存在する場合だけ
4近傍pathが遮断される、という平面双対を使う。始点と終点は障害物に含めない。

複数階・複数時刻など同じ大きさのgridがlayer状に並び、layer内は4近傍、
隣接layer間は同じ座標で接続するときは`layered_grid_components`を使う。
各layerの成分情報に加え、成分単位で重複を除いた`up`と`down`の隣接先を返す。

```cpp
auto compressed = layered_grid_components(f, h, w,
    [&](int k, int r, int c) { return grid[k][r][c] != '#'; });
```

## 計算量

通常のgridはマス数`hw`、方向数`K`として`O(hwK)`時間、`O(hw)`領域。
layer数を`f`とする`layered_grid_components`は`O(fhw log(hw))`時間、`O(fhw)`領域。
`passable(row,column)`は探索中に複数回呼ばれるため、状態を変更しないこと。
`sparse_grid_path_exists`は障害物数を`k`として期待`O(k alpha(k))`時間、`O(k)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `gridpoint`

```cpp
struct gridpoint
```

grid上の(row,column)座標。

### `gridcomponentsresult`

```cpp
struct gridcomponentsresult
```

gridcomponentsresultは各マスの成分番号・成分サイズ・外周接触を持つ。

### `count`

```cpp
int count() const noexcept
```

O(1)。成分数を返す。

### `layeredgridcomponentsresult`

```cpp
struct layeredgridcomponentsresult
```

各layer内のgrid成分と、同じ座標で接する隣接layer成分を持つ。

### `count`

```cpp
int count() const noexcept
```

O(1)。layer数を返す。

### `grid4`

```cpp
inline constexpr std::array<gridpoint, 4> grid4
```

上下左右の4近傍。

### `grid8`

```cpp
inline constexpr std::array<gridpoint, 8> grid8
```

周囲の8近傍。

### `inside_grid`

```cpp
inline bool inside_grid(int row, int column, int height, int width) noexcept
```

O(1)。(row,column)がheight x widthのgrid内ならtrue。

### `sparse_grid_path_exists`

```cpp
inline bool sparse_grid_path_exists( int height, int width, const std::vector<gridpoint>& blocked)
```

O(k alpha(k))期待時間。(0,0)から(h-1,w-1)へ疎なblockedを避ける4近傍pathがあればtrue。

### `grid_components`

```cpp
template<class Passable, std::size_t K> gridcomponentsresult grid_components( int height, int width, Passable passable, const std::array<gridpoint, K>& directions )
```

O(hwK)。passableなマスのdirections連結成分と外周接触を列挙する。

### `grid_components`

```cpp
template<class Passable> gridcomponentsresult grid_components(int height, int width, Passable passable)
```

O(hw)。passableなマスの上下左右連結成分と外周接触を列挙する。

### `layered_grid_components`

```cpp
template <class Passable> layeredgridcomponentsresult layered_grid_components( int layer_count, int height, int width, Passable passable )
```

O(lhw log(hw))。各layerを4近傍成分へ圧縮し、同じ座標で接する上下成分を結ぶ。

### `gridbfs`

```cpp
template<class Passable, std::size_t K> std::vector<std::vector<int>> gridbfs( int height, int width, const std::vector<gridpoint>& sources, Passable passable, const std::array<gridpoint, K>& directions )
```

O(hwK)。複数始点からdirectionsへ進むgrid BFS距離。通れない・到達不能なマスは-1。

### `gridbfs`

```cpp
template<class Passable> std::vector<std::vector<int>> gridbfs( int height, int width, const std::vector<gridpoint>& sources, Passable passable )
```

O(hw)。複数始点から上下左右へ進むgrid BFS距離。通れない・到達不能なマスは-1。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC460 D - Repeatedly Repainting](https://atcoder.jp/contests/abc460/tasks/abc460_d):
  一回操作後の黒マスを全始点とし、8近傍距離の偶奇を使う。

`verify/atcoder_abc460_d.cpp`で公式sampleを確認済み。
API testと障害物付き4近傍・8近傍の距離を確認する。

- [AtCoder ABC450 C - Puddles](https://atcoder.jp/contests/abc450/tasks/abc450_c):
  `grid_components`の`touches_border`がfalseの成分を数える。

`verify/atcoder_abc450_c.cpp`に提出用コードを用意し、公式sampleを確認済み。

- [AtCoder ABC440 G - Haunted House](https://atcoder.jp/contests/abc440/tasks/abc440_g):
  各階の空きマスを成分へ圧縮し、上下階の接触関係上でDPする。

`verify/atcoder_abc440_g.cpp`に使用例を収録し、公式sample一致を確認している。

- [AtCoder ABC413 G - Big Banned Grid](https://atcoder.jp/contests/abc413/tasks/abc413_g):
  最大`2*10^5`四方の盤面を作らず、疎な障害物だけで左上から右下への到達性を判定する。

`verify/atcoder_abc413_g.cpp`で公式sampleを確認済み。
4行4列以下の全障害物配置について通常のBFSと比較している。
