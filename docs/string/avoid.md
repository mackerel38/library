---
title: "禁止patternを含まない語の数え上げ"
documentation_of: //cp/string/avoid.hpp
---

# 禁止patternを含まない語の数え上げ

- Header: `cp/string/avoid.hpp`
- Symbol: `poe::count_avoiding_words<T>`
- Status: experimental

## どんな問題に使えるか

指定したalphabetから作る長さ`length`の語のうち、複数の禁止patternを
連続部分列として一度も含まないものを数える。Aho--Corasickの状態DPと行列累乗を内部で行う。

```cpp
#include "string/avoid.hpp"

mint answer = poe::count_avoiding_words<mint>(length, patterns, 'a', 'z');
```

状態数を禁止patternのtrie頂点数`states`、文字種数を`sigma`とすると、
計算量は`O(states^3 log length + states sigma log sigma)`、領域は`O(states^2)`。
`length`は0でもよく、その場合は空語を1通りと数える。禁止patternは空でないこと。

状態数の3乗が重いため、禁止patternの総長が大きい問題には向かない。
長さが小さい場合は行列累乗を使わないAho--Corasick上の通常DPの方が速い。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_avoiding_words`

```cpp
template <class T, std::ranges::input_range Patterns, std::integral Symbol> T count_avoiding_words(long long length, const Patterns& patterns, Symbol first, Symbol last)
```

禁止patternを含まない長さlengthの語数を数える。O(states^3 log length + states sigma log sigma)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC458 F - Critical Misread](https://atcoder.jp/contests/abc458/tasks/abc458_f)では、
英小文字alphabetと入力patternをそのまま渡せる。
`verify/atcoder_abc458_f.cpp`で公式sample一致を確認している。judge ACは未確認。
