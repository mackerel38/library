---
title: "Wildcard文字列照合"
documentation_of: //cp/string/matching.hpp
---

# Wildcard文字列照合

- Header: `cp/string/matching.hpp`
- Symbol: `poe::wildcard_matches`
- Status: experimental

## どんな問題に使えるか

`text`の各部分文字列と`pattern`を比較し、同じ位置の文字が等しいか、
少なくとも一方がwildcardである開始位置をすべて返す。

```cpp
#include "string/matching.hpp"

auto starts = poe::wildcard_matches(text, pattern, '_');
```

文字を座標圧縮した値`a,b`について、非wildcard同士の
`(a-b)^2`をalignmentごとに合計する。これは`a^2+b^2-2ab`の3項なので、
3回の畳み込みで全alignmentを同時に判定できる。

圧縮alphabetとpattern長から全不一致量が`998244353`未満と分かる場合は単一modのNTTを使う。
それ以外は三法CRTによる整数畳み込みへ切り替える。
計算量`O((n+m) log(n+m))`、領域`O(n+m)`。

空patternは`0..text.size()`の全境界に一致し、patternの方が長い場合は空を返す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `wildcard_matches`

```cpp
inline std::vector<int> wildcard_matches( const std::string& text, const std::string& pattern, char wildcard = '_' )
```

O((n+m) log(n+m))。wildcard_matches(text,pattern,wildcard)で一致する開始位置を返す。
textとpatternのどちらでもwildcardは任意の一文字に一致する。空patternは全境界に一致する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC307 Ex - Marquee](https://atcoder.jp/contests/abc307/tasks/abc307_h)では、
一周期と先頭`W-1`文字を連結した表示文字列に対して全状態を照合する。
`verify/atcoder_abc307_h.cpp`で公式sample 4件を確認済み。judge ACは未確認。
