---
title: "Prefix function / KMP検索"
documentation_of: //cp/string/prefix.hpp
---

# Prefix function / KMP検索

## Include

```cpp
#include "string/prefix.hpp"
```

`prefix_function`はKMPのprefix配列を`O(n)`、`find_all`はpatternの全出現位置を
`O(n+m)`で返す。空patternは`0..n`を返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `prefix_function`

```cpp
template <class T> std::vector<int> prefix_function(const std::vector<T>& sequence)
```

O(n)。prefix[i]=sequence[0..k)とsequence[0..i]の最長一致長kを返す。

### `prefix_function`

```cpp
inline std::vector<int> prefix_function(const std::string& text)
```

O(n)。文字列textのprefix functionを返す。

### `find_all`

```cpp
template <class T> std::vector<int> find_all(const std::vector<T>& text, const std::vector<T>& pattern)
```

O(n+m)。text内でpatternが始まる全位置を昇順で返す。空patternは0..nを返す。

### `find_all`

```cpp
inline std::vector<int> find_all(const std::string& text, const std::string& pattern)
```

O(n+m)。text内でpatternが始まる全位置を昇順で返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ ALDS1_14_B - String Search](https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_B)。

```cpp
for (int position : poe::find_all(text, pattern)) {
    std::cout << position << '\n';
}
```

候補問題。
