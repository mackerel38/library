---
title: "Laminar区間集合"
documentation_of: //cp/structure/laminarintervalset.hpp
---

# Laminar区間集合

- Header: `cp/structure/laminarintervalset.hpp`
- Symbol: `poe::laminarintervalset`
- Status: experimental

## どんな問題に使えるか

端点がすべて異なる閉区間をonlineで追加し、既存区間と真に交差する区間だけを拒否する。
受理済み区間どうしは、互いに素・包含・被包含のいずれかとなるlaminar familyを保つ。

```cpp
#include "structure/laminarintervalset.hpp"

laminarintervalset intervals(coordinate_count);
if (intervals.add(left, right)) {
    // 追加された
}
```

ここで二区間が交差するとは、一方だけに属する部分が双方とも存在し、どちらも他方を包含しないことをいう。
公開APIでは`0 <= left < right < coordinate_count`を満たし、全addを通じて端点が重複しないことが前提。

## 仕組みと計算量

各座標について、その点を含む受理済み区間のうち最短のものを管理する。
新しい区間`[left,right]`が交差しないためには、`left`を含む最短区間が`right`も含み、
かつ`right`を含む最短区間が`left`も含めばよい。

区間追加は「区間長によるrange chmin」、端点判定はpoint getとなるため、
dual Segment Treeで各`add`を`O(log n)`、構築を`O(n)`、領域を`O(n)`で処理する。
`size()`と`intervals()`は`O(1)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `laminarintervalset`

```cpp
struct laminarintervalset
```

端点重複なしのlaminar閉区間族: laminarintervalset data(coordinate_count)。

### `laminarintervalset`

```cpp
explicit laminarintervalset(int coordinate_count) : coordinate_count_(coordinate_count), endpoint_used_(coordinate_count)
```

O(n)。座標[0,coordinate_count)上の空の区間族を作る。

### `add`

```cpp
bool add(int left, int right)
```

O(log n)。閉区間[left,right]が既存区間と交差しなければ追加してtrue、交差すればfalse。

### `size`

```cpp
int size() const noexcept
```

O(1)。受理済み区間数を返す。

### `intervals`

```cpp
const std::vector<std::pair<int, int>>& intervals() const noexcept
```

O(1)。追加順の受理済み閉区間列を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC424 F - Adding Chords](https://atcoder.jp/contests/abc424/tasks/abc424_f)では、
円を点1と点Nの間で切ると、弦の交差判定が区間の交差判定になる。
`verify/atcoder_abc424_f.cpp`で公式sample 2件一致、judge未提出。

property testでは端点を一度だけ使う乱択追加列を生成し、全受理済み区間との直接交差判定と照合する。
