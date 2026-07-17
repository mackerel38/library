---
title: "Push・pop列Segment Tree"
documentation_of: //cp/string/stacksequence.hpp
---

# Push・pop列Segment Tree

- Header: `cp/string/stacksequence.hpp`
- Symbol: `poe::stacksequence`, `poe::stacksequenceresult`
- Status: experimental

## どんな問題に使えるか

列への`push(symbol)`と、空なら何もしない`pop()`からなる固定長の操作列に対して、一点更新と
部分区間の実行結果を扱う。操作を単純な定数サイズmonoidへ潰すと、右側のpopで消える左側末尾の
情報が失われる。本構造はSegment Tree内部の分割履歴を辿って必要な末尾だけ復元する。

```cpp
#include "string/stacksequence.hpp"

// -1はpop、0以上はその数字をpushする。
poe::stacksequence<998244353, 10> sequence(operations);
sequence.set(index, -1);
auto result = sequence.prod(left, right);
if (!result.empty()) std::cout << result.value << '\n';
```

`result.pop`は空列へ作用させても相殺できずに残る先頭pop数、`result.length`と`result.value`は
残った列の長さとbase進数値である。`base`は`modulus`上で逆元を持つ必要がある。

## 計算量

構築`O(n)`、一点更新・区間取得`O(log^2 n)`、全体取得`O(1)`。領域`O(n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `stacksequenceresult`

```cpp
template <int modulus> struct stacksequenceresult
```

push/pop列を実行した結果。popは空stackへ残ったpop数、lengthとvalueは残る列を表す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。実行後の列が空ならtrueを返す。

### `stacksequence`

```cpp
template <int modulus, int base> struct stacksequence
```

一点更新付きpush/pop列: -1をpop、[0,base)をpushとして区間実行結果を返す。

### `stacksequence`

```cpp
explicit stacksequence(const std::vector<int>& operations) : n_(static_cast<int>(operations.size())), power_(n_ + 1), inverse_power_(n_ + 1)
```

O(n)。operationsから構築する。-1はpop、それ以外はpushするsymbolを表す。

### `size`

```cpp
int size() const noexcept
```

O(1)。操作数を返す。

### `set`

```cpp
void set(int index, int operation)
```

O(log^2 n)。index番目をoperationへ置き換える。

### `get`

```cpp
int get(int index) const
```

O(1)。index番目の操作を返す。popは-1で表す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(1)。operator[]でindex番目の操作を返す。

### `prod`

```cpp
result_type prod(int left, int right) const
```

O(log^2 n)。半開区間[left,right)を空stackから実行した正規化結果を返す。

### `all_prod`

```cpp
result_type all_prod() const
```

O(1)。全操作列の実行結果を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC434 G - Keyboard](https://atcoder.jp/contests/abc434/tasks/abc434_g)では、
数字をpush、`B`をpopとしてそのまま使える。`verify/atcoder_abc434_g.cpp`で公式sampleを確認済み。
。
