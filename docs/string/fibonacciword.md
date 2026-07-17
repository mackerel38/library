---
title: "Fibonacci word"
documentation_of: //cp/string/fibonacciword.hpp
---

# Fibonacci word


## 概要

二つの非空文字列を`S1`, `S2`とし、`Si=S(i-1)+S(i-2)`で作る巨大文字列のprefixを、
実体化せずに扱う。文字の区間出現回数と添え字アクセスができる。

## 厳密な定義

- `fibonacciword`: 巨大Fibonacci wordの文字数・添字: fibonacciword word(first,second,max_length)。構築O(256 log max_length)。

## Include

```cpp
#include "string/fibonacciword.hpp"

fibonacciword word(first, second, maximum_position);
auto frequency = word.count(character, left, right); // [left,right)
char value = word[position];
```

構築`O(|first|+|second|+256 log maximum_position)`、各query`O(log maximum_position)`。
長さと個数は指定した最大長で飽和する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `fibonacciword`

```cpp
struct fibonacciword
```

巨大Fibonacci wordの文字数・添字: fibonacciword word(first,second,max_length)。構築O(256 log max_length)。

### `fibonacciword`

```cpp
fibonacciword(std::string first, std::string second, unsigned long long max_length = std::numeric_limits<unsigned long long>::max()) : bases_
```

O(|first|+|second|+256 log max_length)。必要なprefix長まで前計算する。

### `count`

```cpp
unsigned long long count(char character, unsigned long long right) const
```

O(log max_length)。十分大きいS_iの先頭right文字にcharacterが現れる回数を返す。

### `count`

```cpp
unsigned long long count(char character, unsigned long long left, unsigned long long right) const
```

O(log max_length)。半開区間[left,right)のcharacter出現回数を返す。

### `operator[]`

```cpp
char operator[](unsigned long long position) const
```

O(log max_length)。十分大きいS_iの0-indexed position文字目を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC450 E - Fibonacci String](https://atcoder.jp/contests/abc450/tasks/abc450_e)では、
`fibonacciword(X,Y,10^18)`を作り、各queryを`count(C,L-1,R)`で処理できる。
`verify/atcoder_abc450_e.cpp`に提出用コードを用意し、公式sampleを確認済み。
