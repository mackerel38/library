---
title: "巡回シフト"
documentation_of: //cp/string/rotation.hpp
---

# 巡回シフト

- Status: experimental
- Header: `cp/string/rotation.hpp`
- Symbol: `poe::rotation_index`, `poe::minimum_rotation`, `poe::maximum_rotation`, `poe::rotated`, `poe::rotation_distance`

## Include

```cpp
#include "string/rotation.hpp"
```

## できること

列を途中で切って前後を入れ替えた全巡回シフトのうち、辞書順最小・最大の開始位置を
Booth法で`O(n)`に求める。

```cpp
int first = minimum_rotation(text);
string minimum = rotated(text, first);
string maximum = rotated(text, maximum_rotation(text));
```

`rotation_index(sequence, compare)`を使うと任意の比較規則を指定できる。
同じ最適巡回シフトが複数位置にある場合、そのうち最小の開始位置を返す。
空列の開始位置は`0`で、`rotated(empty, 0)`も空列となる。

時間`O(n)`、`rotation_index`の追加領域`O(1)`、`rotated`の返却領域`O(n)`。

`rotation_distance(source, target)`は`source`を左巡回シフトして`target`へ一致させる最小回数を返す。
一致する巡回シフトがなければ`-1`、両方が空なら`0`。KMPで`O(n)`時間・`O(n)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `rotation_index`

```cpp
template <class T, class Compare = std::less<T>> int rotation_index(const std::vector<T>& sequence, Compare compare =
```

O(n)。compareに関して辞書順最小の巡回シフト開始位置を返す。空列は0。

### `minimum_rotation`

```cpp
template <class T> int minimum_rotation(const std::vector<T>& sequence)
```

O(n)。辞書順最小の巡回シフト開始位置を返す。空列は0。

### `maximum_rotation`

```cpp
template <class T> int maximum_rotation(const std::vector<T>& sequence)
```

O(n)。辞書順最大の巡回シフト開始位置を返す。空列は0。

### `rotated`

```cpp
template <class T> std::vector<T> rotated(const std::vector<T>& sequence, int start)
```

O(n)。startから始まる巡回シフト列を返す。空列ではstart=0。

### `minimum_rotation`

```cpp
inline int minimum_rotation(const std::string& text)
```

O(n)。文字列の辞書順最小巡回シフト開始位置を返す。空文字列は0。

### `maximum_rotation`

```cpp
inline int maximum_rotation(const std::string& text)
```

O(n)。文字列の辞書順最大巡回シフト開始位置を返す。空文字列は0。

### `rotated`

```cpp
inline std::string rotated(const std::string& text, int start)
```

O(n)。startから始まる文字列の巡回シフトを返す。空文字列ではstart=0。

### `rotation_distance`

```cpp
template<class T> int rotation_distance(const std::vector<T>& source, const std::vector<T>& target)
```

O(n)。sourceを左巡回シフトしてtargetにする最小回数を返す。不可能なら-1。

### `rotation_distance`

```cpp
inline int rotation_distance(const std::string& source, const std::string& target)
```

O(n)。文字列sourceを左巡回シフトしてtargetにする最小回数を返す。不可能なら-1。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC223 B - String Shifting](https://atcoder.jp/contests/abc223/tasks/abc223_b)は、
文字列の最小・最大巡回シフトをそれぞれ出力する問題である。
`verify/atcoder_abc223_b.cpp`を収録した。judge未提出。

[AtCoder ABC430 E - Shift String](https://atcoder.jp/contests/abc430/tasks/abc430_e)では、
二つの入力文字列を`rotation_distance`へそのまま渡せる。
`verify/atcoder_abc430_e.cpp`で公式sampleを確認済み。judge ACは未確認。

## 検証

- `tests/api/string/rotation.cpp`: 文字列・一般列・周期列・空列
- `tests/property/string/rotation.cpp`: 全巡回シフトを生成する愚直解との比較
- `verify/atcoder_abc223_b.cpp`: 公開問題用コード
- `verify/atcoder_abc430_e.cpp`: 最小左巡回シフト回数
