---
title: "Palindromic Tree"
documentation_of: //cp/string/palindromictree.hpp
---

# Palindromic Tree

- Status: experimental
- Header: `cp/string/palindromictree.hpp`
- Symbol: `poe::palindromictree<Symbol>`, `poe::palindromenode<Symbol>`

## Include

```cpp
#include "string/palindromictree.hpp"
```

## できること

列に現れる相異なる回文を高々`n+2`状態で表す回文木（eertree）。

```cpp
palindromictree<char> tree(text);
int kinds = tree.distinct_palindromes();
auto longest = tree.longest_palindrome();
auto partition = tree.minimum_partition();
```

- `longest_palindrome`: 元の列における`[begin,end)`、状態、長さを返す。
- `palindrome(state)`: 状態が表す回文列を復元する。
- `suffix_state(position)`: その位置で終わる最長回文suffix。
- `suffixes(position)`: その位置で終わる全回文状態を長い順で列挙。
- `minimum_partition`: 元の列を最少個数の回文へ分け、半開区間列も復元。
- `operator[]`: `next`、suffix `link`、`length`、`occurrence`、`first_end`、`terminal`。

default constructorへ`add`してonline構築もできる。出現回数やterminalを使う前に`build()`を呼び、
追加後には再度`build()`する。constructorへ列を渡した場合は自動でbuildされる。

## 計算量

分岐数を`sigma`として構築はならし`O(n log sigma)`、状態数・領域は`O(n)`。
最長回文・相異なる回文数・各位置の最長suffixは`O(1)`、回文復元は`O(length)`。
`minimum_partition`は全prefixの回文suffixを列挙するため最悪`O(n^2)`、領域`O(n)`。
長い同一文字列などで分割だけが必要な最大制約では、将来のseries link版を選ぶ余地がある。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `palindromenode`

```cpp
template <class Symbol> struct palindromenode
```

回文木の一状態。next、suffix link、回文長、出現数、最初の終了位置を持つ。

### `palindromematch`

```cpp
struct palindromematch
```

最長回文結果。元の列の半開区間[begin,end)と回文木stateを持つ。

### `length`

```cpp
int length() const noexcept
```

O(1)。回文長を返す。

### `palindromepartitionresult`

```cpp
struct palindromepartitionresult
```

最小回文分割結果。rangesは元の列を覆う半開区間列。

### `operator[]`

```cpp
const std::pair<int, int>& operator[](int index) const
```

O(1)。index番目の回文区間を返す。

### `palindromictree`

```cpp
template <class Symbol = char> struct palindromictree
```

相異なる回文をonline構築する: palindromictree<char> tree(text); 構築ならしO(n log sigma)。

### `palindromictree`

```cpp
palindromictree() : nodes_(2)
```

O(1)。長さ-1と0の根だけを持つ空の回文木を作る。

### `palindromictree`

```cpp
template <std::ranges::input_range Range> explicit palindromictree(const Range& sequence) : palindromictree()
```

ならしO(n log sigma)。sequenceを順に追加してbuildする。

### `add`

```cpp
int add(const Symbol& symbol)
```

ならしO(log sigma)。末尾へsymbolを追加し、新しい列の最長回文suffix状態を返す。

### `add`

```cpp
template <std::ranges::input_range Range> void add(const Range& sequence)
```

ならしO(length log sigma)。sequenceを末尾へ追加する。

### `build`

```cpp
void build()
```

O(states log states)。各回文の出現回数と末尾suffix状態を確定する。

### `distinct_palindromes`

```cpp
int distinct_palindromes() const noexcept
```

O(1)。空でない相異なる回文数を返す。

### `longest_palindrome`

```cpp
palindromematch longest_palindrome() const
```

O(1)。元の列全体から最長回文の区間と状態を返す。

### `palindrome`

```cpp
std::vector<Symbol> palindrome(int state) const
```

O(length)。stateが表す回文列を返す。state>=2。

### `suffix_state`

```cpp
int suffix_state(int position) const
```

O(1)。positionで終わる最長回文suffixの状態を返す。

### `suffixes`

```cpp
std::vector<int> suffixes(int position) const
```

O(その位置で終わる回文数)。positionで終わる全非空回文状態を長い順で返す。

### `minimum_partition`

```cpp
palindromepartitionresult minimum_partition() const
```

O(全prefixの回文suffix数)、最悪O(n^2)。元の列の最小回文分割と区間を返す。

### `operator[]`

```cpp
const palindromenode<Symbol>& operator[](int state) const
```

O(1)。状態情報を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。状態数を返す。根2個を含みn+2以下。

### `length`

```cpp
int length() const noexcept
```

O(1)。元の列長を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC320 B - Longest Palindrome](https://atcoder.jp/contests/abc320/tasks/abc320_b)は、
`longest_palindrome().length()`で答えられる。
`verify/atcoder_abc320_b.cpp`を収録した。judge未提出。

## 検証

- `tests/api/string/palindromictree.cpp`: 状態・出現回数・suffix・最小分割
- `tests/property/string/palindromictree.cpp`: 全substringの愚直回文集合・出現数・分割DPとの比較
- `verify/atcoder_abc320_b.cpp`: 公開問題用コード
