---
title: "数直線上の最寄り距離総和"
documentation_of: //cp/structure/nearestset.hpp
---

# 数直線上の最寄り距離総和

- Header: `cp/structure/nearestset.hpp`
- Symbol: `poe::nearestset<T, Sum>`
- Status: experimental

```cpp
#include "structure/nearestset.hpp"
```

相異なる点を順次追加・削除し、各点から最寄りの別点までの距離の総和を維持する。

```cpp
poe::nearestset<long long> points;
points.add(0);
points.add(x);
cout << points.sum() << '\n';
```

`add`・`erase`は集合が変化したときだけ`true`を返す。`distance(value)`はvalue自身を除く最寄り距離で、
valueが存在しないか集合が一要素なら`nullopt`。`sum()`は一要素以下なら`0`。

挿入・削除で寄与が変わりうる点は、その点と昇順で隣接する高々三点だけなので、
`add`・`erase`・`distance`は`O(log n)`、`sum`・`size`は`O(1)`。領域`O(n)`。
座標差と総和は`Sum`へ収まる必要があり、既定は`long long`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `nearestset`

```cpp
template<class T, class Sum = long long> struct nearestset
```

数直線上の点集合: nearestset<long long> points; add/erase O(log n)、sum O(1)。

### `nearestset`

```cpp
nearestset() = default
```

O(1)。空集合を作る。

### `add`

```cpp
bool add(const T& value)
```

O(log n)。valueを追加し、集合が変化したならtrueを返す。

### `erase`

```cpp
bool erase(const T& value)
```

O(log n)。valueを削除し、集合が変化したならtrueを返す。

### `distance`

```cpp
std::optional<Sum> distance(const T& value) const
```

O(log n)。valueが含まれるなら、別の最寄り点までの距離を返す。点が一つならnullopt。

### `sum`

```cpp
Sum sum() const noexcept
```

O(1)。各点から最寄りの別点までの距離の総和を返す。点が一つ以下なら0。

### `contains`

```cpp
bool contains(const T& value) const
```

O(log n)。valueが含まれるならtrueを返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。点数を返す。

### `values`

```cpp
const std::set<T>& values() const noexcept
```

O(1)。点を昇順に保持するsetを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC430 D - Neighbor Distance](https://atcoder.jp/contests/abc430/tasks/abc430_d)では、
最初に座標`0`を追加し、各入力座標の`add`後に`sum()`を出力する。
`verify/atcoder_abc430_d.cpp`で公式sampleを確認済み。
