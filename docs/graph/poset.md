---
title: "poset"
documentation_of: //cp/graph/poset.hpp
---

# `poset`

## 概要

「互いに大小比較できない要素を最大でいくつ選べるか」を半順序から求める。
実際の最大反鎖と、全要素を最少本数のchainへ分ける方法も返す。

## 厳密な定義

有限集合 $V=\{0,1,\ldots,n-1\}$ とstrict order $<$ が与えられる。
反鎖 $A\subseteq V$ は、異なる任意の $x,y\in A$ について $x<y$ でも $y<x$ でもない集合である。
`maximum_poset_antichain` は $|A|$ が最大の反鎖と、各chain内の要素が全順序になる最小chain分割を返す。
Dilworthの定理により、この二つの大きさは等しい。

## Include

```cpp
#include "graph/poset.hpp"
```

## API

```cpp
struct posetantichainresult {
    vector<int> antichain;
    vector<vector<int>> chains;
    int size() const;
};

auto result = maximum_poset_antichain(size, less);
```

`less(i, j)` は $i<j$ なら `true` を返す。呼び出し側は、`less` が非反射的かつ推移的なstrict partial orderであることを保証する。
特にDAGの辺だけでなく、到達可能な全頂点対を渡す必要がある。

`antichain` は最大反鎖、`chains` は最小chain分割である。各chainは小さい順に並ぶ。
二部最大matchingとDilworthの定理を用いる。比較を $O(1)$ とすると計算量は $O(n^2+n^2\sqrt n)$、使用メモリは $O(n^2)$。

## 使用例

[AtCoder ABC237 Ex - Hakata](https://atcoder.jp/contests/abc237/tasks/abc237_h)では、相異なる回文を回文木で列挙し、文字列包含を半順序として渡す。

```cpp
auto result = maximum_poset_antichain(palindromes.size(), [&](int i, int j) {
    return palindromes[i].size() < palindromes[j].size()
        && palindromes[j].find(palindromes[i]) != string::npos;
});
cout << result.size() << '\n';
```

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `posetantichainresult`

```cpp
struct posetantichainresult
```

半順序の最大反鎖と、それと同数の最小chain分割。

### `size`

```cpp
int size() const noexcept
```

O(1)。最大反鎖の大きさを返す。

### `maximum_poset_antichain`

```cpp
template<class StrictOrder> posetantichainresult maximum_poset_antichain(int size, StrictOrder less)
```

O(n^2+n^2sqrt(n))。strict order less(i,j)で与えた半順序の最大反鎖と最小chain分割を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
