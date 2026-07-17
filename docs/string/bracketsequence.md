---
title: "動的括弧列"
documentation_of: //cp/string/bracketsequence.hpp
---

# 動的括弧列

- Header: `cp/string/bracketsequence.hpp`
- Symbol: `poe::bracketsequence`
- Status: experimental

## どんな問題に使えるか

`(`と`)`を末尾へ追加・削除しながら、現在の列が正しい括弧列かを判定する。

```cpp
#include "string/bracketsequence.hpp"

bracketsequence brackets;
brackets.push('(');
brackets.push(')');
if (brackets.good()) cout << "Yes\n";
brackets.pop();
```

現在のbalance、全prefix balanceの最小値を履歴stackへ保存する。良い括弧列である必要十分条件は、
全体のbalanceが0かつprefix最小値が0であること。

`back`、`operator[]`、`size`、`empty`、`string`も使える。途中位置の変更や区間queryが必要な場合は、
和と最小prefix和をmonoidにしたSegment Treeを使う。

## 計算量

構築`O(n)`。`push`は償却`O(1)`、`pop`、状態取得、判定は`O(1)`。領域`O(n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `bracketsequence`

```cpp
struct bracketsequence
```

末尾push/popできる括弧列: bracketsequence brackets; brackets.push('('); brackets.good()。

### `bracketsequence`

```cpp
bracketsequence() : balance_
```

O(1)。空の括弧列を作る。

### `bracketsequence`

```cpp
explicit bracketsequence(std::string_view text) : bracketsequence()
```

O(n)。'('と')'からなるtextを格納する。

### `push`

```cpp
void push(char bracket)
```

償却O(1)。末尾へbracketを追加する。

### `pop`

```cpp
void pop()
```

O(1)。空でない列の末尾を削除する。

### `back`

```cpp
char back() const
```

O(1)。末尾の括弧を返す。

### `operator[]`

```cpp
char operator[](int index) const
```

O(1)。index番目の括弧を返す。

### `balance`

```cpp
int balance() const noexcept
```

O(1)。現在の開き括弧数-閉じ括弧数を返す。

### `minimum_balance`

```cpp
int minimum_balance() const noexcept
```

O(1)。prefix balanceの最小値を返す。空prefixを含む。

### `good`

```cpp
bool good() const noexcept
```

O(1)。現在の列が良い括弧列ならtrueを返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。文字数を返す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。空ならtrueを返す。

### `reserve`

```cpp
void reserve(int count)
```

O(n)。保持領域をcount文字分予約する。

### `string`

```cpp
const std::string& string() const noexcept
```

O(1)。現在の文字列を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC428 C - Brackets Stack Query](https://atcoder.jp/contests/abc428/tasks/abc428_c):
  各push/pop直後に`good()`を出力する。

`verify/atcoder_abc428_c.cpp`に提出用コードを収録し、公式sampleとの一致を確認済み。
judge ACは未確認。

`tests/property/string/bracketsequence.cpp`では、ランダムpush/pop後に全prefixを直接走査して比較する。
