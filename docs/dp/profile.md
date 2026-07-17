---
title: "Frontierの連結状態"
documentation_of: //cp/dp/profile.hpp
---

# Frontierの連結状態

- Header: `cp/dp/profile.hpp`
- Symbol: `poe::submaskprofileresult`, `poe::minimum_submask_profile`,
  `poe::connectivityprofile`, `poe::normalize_profile`
- Status: experimental

## どんな問題に使えるか

幅の小さい盤面を順に走査し、処理済み領域と未処理領域の境界に接する連結成分だけを状態にする問題に
使う。`connectivityprofile`は空位置を`-1`、追跡したい一つの成分を`0`、その他の成分を正の番号で
表す。成分番号は毎回正規化されるため、同じ分割がラベルの付け方だけで別状態になることを防げる。

この型が保持するのは「境界上の分割」と一つの注目成分だけである。閉じた成分の個数、次数、重み、
複数端点の対応などが必要なら、追加情報を持つ独自状態に組み込む。

## 使い方

```cpp
#include "dp/profile.hpp"

auto state = poe::connectivityprofile::from_mask(height, first_mask, 0);
state = state.advance(next_mask);
if (state.has_marked() && state.is_marked(height - 1)) {
    // 注目成分が右端の目的位置まで残っている
}
```

`from_mask`と`advance`は同じ列の隣接占有マスを縦に結び、前後列の同位置を横に結ぶ。
局所的な辺の張り方が異なる問題では、`add`、`erase`、`connect`、`mark`を使う。
生のラベル列だけ正規化したい場合は`normalize_profile`を使える。

## 操作と計算量

| 操作 | 意味 | 計算量 |
| --- | --- | --- |
| `from_mask`, `advance` | 占有maskから標準の上下左右接続を作る | `O(H^2)` |
| `add`, `erase`, `connect`, `mark` | 一位置または二成分を更新する | `O(H)` |
| `normalize` | ラベルを標準形へ直す | `O(H log H)` |
| `operator[]`, `occupied`, `is_marked` | 一位置を調べる | `O(1)` |
| `has_marked` | 注目成分が境界に残るか調べる | `O(H)` |

幅は63未満である。`advance(mask)`は占有マス間の辺を必ず張るため、辺を選ぶ問題には直接使えない。
また注目成分が一度frontierから消えた後に復活することはないので、通常はその状態を直ちに捨てる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `submaskprofileresult`

```cpp
struct submaskprofileresult
```

行submask profile DPの結果。costが削除bit数、masksが選んだ各行mask。

### `minimum_submask_profile`

```cpp
template <class Compatible> submaskprofileresult minimum_submask_profile( const std::vector<std::uint64_t>& available, int width, Compatible compatible )
```

O(h 4^w)時間・O(h 2^w)空間。各行のsubmaskを隣接compatibleに選び削除bit数を最小化する。

### `normalize_profile`

```cpp
inline std::vector<int> normalize_profile( std::vector<int> labels, int distinguished = -1, bool reserve_zero = false )
```

O(width)。normalize_profile(labels): frontier DPの連結成分番号を出現順に0,1,...へ正規化する。
-1は空マス。distinguished>=0ならその位置の成分を必ず0にする。
reserve_zero=trueならdistinguishedの成分が消えた後も0を他成分へ再利用しない。

### `connectivityprofile`

```cpp
struct connectivityprofile
```

connectivityprofile state(width): frontier上の連結成分を持つ。-1は空、0は注目成分。

### `connectivityprofile`

```cpp
explicit connectivityprofile(int width = 0) : labels(width, -1)
```

O(width)。connectivityprofile(width): 幅widthの空状態を作る。

### `connectivityprofile`

```cpp
explicit connectivityprofile(std::vector<int> labels_) : labels(std::move(labels_))
```

O(width)。connectivityprofile(labels): 0を注目成分として正規化して持つ。

### `from_mask`

```cpp
static connectivityprofile from_mask(int width, std::uint64_t mask, int marked = -1)
```

O(width^2)。from_mask(width, mask, marked): 1のマスを上下に連結し、markedの成分を0にする。

### `advance`

```cpp
connectivityprofile advance(std::uint64_t mask) const
```

O(width^2)。state.advance(mask): 次の列をmaskとして、横辺と縦辺をすべて張った状態を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。state.size(): frontierの幅を返す。

### `occupied`

```cpp
bool occupied(int position) const
```

O(1)。state.occupied(position): positionが空でなければtrueを返す。

### `operator[]`

```cpp
int operator[](int position) const
```

O(1)。state[position]: positionの成分番号を返す。空なら-1、注目成分なら0。

### `add`

```cpp
void add(int position)
```

O(width)。state.add(position): 空のpositionへ新しい独立成分を置く。

### `erase`

```cpp
void erase(int position)
```

O(width)。state.erase(position): positionを空にして状態を正規化する。

### `connect`

```cpp
void connect(int a, int b)
```

O(width)。state.connect(a, b): 占有済みのaとbの成分を併合する。

### `mark`

```cpp
void mark(int position)
```

O(width)。state.mark(position): positionを含む成分を注目成分0にする。

### `is_marked`

```cpp
bool is_marked(int position) const
```

O(1)。state.is_marked(position): positionが注目成分ならtrueを返す。

### `has_marked`

```cpp
bool has_marked() const
```

O(width)。state.has_marked(): 注目成分がfrontier上に残っていればtrueを返す。

### `normalize`

```cpp
void normalize()
```

O(width log width)。state.normalize(): 同じ連結状態を同じ成分番号へ正規化する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[TDPC S - マス目](https://atcoder.jp/contests/tdpc/tasks/tdpc_grid)のconnection profile DPで使える。
この問題の列挙まで含む短い完成形は`recipe/connectivityfrontier.hpp`に置いている。
`verify/atcoder_tdpc_grid.cpp`で4x4以下の全盤面との比較を行い、2026-07-15にjudge AC確認済み。

## 行submask選択

`minimum_submask_profile(available, width, compatible)`は、各行で`available[row]`のsubmaskを一つ選び、
隣接行maskが`compatible(previous,current)`を満たす条件で、消したbit数の総和を最小化する。

```cpp
auto result = minimum_submask_profile(initial_masks, width,
    [&](uint64_t upper, uint64_t lower) {
        return valid_adjacent_rows(upper, lower);
    });
```

`result.cost`が最小削除数、`result.masks`が復元した各行mask。
計算量は高さ`h`、幅`w`として`O(h 4^w)`時間・`O(h 2^w)`領域。
横方向だけの制約は各行maskを列挙する時点で`available`やpredicate側へ反映する。

[AtCoder ABC424 D - 2x2 Erasing 2](https://atcoder.jp/contests/abc424/tasks/abc424_d)では、
上下二行の共通1bitに隣接する二bitがなければcompatibleとする。
`verify/atcoder_abc424_d.cpp`で公式sample一致、judge未提出。
