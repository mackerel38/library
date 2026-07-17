---
title: "twosat"
documentation_of: //cp/graph/twosat.hpp
---

# `twosat`

## Include

```cpp
#include "graph/twosat.hpp"
```

`add_clause(i, f, j, g)`で`(x_i=f) or (x_j=g)`を追加し、`O(n+m)`で充足可能性を判定する。
`force/imply/add_xor/add_equal/at_most_one`も提供する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `twosat`

```cpp
struct twosat
```

2-SATを解く: twosat sat(n); sat.add_clause(i, f, j, g); 計算O(n+m)。

### `twosat`

```cpp
explicit twosat(int n) : n_(n), graph_(2 * n), answer_(n)
```

O(n)。n個の真偽変数を作る。

### `add_clause`

```cpp
void add_clause(int i, bool f, int j, bool g)
```

O(1)。(x_i=f)または(x_j=g)という節を追加する。

### `force`

```cpp
void force(int i, bool value)
```

O(1)。x_i=valueを必須にする。

### `imply`

```cpp
void imply(int i, bool from_value, int j, bool to_value)
```

O(1)。(x_i=from_value)ならば(x_j=to_value)という含意を追加する。

### `add_xor`

```cpp
void add_xor(int i, bool f, int j, bool g)
```

O(1)。リテラル(x_i=f)と(x_j=g)のちょうど一方を真にする。

### `add_equal`

```cpp
void add_equal(int i, bool f, int j, bool g)
```

O(1)。リテラル(x_i=f)と(x_j=g)の真偽を一致させる。

### `at_most_one`

```cpp
void at_most_one(const std::vector<std::pair<int, bool>>& literals)
```

O(k^2)。指定したリテラルのうち高々一つだけを真にする。

### `satisfiable`

```cpp
bool satisfiable()
```

O(n+m)。充足可能ならtrueを返し、answer()を構築する。

### `answer`

```cpp
const std::vector<bool>& answer() const
```

O(1)。直前のsatisfiable()で得た割当を返す。

### `operator[]`

```cpp
bool operator[](int index) const
```

O(1)。sat[index]は直前に得た変数indexの割当を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。変数数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Two SAT](https://judge.yosupo.jp/problem/two_sat)。

```cpp
poe::twosat sat(variable_count);
sat.add_clause(left_variable, left_value, right_variable, right_value);
if (!sat.satisfiable()) std::cout << "s UNSATISFIABLE\n";
else for (bool value : sat.answer()) std::cout << value << ' ';
```

候補問題。
