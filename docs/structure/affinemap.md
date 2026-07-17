---
title: "Affine変換付きmap"
documentation_of: //cp/structure/affinemap.hpp
---

# Affine変換付きmap

- Header: `cp/structure/affinemap.hpp`
- Symbol: `poe::affinemap`
- Status: experimental

## どんな問題に使えるか

キーごとに値を持ち、全キーの値へ同じ`x -> a*x+b`を何度も適用しながら、個別キーの追加・削除と
総和取得も行う問題に使う。全要素更新を二つの遅延係数へまとめるため、要素数に依存せず更新できる。

```cpp
#include "structure/affinemap.hpp"

poe::affinemap<int, mint> values;
values.set(3, 10);
values.set(8, 20);
values.apply(-1, 100); // 全valueを100-xへ変換
mint total = values.sum();
values.erase(3);
```

個別代入では現在のaffine変換を逆算するため、累積scaleが0でなく、`Value`で除算できる必要がある。

## 計算量

`set`、`erase`、`contains`、`get`は`O(log n)`、`apply`、`sum`、`size`は`O(1)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `affinemap`

```cpp
template <class Key, class Value> struct affinemap
```

全valueへのaffine変換を遅延できるmap: affinemap<int,mint> values; apply(a,b)でxをa*x+bへ変える。

### `affinemap`

```cpp
affinemap() = default
```

O(1)。空のmapを作る。

### `set`

```cpp
void set(const Key& key, const Value& value)
```

O(log n)。keyの論理valueを代入する。現在のscaleが0でないことが必要。

### `erase`

```cpp
bool erase(const Key& key)
```

O(log n)。keyを削除し、存在したか返す。

### `contains`

```cpp
bool contains(const Key& key) const
```

O(log n)。keyが存在するか返す。

### `get`

```cpp
Value get(const Key& key) const
```

O(log n)。keyの論理valueを返す。keyが存在することが必要。

### `apply`

```cpp
void apply(const Value& scale, const Value& shift)
```

O(1)。全valueをx -> scale*x+shiftへ一括変換する。

### `sum`

```cpp
Value sum() const
```

O(1)。全valueの総和を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。key数を返す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。空ならtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC435 G - Domino Arrangement](https://atcoder.jp/contests/abc435/tasks/abc435_g)では、
各色に対応するDP値を、偶奇別の`affinemap`で管理する。二つ前の状態から全値を
`x -> S-x`へ変換し、使用可能区間へ出入りする色だけを追加・削除することで全体`O((N+M)log M)`となる。
`verify/atcoder_abc435_g.cpp`で公式sampleを確認済み。
