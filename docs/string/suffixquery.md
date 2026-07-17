---
title: "suffixquery"
documentation_of: //cp/string/suffixquery.hpp
---

# `suffixquery`

## Include

```cpp
#include "string/suffixquery.hpp"
```

文字列からsuffix array、rank、LCPのRMQを`O(n log n)`で構築する。
suffix間LCPと部分文字列比較は`O(1)`、pattern検索は`O(|pattern| log n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `suffixquery`

```cpp
struct suffixquery
```

suffix間LCP・部分文字列比較・pattern検索: suffixquery query(text); 構築O(n log n)。

### `suffixquery`

```cpp
explicit suffixquery(std::string text) : text_(std::move(text)), suffix_array_(poe::suffix_array(text_)), rank_(text_.size())
```

O(n log n)。textのsuffix array、LCP、RMQを構築する。

### `lcp`

```cpp
int lcp(int first, int second) const
```

O(1)。suffix text[first..]とtext[second..]のLCP長を返す。

### `compare`

```cpp
int compare(int first_left, int first_right, int second_left, int second_right) const
```

O(1)。二つの部分文字列を辞書順比較し、左が小/同じ/大なら-1/0/1を返す。

### `less`

```cpp
bool less(int first_left, int first_right, int second_left, int second_right) const
```

O(1)。左の部分文字列が右より辞書順で小さいならtrueを返す。

### `equal_range`

```cpp
std::pair<int, int> equal_range(const std::string& pattern) const
```

O(|pattern| log n)。patternで始まるsuffixのSA上半開区間を返す。

### `contains`

```cpp
bool contains(const std::string& pattern) const
```

O(|pattern| log n)。textがpatternを含むならtrueを返す。

### `suffix_array`

```cpp
const std::vector<int>& suffix_array() const noexcept
```

O(1)。suffix arrayを返す。

### `rank`

```cpp
const std::vector<int>& rank() const noexcept
```

O(1)。suffix開始位置ごとの順位を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。文字列長を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AOJ ALDS1_14_D - Multiple String Matching](https://onlinejudge.u-aizu.ac.jp/problems/ALDS1_14_D)。

```cpp
poe::suffixquery query(text);
while (query_count--) {
    std::string pattern;
    std::cin >> pattern;
    std::cout << query.contains(pattern) << '\n';
}
```

候補問題。
