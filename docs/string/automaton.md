---
title: "DFA・右文脈からの構築・部分文字列集計"
documentation_of: //cp/string/automaton.hpp
---

# DFA・右文脈からの構築・部分文字列集計

- Header: `cp/string/automaton.hpp`
- Symbol: `poe::dfa`, `poe::right_context_dfa`, `poe::accepted_substring_stats`
- Status: experimental

```cpp
#include "string/automaton.hpp"
```

## DFA

alphabetを`0..sigma-1`で表す決定性有限automaton。`next[state][symbol]`と`accepting[state]`を持ち、
`accepts(word, project)`で語を判定する。

## 受理される部分文字列

```cpp
auto stats = poe::accepted_substring_stats(automaton, text, [](char c) { return c - '0'; });
cout << stats.longest << ' ' << stats.count << '\n';
```

各位置で「そこを右端とする全部分文字列」の状態別個数と最早左端を更新する。
文字列長を`n`、状態数を`q`として`O(nq)`時間・`O(q)`領域。
空文字列は数えず、受理される非空部分文字列がなければ`longest=-1, count=0`を返す。

## membership oracleからの候補DFA

`right_context_dfa(sigma, u, accepts)`は、prefixを「長さ`u`以下の全suffixを加えたときの
membership結果」で分類し、Myhill--Nerode型の候補DFAを構築する。

```cpp
auto automaton = poe::right_context_dfa(2, 3, [&](const vector<int>& word) {
    return accepts_word(word);
});
```

oracle呼び出し回数と構築量は、文脈数`1+sigma+...+sigma^u`と生成された状態数に比例する。
一般の言語について有限`u`だけで正しさは保証されない。対象言語で右文脈同値類がこの深さまでに
安定することを別途証明するか、十分なholdout語で照合して使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `dfa`

```cpp
struct dfa
```

決定性有限automaton。alphabetは0..alphabet_size-1の整数で表す。

### `size`

```cpp
int size() const noexcept
```

O(1)。状態数を返す。

### `alphabet_size`

```cpp
int alphabet_size() const noexcept
```

O(1)。alphabetの大きさを返す。

### `accepts`

```cpp
template<std::ranges::input_range Range, class Project = std::identity> bool accepts(const Range& word, Project project =
```

O(|word|)。各要素をprojectでalphabet番号へ変換し、wordを受理するか返す。

### `acceptedsubstringstats`

```cpp
struct acceptedsubstringstats
```

accepted_substring_statsの結果。受理部分文字列がなければlongest=-1、count=0。

### `accepted_substring_stats`

```cpp
template<std::ranges::input_range Range, class Project = std::identity> acceptedsubstringstats accepted_substring_stats( const dfa& automaton, const Range& text, Project project =
```

O(nq)。q状態DFAが受理する非空部分文字列の最長長と位置を区別した総数を返す。

### `right_context_dfa`

```cpp
template<class Accept> dfa right_context_dfa(int alphabet_size, int context_length, Accept accepts)
```

membership oracleを長さcontext_length以下の右文脈で分類し、候補DFAを構築する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC418 G - Binary Operation](https://atcoder.jp/contests/abc418/tasks/abc418_g)では、
二項演算の任意括弧付けで`1`へ縮約できるかをoracleとする。公式解説により全16演算で
`u<=3`に安定し、状態数は7以下であることが示されている。
`verify/atcoder_abc418_g.cpp`で公式2 sampleを確認済み。judge ACは未確認。
