---
title: "除外整数集合"
documentation_of: //cp/structure/excludedset.hpp
---

# 除外整数集合

- Header: `cp/structure/excludedset.hpp`
- Symbol: `poe::excludedset`
- Status: experimental

## どんな問題に使えるか

変更されない有限の整数集合を「使用禁止」とし、ある下限以上で使用可能な整数を
小さい順に数える問題に使う。禁止集合は構築時にソート・重複除去される。

```cpp
#include "structure/excludedset.hpp"

poe::excludedset<long long> data(forbidden);
long long answer = data.kth_allowed(lower, rank);
```

構築は`O(n log n)`、`contains`と`kth_allowed`は`O(log n)`。
`rank`は1-indexedである。答えや途中の差が型`T`の範囲を超えないことを呼び出し側で保証する。

## 仕組み

`lower`以上の最初の禁止値を二分探索する。昇順禁止列の位置`i`までに存在する
使用可能整数の個数は「区間内の整数数 - 禁止値数」であり、`i`について単調に増える。
この個数が`rank`以上になる最初の位置をもう一度二分探索する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `excludedset`

```cpp
template <std::integral T> struct excludedset
```

固定された除外整数集合: excludedset<long long> data(values)。

### `excludedset`

```cpp
explicit excludedset(std::vector<T> values) : values_(std::move(values))
```

O(n log n)。valuesを除外集合として構築し、重複を取り除く。

### `contains`

```cpp
bool contains(T value) const
```

O(log n)。valueが除外されているならtrueを返す。

### `kth_allowed`

```cpp
T kth_allowed(T lower, T rank) const
```

O(log n)。lower以上かつ除外されていない整数のうちrank番目を返す。rankは1以上。

### `size`

```cpp
int size() const noexcept
```

O(1)。除外整数の個数を返す。

### `values`

```cpp
const std::vector<T>& values() const noexcept
```

O(1)。昇順の除外整数列を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC440 D - Forbidden List 2](https://atcoder.jp/contests/abc440/tasks/abc440_d)では、
各質問を`data.kth_allowed(X, Y)`で処理できる。
`verify/atcoder_abc440_d.cpp`で公式sampleを確認済み。judge ACは未確認。

property testでは小さな禁止集合に対する愚直な整数走査と照合している。
