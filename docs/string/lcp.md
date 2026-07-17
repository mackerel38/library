---
title: "lcp_array"
documentation_of: //cp/string/lcp.hpp
---

# `lcp_array`

## 概要

Kasai法によるLCP配列を扱う。

## 厳密な定義

- `lcp_array`: O(n)。sequenceとそのsuffix arrayから隣接suffix間のLCP配列を返す。


## Include

```cpp
#include "string/lcp.hpp"
```

列とそのsuffix arrayから、辞書順で隣接するsuffixのLCP長を`O(n)`で返す。
空列と要素数1では空配列になる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lcp_array`

```cpp
template <class T> std::vector<int> lcp_array(const std::vector<T>& sequence, const std::vector<int>& suffix_array)
```

O(n)。sequenceとそのsuffix arrayから隣接suffix間のLCP配列を返す。

### `lcp_array`

```cpp
inline std::vector<int> lcp_array(const std::string& text, const std::vector<int>& suffix_array)
```

O(n)。textとそのsuffix arrayから隣接suffix間のLCP配列を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Number of Substrings](https://judge.yosupo.jp/problem/number_of_substrings)。

```cpp
auto suffix = poe::suffix_array(text);
auto lcp = poe::lcp_array(text, suffix);
long long answer = 1LL * text.size() * (text.size() + 1) / 2;
answer -= std::accumulate(lcp.begin(), lcp.end(), 0LL);
```

候補問題。
