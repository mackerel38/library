---
title: "SWAG"
documentation_of: //cp/structure/swag.hpp
---

# SWAG


## 概要

queueの先頭削除・末尾追加を行いながら、queue全体を順番どおりモノイド演算で畳み込む。
全操作はならし`O(1)`で、sliding windowの最小値・最大値・GCD・行列積・写像合成などに使える。

## 厳密な定義

- `swag`: queue全体のモノイド積: swag<S, op, e> queue; 各操作ならしO(1)。

```cpp
swag<int, minimum, inf> window;
window.push(value);
window.pop();
int answer = window.prod();
```

`push`、`pop`、`front`、`back`、`prod`、`size`、`empty`を持つ。
空の`prod()`は`e()`を返す。`pop()`と`front()`は空queueでは使えない。
`op`は結合的で、`e()`は単位元であること。可換性は不要である。

## Include

```cpp
#include "structure/swag.hpp"
```

## 計算量

各操作ならし`O(1)`、全要素を通した合計時間`O(q)`、領域`O(n)`。
一度の`pop`または`front`が要素移動に`O(n)`かかる場合はある。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `swag`

```cpp
template <class S, auto op, auto e> struct swag
```

queue全体のモノイド積: swag<S, op, e> queue; 各操作ならしO(1)。

### `swag`

```cpp
swag() = default
```

O(1)。空のqueueを構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。空ならtrueを返す。

### `push`

```cpp
void push(const S& value)
```

O(1)。末尾へvalueを追加する。

### `pop`

```cpp
void pop()
```

ならしO(1)。先頭要素を削除する。空でないこと。

### `front`

```cpp
const S& front()
```

ならしO(1)。先頭要素を返す。空でないこと。

### `back`

```cpp
const S& back() const
```

O(1)。末尾要素を返す。空でないこと。

### `prod`

```cpp
S prod() const
```

O(1)。先頭から順にopで畳み込んだ値を返す。空ならe()。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Queue Operate All Composite](https://judge.yosupo.jp/problem/queue_operate_all_composite)で、
非可換なアフィン写像合成を扱うverifyコードを収録した。

[AtCoder ABC456 F - Plan Holidays](https://atcoder.jp/contests/abc456/tasks/abc456_f)では、
2状態min-plus DPの遷移行列を区間ごとに合成する。長さ`K`の窓を一日ずつ滑らせながら
`prod()`を取ることで、各開始位置からDPをやり直す`O(NK)`を全体`O(N)`へ落とせる。
`verify/atcoder_abc456_f.cpp`に使用例を収録した。

## 検証

- `tests/api/structure/swag.cpp`: 公開APIと非可換演算
- `tests/property/structure/swag.cpp`: `std::deque`の愚直連結との固定seed比較
- `verify/librarychecker_queue_operate_all_composite.cpp`: 公開問題用コード
- `verify/atcoder_abc456_f.cpp`: min-plus遷移合成によるsliding window DP
