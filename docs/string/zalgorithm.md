---
title: "Z algorithm"
documentation_of: //cp/string/zalgorithm.hpp
---

# Z algorithm

## Include

```cpp
#include "string/zalgorithm.hpp"
```

`z[i] = LCP(sequence, sequence[i..])`となる配列を`O(n)`で返す。
空列は空配列、非空列では`z[0]`が列長になる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `z_algorithm`

```cpp
template <class T> std::vector<int> z_algorithm(const std::vector<T>& sequence)
```

O(n)。z[i]=LCP(sequence, sequence[i..])となるZ配列を返す。

### `z_algorithm`

```cpp
inline std::vector<int> z_algorithm(const std::string& text)
```

O(n)。文字列textのZ配列を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Z Algorithm](https://judge.yosupo.jp/problem/zalgorithm)。

```cpp
auto z = poe::z_algorithm(text);
for (int length : z) std::cout << length << ' ';
```

候補問題へのjudge ACは未確認。
