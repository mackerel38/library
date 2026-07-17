---
title: "文字列の連結順"
documentation_of: //cp/string/concatenation.hpp
---

# 文字列の連結順

- Header: `cp/string/concatenation.hpp`
- Symbol: `poe::concatenation_order`, `poe::second_concatenation`,
  `poe::minimum_repeated_concatenation`
- Status: experimental

## どんな問題に使えるか

複数の文字列を並べ替えて連結し、辞書順を最小化する問題に使う。二文字列`x,y`の順は
`xy < yx`で比較する。各比較で実際に連結文字列を作らず、各文字列のZ-arrayを再利用するため、
一つだけ非常に長い文字列がある入力でも比較総量を抑えられる。

```cpp
#include "string/concatenation.hpp"

auto order = poe::concatenation_order(strings);
std::string second = poe::second_concatenation(strings);
std::string repeated = poe::minimum_repeated_concatenation(strings, k);
```

`second_concatenation`は、添字の順列をすべて列挙して得る連結文字列を重複込みで辞書順に並べた
2番目を返す。同じ文字列を作る異なる順列も別々に数える。

`minimum_repeated_concatenation(strings,k)`は、各回同じ候補集合から重複を許して文字列を選び、
ちょうど`k`個連結した結果の辞書順最小を返す。無限に選ぶ場合の最小周期語を決め、
そのprefixを候補文字列でちょうど`k`個に分割できる最短長を周期内位置DPで求める。

## 計算量

文字列長の総和を`L`、個数を`n`として`O(L log n)`時間、`O(L+n)`領域。
`minimum_repeated_concatenation`は最小周期長を`m`として
`O(Lm+km^2+|answer|)`時間、`O(L+m^2)`領域。候補長が小さい場合に向く。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `concatenation_order`

```cpp
inline std::vector<int> concatenation_order(const std::vector<std::string>& strings)
```

O(sum|s| log n)。全文字列の連結が辞書順最小になる添字順を返す。

### `second_concatenation`

```cpp
inline std::string second_concatenation(const std::vector<std::string>& strings)
```

O(sum|s| log n)。全順列の連結を重複込みで辞書順に並べた2番目を返す。要素数2以上。

### `minimum_repeated_concatenation`

```cpp
inline std::string minimum_repeated_concatenation( const std::vector<std::string>& strings, int k)
```

O(Lm+km^2+|answer|)。stringsから重複を許してちょうどk個選ぶ連結の辞書順最小を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC434 F - Concat (2nd)](https://atcoder.jp/contests/abc434/tasks/abc434_f)は、
全順列による連結の2番目を直接求める問題である。`verify/atcoder_abc434_f.cpp`で公式sampleを確認済み。
。

[AtCoder ABC416 G - Concat (1st)](https://atcoder.jp/contests/abc416/tasks/abc416_g)は、
同じ候補から重複を許してちょうど`K`個選ぶ最小連結である。
`verify/atcoder_abc416_g.cpp`で公式sampleを確認済み。
