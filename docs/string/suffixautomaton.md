---
title: "Suffix Automaton"
documentation_of: //cp/string/suffixautomaton.hpp
---

# Suffix Automaton

- Status: experimental
- Header: `cp/string/suffixautomaton.hpp`
- Symbol: `poe::suffixautomaton<Symbol>`, `poe::suffixstate<Symbol>`, `poe::suffixmatch`

## Include

```cpp
#include "string/suffixautomaton.hpp"
```

## できること

一つの列に含まれる全連続部分列を高々`2n`状態のDFAで表す。

```cpp
suffixautomaton<char> automaton(text);
bool exists = automaton.contains(pattern);
long long occurrences = automaton.count(pattern);
unsigned long long kinds = automaton.distinct_substrings();
auto third = automaton.kth(2);
auto common = automaton.longest_common_substring(other);
```

- `count`: 重なりを含む出現回数。空patternは`length()+1`回とする。
- `distinct_substrings`: 空でない相異なる連続部分列数。
- `kth(k)`: 0-indexed辞書順k番目を`vector<Symbol>`で返す。存在しなければ`nullopt`。
- `longest_common_substring`: 別列との最長共通連続部分列。別列側の`[begin(), end)`も返す。
- `longest_repeated_substring`: 二回以上現れる連続部分列の最大長。
- `operator[]`: 状態の`next`、`link`、`length`、`occurrence`、`first_end`、`terminal`。

default constructorへ`add`でオンライン追加もできるが、出現回数やterminal情報を使う前に
`build()`を呼ぶ。追加済みautomatonへさらに追加した場合も再度`build()`が必要。

## 計算量

分岐数を`sigma`とすると構築はならし`O(n log sigma)`、状態数・領域は`O(n)`。
`contains`・`count`は`O(pattern length log sigma)`、相異なる部分列数と最長反復は`O(states)`、
最長共通部分列は`O(other length log sigma)`、`kth`は`O(states + answer length)`。

相異なる部分列数は最大`n(n+1)/2`なので`unsigned long long`で返す。
さらに大きな概念上の列を扱う場合、`kth`内部の個数は`ULLONG_MAX`で飽和する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `suffixstate`

```cpp
template <class Symbol> struct suffixstate
```

suffix automatonの一状態。next、suffix link、最長長、出現数、最初の終了位置を持つ。

### `suffixmatch`

```cpp
struct suffixmatch
```

別列との最長共通部分列結果。substringは連続部分列を表す。

### `begin`

```cpp
int begin() const noexcept
```

O(1)。別列側の一致開始位置を返す。

### `suffixautomaton`

```cpp
template <class Symbol = char> struct suffixautomaton
```

全連続部分列を表す最小DFA: suffixautomaton<char> sam(text); 構築ならしO(n log sigma)。

### `suffixautomaton`

```cpp
suffixautomaton() : states_(1)
```

O(1)。空列のautomatonを作る。

### `suffixautomaton`

```cpp
template <std::ranges::input_range Range> explicit suffixautomaton(const Range& sequence) : suffixautomaton()
```

ならしO(n log sigma)。sequenceを順に追加してbuildする。

### `add`

```cpp
int add(const Symbol& symbol)
```

ならしO(log sigma)。末尾へsymbolを追加し、新しい列全体に対応する状態を返す。

### `add`

```cpp
template <std::ranges::input_range Range> void add(const Range& sequence)
```

ならしO(length log sigma)。sequenceを末尾へ追加する。

### `build`

```cpp
void build()
```

O(states log states)。出現回数とsuffix状態を確定する。追加後にcount等を使う前に呼ぶ。

### `contains`

```cpp
template <std::ranges::input_range Range> bool contains(const Range& pattern) const
```

O(length log sigma)。patternが連続部分列として存在するか返す。

### `count`

```cpp
template <std::ranges::input_range Range> long long count(const Range& pattern) const
```

O(length log sigma)。patternの重なりを含む出現回数を返す。build後に使う。

### `distinct_substrings`

```cpp
unsigned long long distinct_substrings() const
```

O(states)。空でない相異なる連続部分列数を返す。

### `kth`

```cpp
std::optional<std::vector<Symbol>> kth(unsigned long long k) const
```

O(states + answer length)。0-indexed辞書順k番目の相異なる非空部分列。なければnullopt。

### `longest_common_substring`

```cpp
template <std::ranges::input_range Range> suffixmatch longest_common_substring(const Range& other) const
```

O(other length log sigma)。別列との最長共通連続部分列の長さと別列側終了位置を返す。

### `longest_repeated_substring`

```cpp
int longest_repeated_substring() const
```

O(states)。二回以上現れる連続部分列の最大長を返す。build後に使う。

### `operator[]`

```cpp
const suffixstate<Symbol>& operator[](int state) const
```

O(1)。状態情報を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。状態数を返す。2n以下。

### `length`

```cpp
int length() const noexcept
```

O(1)。元の列長を返す。

### `last_state`

```cpp
int last_state() const noexcept
```

O(1)。元の列全体に対応する状態を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC347 B - Substring](https://atcoder.jp/contests/abc347/tasks/abc347_b)は、
`distinct_substrings()`をそのまま答えられる。
`verify/atcoder_abc347_b.cpp`を収録した。judge未提出。

## 検証

- `tests/api/string/suffixautomaton.cpp`: 存在・回数・k番目・LCS・オンライン追加
- `tests/property/string/suffixautomaton.cpp`: 小さいランダム列の全substring集合と比較
- `verify/atcoder_abc347_b.cpp`: 公開問題用コード
