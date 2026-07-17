---
title: "`suffix_array`"
documentation_of: //cp/string/suffixarray.hpp
---

# `suffix_array`

## Include

```cpp
#include "string/suffixarray.hpp"
```

文字列版はSA-ISで`O(n)`。値域上限付き整数列版は`O(n+upper)`、一般列版は
座標圧縮込み`O(n log n)`。空列にも対応する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `suffix_array`

```cpp
inline std::vector<int> suffix_array(const std::vector<int>& sequence, int upper)
```

O(n+upper)。0以上upper以下の整数列sequenceのsuffix arrayを返す。

### `suffix_array`

```cpp
template <class T> std::vector<int> suffix_array(const std::vector<T>& sequence)
```

O(n log n)。比較可能な列sequenceのsuffix arrayを返す。追加領域O(n)。

### `suffix_array`

```cpp
inline std::vector<int> suffix_array(const std::string& text)
```

O(n)。文字列textのsuffix arrayを返す。追加領域O(n)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Suffix Array](https://judge.yosupo.jp/problem/suffixarray)。

```cpp
auto suffix = poe::suffix_array(text);
for (int position : suffix) std::cout << position << ' ';
```

`verify/suffixarray.test.cpp`で2026-07-15に全ケースAC確認済み。
