---
title: "辞書順部分列"
documentation_of: //cp/string/subsequence.hpp
---

# 辞書順部分列

- Header: `cp/string/subsequence.hpp`
- Symbol: `poe::count_substrings_without_subsequence`, `poe::kth_distinct_subsequence`
- Status: experimental

## どんな問題に使えるか

文字列から順序を保って文字を間引いた、相異なる空でない部分列を辞書順で扱う問題に使う。
各文字を次に使う最左位置へ正規化して重複を除き、個数を`k+1`で飽和させながらk番目を復元する。

部分文字列、重複を区別する部分列、辞書順以外の順序には使えない。alphabetは昇順・重複なしで、
入力に現れる全文字を含める必要がある。

また、各位置を区別する部分文字列のうち、指定列を部分列として含まないものも数えられる。
各長さのprefixを作れる最も右の開始位置を更新するため、文字種には依存しない。

## API・計算量

```cpp
#include "string/subsequence.hpp"

auto answer = poe::kth_distinct_subsequence(s, k);
long long count = poe::count_substrings_without_subsequence(s, pattern);
std::cout << (answer ? *answer : "Eel") << '\n';
```

- `kth_distinct_subsequence`: `O(n|alphabet|)`時間・領域。
- `count_substrings_without_subsequence`: `O(|text||pattern|)`時間、`O(|pattern|)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_substrings_without_subsequence`

```cpp
template <class Sequence> long long count_substrings_without_subsequence( const Sequence& text, const Sequence& pattern )
```

O(|text||pattern|)。patternを部分列として含まないtextの空でない部分文字列の個数を返す。

### `kth_distinct_subsequence`

```cpp
inline std::optional<std::string> kth_distinct_subsequence( const std::string& text, unsigned long long k, const std::string& alphabet = "abcdefghijklmnopqrstuvwxyz" )
```

O(n|alphabet|)。kth_distinct_subsequence(text,k)で辞書順k番目(1-indexed)の空でない部分列を返す。
存在しなければnullopt。alphabetは重複なし昇順で、textの全文字を含むこと。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[TDPC G - 辞書順](https://atcoder.jp/contests/tdpc/tasks/tdpc_lexicographical)へ直接使える。
`verify/atcoder_tdpc_lexicographical.cpp`で2026-07-15にjudge AC確認済み。

[AtCoder ABC452 D - No-Subsequence Substring](https://atcoder.jp/contests/abc452/tasks/abc452_d)へ
`count_substrings_without_subsequence(S, T)`を直接使える。
`verify/atcoder_abc452_d.cpp`に提出用コードを用意し、公式sampleを確認済み。judge ACは未確認。
