---
title: "列DP"
documentation_of: //cp/dp/sequence.hpp
---

# 列DP

- Header: `cp/dp/sequence.hpp`
- Symbol: `poe::lis`, `poe::lnds`, `poe::longest_nested_intervals`, `poe::lcs`, `poe::minimum_unbordered_pattern_increase`
- Status: experimental

## どんな問題に使えるか

元の順序を保って要素を間引く問題に使う。大小関係だけならLIS/LNDS、二列の一致要素を残すならLCSを選ぶ。
連続部分列、編集距離、重み付きLISは別問題であり、この戻り値をそのまま使えない。

## 使い方

```cpp
#include "dp/sequence.hpp"

auto result = poe::lis(values);
for (int index : result.indices) std::cout << values[index] << ' ';
auto nested = poe::longest_nested_intervals(intervals);
std::string common = poe::lcs(left, right);
```

`lis`は狭義、`lnds`は広義の最長増加部分列を元配列の添字で復元する。
`longest_nested_intervals`は`[left, right]`を外側から内側へ狭義包含する最長列を、
元の区間列の添字で復元する。端点が一致する区間同士は包含とみなさない。
`lcs`は文字列または`vector`の最長共通部分列を一つ復元する。

`minimum_unbordered_pattern_increase(text, pattern, k)`は、文字置換によって`pattern`の
出現数をちょうど`k`増やす最小置換数を返す。不可能なら`-1`。`ABC`や`docker`のように
空でなく自己重なりしない、すなわち真のprefixとsuffixが一致しないpattern専用である。
patternがborderを持つ場合はassertに失敗する。

## 戻り値と選択基準

- `lis`と`lnds`は値列ではなく元配列の添字列を返す。
- `lis`は`<`、`lnds`は`<=`を満たす部分列である。
- `lcs`は最長解を一つ返すが、複数解がある場合の辞書順は保証しない。

連続区間へ部分列APIを使わない。重み付きLISは区間最大値データ構造、巨大LCSはbitsetや
Hunt-Szymanskiなど、制約に応じた別解が必要になる。LCSは`O(nm)`領域も使う。

## 計算量

- LIS/LNDS・最長狭義包含区間列: `O(n log n)`時間、`O(n)`領域。
- LCS: `O(nm)`時間、`O(nm)`領域。
- 自己重なりしないpatternの出現増加: text長`n`、pattern長`m`、増加数`k`として
  `O(n(m+k))`時間、`O(nk)`領域。短いpatternと小さい`k`向け。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lis_result`

```cpp
struct lis_result
```

lis_result.indicesに狭義LISの元配列上の添字を保持する。

### `size`

```cpp
int size() const noexcept
```

O(1)。result.size(): LISの長さを返す。

### `lis`

```cpp
template <class T> lis_result lis(const std::vector<T>& values)
```

O(n log n)。lis(values): 狭義最長増加部分列を一つ復元する。

### `lnds`

```cpp
template <class T> lis_result lnds(const std::vector<T>& values)
```

O(n log n)。lnds(values): 広義最長増加部分列を一つ復元する。

### `longest_nested_intervals`

```cpp
template <class T> lis_result longest_nested_intervals(const std::vector<std::pair<T, T>>& intervals)
```

O(n log n)。longest_nested_intervals(intervals): 区間を外側から内側へ狭義包含する最長列を復元する。

### `lcs`

```cpp
template <class Sequence> Sequence lcs(const Sequence& left, const Sequence& right)
```

O(nm)時間・O(nm)領域。lcs(a, b): 最長共通部分列を一つ返す。

### `minimum_unbordered_pattern_increase`

```cpp
template <class Sequence> int minimum_unbordered_pattern_increase( const Sequence& text, const Sequence& pattern, int increase )
```

O(n(m+k))時間・O(nk)領域。自己重なりしないpatternの出現数をちょうどk増やす最小置換数。
minimum_unbordered_pattern_increase(text, pattern, k): 不可能なら-1。patternは空でなくborderを持たないこと。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [EDPC F - LCS](https://atcoder.jp/contests/dp/tasks/dp_f)
- [NDPC P - LIS](https://atcoder.jp/contests/ndpc/tasks/ndpc2026_p)ではLISが構成要素になるが、問題全体は固有の状態設計を要する。
- [AtCoder ABC462 F - More ABC](https://atcoder.jp/contests/abc462/tasks/abc462_f):
  `minimum_unbordered_pattern_increase(s, std::string("ABC"), k)`で解ける。
- [AtCoder ABC410 G - Longest Chord Chain](https://atcoder.jp/contests/abc410/tasks/abc410_g):
  各弦`a < b`を`[a,b]`と`[b,a+2N]`へ複製し、
  `longest_nested_intervals`の長さを求める。

```cpp
std::cout << poe::lcs(s, t) << '\n'; // EDPC F
```

`verify/atcoder_dp_f.cpp`で2026-07-15にjudge AC確認済み。ABC462 Fはverifyコードを用意し、
公式sampleを確認済みだがjudge ACは未確認。ABC410 Gもverifyコードを用意して公式sampleを確認済み。
API test、LIS・区間包含列・出現数増加の愚直差分も実施済み。
