---
title: "区間集合"
documentation_of: //cp/structure/intervalset.hpp
---

# 区間集合

- Header: `cp/structure/intervalset.hpp`
- Symbol: `poe::intervalset`
- Status: experimental

## どんな問題に使えるか

整数直線や実数直線上の被覆を、互いに交わらず隣接もしない半開区間へ圧縮して管理する。
区間の追加・削除、点の包含判定、被覆長の取得が必要な問題に使える。座標の最大値ではなく、
区間の境界数にだけ依存して動く。

```cpp
#include "structure/intervalset.hpp"

poe::intervalset<long long> covered;
covered.add(2, 8);
covered.erase(4, 6);
long long length = covered.covered(); // 4
bool has_three = covered.contains(3);
```

`add(left,right)`と`erase(left,right)`は、実際に増減した長さを返す。区間は常に`[left,right)`である。

## 計算量

交差または吸収する区間数を`k`として、追加・削除は`O((k+1)log n)`、包含判定は`O(log n)`、
被覆長は`O(1)`。削除だけを続ける場合、消滅する区間について償却できる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `intervalset`

```cpp
template <class T = long long> struct intervalset
```

互いに素な半開区間の和集合: intervalset<long long> covered; add/eraseで被覆長も管理する。

### `intervalset`

```cpp
intervalset() = default
```

O(1)。空の区間集合を作る。

### `add`

```cpp
T add(T left, T right)
```

O((k+1)log n)。[left,right)を加え、新しく加わった長さを返す。kは吸収する区間数。

### `erase`

```cpp
T erase(T left, T right)
```

O((k+1)log n)。[left,right)を除き、実際に除かれた長さを返す。kは交差する区間数。

### `contains`

```cpp
bool contains(T value) const
```

O(log n)。valueが和集合に含まれるか返す。

### `covered`

```cpp
T covered() const noexcept
```

O(1)。和集合の長さを返す。

### `intervals`

```cpp
const std::map<T, T>& intervals() const noexcept
```

O(1)。互いに交わらず隣接もしない半開区間のmapを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC435 E - Cover query](https://atcoder.jp/contests/abc435/tasks/abc435_e)では、
最初に白い区間`[0,N)`を追加し、各queryで黒くする区間を`erase`すれば、残る白マス数を
`covered()`で得られる。`verify/atcoder_abc435_e.cpp`で公式sampleを確認済み。
