---
title: "動的中央値"
documentation_of: //cp/structure/median.hpp
---

# 動的中央値


## 概要

値を逐次追加・削除しながら中央値を求める。中央値へ全要素をそろえる費用、
sliding windowの中央値、オンラインな絶対値和最小化にも使える。

## 厳密な定義

- `median`: 動的中央値: median<long long> values; add/eraseはO(log n)、lower/upper/deviationはO(1)。

## Include

```cpp
#include "structure/median.hpp"

poe::median<long long> values;
values.add(x);
cout << values.lower() << '\n';
```

- `add(value)`, `erase(value)`: `O(log n)`。`erase`は存在する値を一つ削除する。
- `lower()`: 偶数個なら小さい側、`upper()`は大きい側の中央値。`O(1)`。
- `deviation()`: `lower()`から全要素への絶対差の和。`O(1)`。
- `size()`, `empty()`: `O(1)`。

`lower`, `upper`は空集合では呼べない。`deviation`は空集合なら0を返す。
和と積を使うため、`T`には値だけでなく偏差和も収まる型を選ぶ。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `median`

```cpp
template <class T> struct median
```

動的中央値: median<long long> values; add/eraseはO(log n)、lower/upper/deviationはO(1)。

### `median`

```cpp
median() = default
```

O(1)。空の多重集合を作る。

### `add`

```cpp
void add(const T& value)
```

O(log n)。valueを一つ追加する。

### `erase`

```cpp
void erase(const T& value)
```

O(log n)。valueを一つ削除する。valueが存在すること。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。空かを返す。

### `lower`

```cpp
const T& lower() const
```

O(1)。小さい側の中央値を返す。空でないこと。

### `upper`

```cpp
const T& upper() const
```

O(1)。大きい側の中央値を返す。空でないこと。要素数が奇数ならlower()と等しい。

### `deviation`

```cpp
T deviation() const
```

O(1)。lower()から各要素までの絶対差の総和を返す。空なら0。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC458 D - Chalkboard Median](https://atcoder.jp/contests/abc458/tasks/abc458_d)は、
初期値と各queryの2値を`add`し、毎回`lower()`を出力する。
`verify/atcoder_abc458_d.cpp`で公式sample一致を確認している。
