---
title: "Manacher法"
documentation_of: //cp/string/manacher.hpp
---

# Manacher法

## Include

```cpp
#include "string/manacher.hpp"
```

`manacher_odd`と`manacher_even`は、各中心の奇数長・偶数長回文半径を`O(n)`で返す。
奇数長は`2*odd[i]-1`、境界`(i-1,i)`中心の偶数長は`2*even[i]`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `manacher_odd`

```cpp
template <class T> std::vector<int> manacher_odd(const std::vector<T>& sequence)
```

O(n)。odd[i]を中心iの奇数長回文半径として返す。回文長は2*odd[i]-1。

### `manacher_even`

```cpp
template <class T> std::vector<int> manacher_even(const std::vector<T>& sequence)
```

O(n)。even[i]を境界(i-1,i)中心の偶数長回文半径として返す。回文長は2*even[i]。

### `manacher_odd`

```cpp
inline std::vector<int> manacher_odd(const std::string& text)
```

O(n)。文字列textの奇数長回文半径を返す。

### `manacher_even`

```cpp
inline std::vector<int> manacher_even(const std::string& text)
```

O(n)。文字列textの偶数長回文半径を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Enumerate Palindromes](https://judge.yosupo.jp/problem/enumerate_palindromes)。

```cpp
auto odd = poe::manacher_odd(text);
auto even = poe::manacher_even(text);
for (int center = 0; center < static_cast<int>(text.size()); ++center) {
    std::cout << 2 * odd[center] - 1 << ' ';
    if (center + 1 < static_cast<int>(text.size())) std::cout << 2 * even[center + 1] << ' ';
}
```

候補問題。
