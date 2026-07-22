---
title: "禁止patternの回避・破壊"
documentation_of: //cp/string/avoid.hpp
---

# 禁止patternの回避・破壊

## 概要

複数の禁止patternについて、既存text内の全出現を壊す最小文字位置集合を求めます。
また、指定alphabetから作る固定長の語のうち、禁止patternを一度も含まない語を数えます。

## 厳密な定義

text中でpatternが現れる半開区間全体を$\mathcal I$とします。
`minimum_pattern_replacements(text, patterns)`は、すべての$[l,r)\in\mathcal I$について

$$
P\cap[l,r)\neq\varnothing
$$

を満たす位置集合$P\subseteq\{0,1,\ldots,|text|-1\}$のうち、要素数が最小のものを返します。
返した各位置をpatternに使われない文字へ置換すれば、全出現が壊れます。

`count_avoiding_words<T>(length, patterns, first, last)`は、alphabet
$\{first,first+1,\ldots,last\}$からなる長さ`length`の語で、どのpatternも連続部分列として含まないものの個数を`T`で返します。

## Include

```cpp
#include "string/avoid.hpp"
```

## 最短の使用例

```cpp
auto positions = minimum_pattern_replacements(text, patterns);
cout << positions.size() << '\n';

mint count = count_avoiding_words<mint>(length, patterns, 'a', 'z');
```

## APIの選び方

| 欲しいもの | API | 出力 |
| --- | --- | --- |
| 既存textの全禁止pattern出現を壊す | `minimum_pattern_replacements` | 置換する0-indexed位置列 |
| 禁止patternを含まない固定長の語数 | `count_avoiding_words` | 型`T`の個数 |

## `minimum_pattern_replacements`

```cpp
template <std::ranges::input_range Text, std::ranges::input_range Patterns>
std::vector<int> minimum_pattern_replacements(
    const Text& text,
    const Patterns& patterns
);
```

- `text`: 調べる文字列またはsymbol列です。
- `patterns`: 空でないpatternの列です。列自体は空でも構いません。
- 戻り値: 0-indexed位置を狭義昇順に並べた最小解です。複数解がある場合は、出現区間の右端を左から選ぶgreedy解です。
- 計算量: $L=|text|$、$M=\sum|pattern|$、Aho–Corasick状態数を$q$、文字種数を$\sigma$として
  $O((L+M)\log\sigma+q\sigma)$時間、$O(q\sigma)$領域です。

各文字を読んだ状態で終わる最短pattern長を$O(1)$で取得するため、出現数が多くても全一致を列挙しません。
一致した時点でその右端を選び、automatonを根へ戻します。これは区間を右端順に刺す最小点集合greedyと同じです。

## `count_avoiding_words`

```cpp
template <class T, std::ranges::input_range Patterns, std::integral Symbol>
T count_avoiding_words(
    long long length,
    const Patterns& patterns,
    Symbol first,
    Symbol last
);
```

- `length`: 作る語の長さです。0以上です。
- `patterns`: 空でない禁止patternの列です。
- `[first, last]`: 閉区間で指定するalphabetです。
- `T`: 加算、乗算、`T{1}`を使える個数型です。通常は`modint`を使います。
- 計算量: 状態数を$q$として$O(q^3\log length+q\sigma\log\sigma)$時間、$O(q^2)$領域です。

`length=0`では空語を1通りと数えます。状態数の3乗が重いため、禁止patternの総長が大きい問題には向きません。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `minimum_pattern_replacements`

```cpp
template <std::ranges::input_range Text, std::ranges::input_range Patterns> std::vector<int> minimum_pattern_replacements( const Text& text, const Patterns& patterns )
```

O((|text|+sum|pattern|) log sigma+states sigma)。全pattern出現を壊す最小文字位置集合を返す。

### `count_avoiding_words`

```cpp
template <class T, std::ranges::input_range Patterns, std::integral Symbol> T count_avoiding_words(long long length, const Patterns& patterns, Symbol first, Symbol last)
```

禁止patternを含まない長さlengthの語数を数える。O(states^3 log length + states sigma log sigma)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC268 Ex - Taboo](https://atcoder.jp/contests/abc268/tasks/abc268_h): `minimum_pattern_replacements`で置換回数と位置を求められます。
- [ABC458 F - Critical Misread](https://atcoder.jp/contests/abc458/tasks/abc458_f): `count_avoiding_words`へ英小文字alphabetとpatternを渡せます。

## 検証

最小置換位置は長さ12以下のランダムtextについて全位置subsetを列挙し、全pattern出現を直接調べた最小値と5,000件比較しています。
回避語数は二文字alphabetの全語列挙と比較しています。ABC268 Exの公式3サンプル、ABC458 Fの公式サンプルもverifyコードで確認しています。
