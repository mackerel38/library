---
title: "Run-length encoding"
documentation_of: //cp/string/runlength.hpp
---

# Run-length encoding

- Status: experimental
- Header: `cp/string/runlength.hpp`
- Symbol: `poe::runlength`, `poe::runlength_decode`, `poe::runlength_string`

## Include

```cpp
#include "string/runlength.hpp"
```

## できること

連続する等しい要素を`(value, count)`へまとめる。文字列だけでなく一般のrangeへ使える。

```cpp
vector<pair<char, int>> runs = runlength(text);
for (auto [character, count] : runs) {
    // 同じcharacterがcount個連続する
}
string restored = runlength_string(runs);
```

`runlength_decode`は一般の`vector<pair<T,int>>`を`vector<T>`へ戻す。
countは非負でなければならず、0のrunは復元時に無視される。

符号化は`O(n)`、復元は復元後の長さに対して`O(n)`。返却領域も同じ。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `runlength`

```cpp
template <std::ranges::input_range Range> auto runlength(const Range& sequence)
```

O(n)。連続する等しい要素を(value, count)へまとめる。

### `runlength_decode`

```cpp
template <class T> std::vector<T> runlength_decode(const std::vector<std::pair<T, int>>& runs)
```

O(復元後の長さ)。非負のcountを持つ(value, count)列を元の列へ戻す。

### `runlength_string`

```cpp
inline std::string runlength_string(const std::vector<std::pair<char, int>>& runs)
```

O(復元後の長さ)。charの(value, count)列を文字列へ戻す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC019 B - 高橋くんと文字列圧縮](https://atcoder.jp/contests/abc019/tasks/abc019_2):
  各runの文字と長さを順に出力する。
- [AtCoder ABC259 C - XX to XXX](https://atcoder.jp/contests/abc259/tasks/abc259_c):
  二文字列のrunを比較し、長さ1のrunは増やせないという条件を判定する。
- [AtCoder ABC426 D - Pop and Insert](https://atcoder.jp/contests/abc426/tasks/abc426_d):
  文字ごとの総数と最長runを一度の符号化から集計する。

`verify/atcoder_abc019_b.cpp`、`verify/atcoder_abc259_c.cpp`、
`verify/atcoder_abc426_d.cpp`を収録した。ABC426 Dは公式sample一致。

## 検証

- `tests/api/string/runlength.cpp`: 文字列・整数列・空列・decode
- `tests/property/string/runlength.cpp`: ランダム列の符号化後復元とrun不変条件
