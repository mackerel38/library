---
title: "永続動的 Li Chao Tree"
documentation_of: //cp/structure/persistentlichaotree.hpp
---

# 永続動的 Li Chao Tree


## 概要

直線集合の版を残しながら、任意の実数座標で最小値または最大値を求める。
prefixごとの直線集合、分岐するDP、永続版を左右から作って一要素を除外する問題に使える。
問い合わせ座標の事前列挙は不要で、versionは軽い整数handleで表す。

## 厳密な定義

- `lichaomode`: Li Chao Treeが最小値・最大値のどちらを返すかを表す。
- `persistentlichaotree`: 永続動的Li Chao Tree: versionはint handle。直線追加・取得O(depth)、copy O(1)。

## Include

```cpp
#include "structure/persistentlichaotree.hpp"

persistentlichaotree<double, lichaomode::maximum> lines(0.0, 1.0, 60);
int root0 = lines.initial();
int root1 = lines.add_line(root0, {slope, intercept});
double best = lines.get(root1, x);
```

直線追加・queryは`O(depth)`、version copyはhandle代入で`O(1)`、追加領域は一追加
`O(depth)`。連続領域を有限深さで分割するため、浮動小数点版は近似である。
幅`W`、傾き差上限`A`なら打切り由来の値誤差は概ね`WA/2^depth`以下になる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lichaomode`

```cpp
enum class lichaomode
```

Li Chao Treeが最小値・最大値のどちらを返すかを表す。

### `persistentlichaotree`

```cpp
template <class T, lichaomode Mode = lichaomode::minimum> struct persistentlichaotree
```

永続動的Li Chao Tree: versionはint handle。直線追加・取得O(depth)、copy O(1)。

### `persistentlichaotree`

```cpp
persistentlichaotree(T left, T right, int depth = 60) : left_(left), right_(right), depth_(depth)
```

O(1)。query範囲[left,right]と最大深さを指定して空treeを作る。

### `initial`

```cpp
version initial() const noexcept
```

O(1)。直線を持たない初期versionを返す。

### `add_line`

```cpp
version add_line(version base, line<T> value)
```

O(depth)。baseへvalueを追加した新versionを返し、baseは変更しない。

### `get`

```cpp
T get(version root, T x) const
```

O(depth)。指定versionに含まれる直線のxでの最小値または最大値を返す。

### `empty`

```cpp
bool empty(version root) const noexcept
```

O(1)。versionが空ならtrueを返す。

### `node_count`

```cpp
int node_count() const noexcept
```

O(1)。確保済みnode数を返す。

### `reserve_nodes`

```cpp
void reserve_nodes(std::size_t count)
```

O(count)。node再確保を減らすため領域を予約する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC448 G - Conquest](https://atcoder.jp/contests/abc448/tasks/abc448_g)の公式解説にある別解では、
prefix・suffixの永続動的Li Chao Treeにより、一本だけ除いた上包絡線を評価する。
問題全体にはさらに零和ゲームの小次元LPが必要なため、本ヘッダ単独。
API testと各versionの全直線走査との固定seed比較を行う。
