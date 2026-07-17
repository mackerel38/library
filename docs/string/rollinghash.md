---
title: "Rolling Hash"
documentation_of: //cp/string/rollinghash.hpp
---

# Rolling Hash

- Status: experimental
- Header: `cp/string/rollinghash.hpp`
- Symbol: `poe::rollinghash<T>`

## Include

```cpp
#include "string/rollinghash.hpp"
```

## できること

文字列または`std::hash<T>`対応列を`2^61-1`を法とするhashへ変換し、
部分列hash・部分列一致・別の列とのLCPを求める。

```cpp
rollinghash<char> first(text), second(pattern);
bool same = first.equal(left, right, second, 0, pattern.size());
int common = first.lcp(left, text.size(), second, 0, pattern.size());
```

`hash(left, right)`は半開部分列のhash、`operator[]`は元要素を返す。
`concat(left_hash, right_hash, right_length)`は二つのhashを`O(1)`で連結するが、
呼び出し元オブジェクトが`right_length`以上のbase冪を持つ必要がある。

## 計算量

構築`O(n)`、部分列hash・一致・連結`O(1)`、LCP`O(log n)`、領域`O(n)`。

## 衝突について

hash一致は数学的な文字列一致を保証せず、極めて低い確率で衝突する。
誤判定を一切許容できない用途ではsuffix array、Z algorithm、KMPなどの決定的手法を使う。
baseは全オブジェクトで固定されているため、別オブジェクト間のhashを比較できる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rollinghash`

```cpp
template <class T = char> struct rollinghash
```

2^61-1 modの列hash: rollinghash<char> hash(text); 構築O(n)、部分列hash O(1)、LCP O(log n)。

### `rollinghash`

```cpp
explicit rollinghash(const std::vector<T>& values) : values_(values)
```

O(n)。valuesのprefix hashとbase冪を構築する。

### `rollinghash`

```cpp
explicit rollinghash(const std::basic_string<T>& text) : rollinghash(std::vector<T>(text.begin(), text.end()))
```

O(n)。文字列textから構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。列長を返す。

### `operator[]`

```cpp
const T& operator[](int index) const
```

O(1)。hash[index]で元の列の要素を返す。

### `hash`

```cpp
value_type hash(int left, int right) const
```

O(1)。半開区間[left,right)のhashを返す。

### `concat`

```cpp
value_type concat(value_type left_hash, value_type right_hash, int right_length) const
```

O(1)。left_hashの後ろへ長さright_lengthのright_hashを連結したhashを返す。

### `equal`

```cpp
bool equal(int left, int right, const rollinghash& other, int other_left, int other_right) const
```

O(1)。二つの半開部分列がhash上で等しいか返す。

### `lcp`

```cpp
int lcp(int left, int right, const rollinghash& other, int other_left, int other_right) const
```

O(log length)。二つの半開部分列のLCP長を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC141 E - Who Says a Pun?](https://atcoder.jp/contests/abc141/tasks/abc141_e)では、
二つのsuffixのLCPを求め、開始位置の差で上限を取ることで重ならない等しい部分文字列を探せる。
`verify/atcoder_abc141_e.cpp`を収録した。judge未提出。

## 検証

- `tests/api/string/rollinghash.cpp`: 部分列・別文字列LCP・連結
- `tests/property/string/rollinghash.cpp`: ランダム列の一致とLCPを愚直比較
- `verify/atcoder_abc141_e.cpp`: 公開問題用コード
