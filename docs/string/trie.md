---
title: "Trie / Aho--Corasick"
documentation_of: //cp/string/trie.hpp
---

# Trie / Aho--Corasick

- Status: experimental
- Header: `cp/string/trie.hpp`
- Symbol: `poe::trie<Symbol>`, `poe::ahocorasick<Symbol>`

## Include

```cpp
#include "string/trie.hpp"
```

## 選び方

- `trie`: wordの完全一致、prefixの存在・個数、共通prefixを扱う。
- `ahocorasick`: 多数のpatternを一つのtextから同時検索する。またはautomaton状態をDPへ載せる。

文字種は英小文字に固定していない。遷移は`std::map<Symbol, int>`で保持する。
小文字英字の全遷移が必要なら`ahocorasick<char> ac('a', 'z');`と書く。

## Trie

```cpp
trie<char> dictionary;
int id = dictionary.add(word);
int node = dictionary.find(prefix);
int exact = dictionary.count(word);
int prefixed = dictionary.prefix_count(prefix);
```

`add`は重複も一件として数え、word番号を返す。空wordも追加できる。
`next(node, symbol)`は辺がなければ`-1`、`operator[]`は`trienode`を返す。
nodeには`next`、`parent`、`depth`、`prefix_count`、`terminal_count`、終端word番号`words`がある。

各操作はword長を`length`、nodeの分岐数上限を`sigma`として`O(length log sigma)`。
領域は追加した文字数の合計に比例する。

## Aho--Corasick

```cpp
ahocorasick<char> automaton('a', 'z');
for (const string& pattern : patterns) automaton.add(pattern);
automaton.build();

int state = 0;
state = automaton.next(state, character);
unsigned mask = automaton.pattern_mask<unsigned>(state);
```

pattern追加後に`build()`が必要で、空patternは追加できない。
`next`はalphabet外の文字を読むと根へ戻る。

- `match(text)`: `{pattern, begin, end}`を終了位置順で列挙。
- `find_all(text)`: patternごとの出現開始位置。
- `count(text)`: patternごとの重なりを含む出現回数。全一致を列挙しない。
- `patterns(state)` / `pattern_mask<UInt>(state)`: その状態で終わるpattern集合。
- `failure(state)`: failure link。

状態数を`states`、alphabet幅を`sigma`とすると、`build`は
`O(states sigma log sigma)`、領域`O(states sigma)`。構築後の一遷移は`O(log sigma)`。
全文検索は`O(text log sigma + occurrences)`、`count`は
`O(text log sigma + states + patterns)`。

alphabetをpatternから自動収集するdefault constructorもある。ただしDPでpatternに現れない文字も
列挙する場合は、constructorへalphabet全体を渡す必要がある。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `trienode`

```cpp
template <class Symbol> struct trienode
```

trieの一頂点。next、親、深さ、通過word数、終端word数をO(1)で参照する。

### `trie`

```cpp
template <class Symbol = char> struct trie
```

任意文字種のprefix木: trie<char> dictionary; dictionary.add(word); 各操作O(length log sigma)。

### `trie`

```cpp
trie() : nodes_(1)
```

O(1)。根だけを持つ空のtrieを作る。

### `add`

```cpp
template <std::ranges::input_range Range> int add(const Range& word)
```

O(length log sigma)。wordを追加してword番号を返す。空列も追加できる。

### `find`

```cpp
template <std::ranges::input_range Range> int find(const Range& word) const
```

O(length log sigma)。wordに対応する頂点を返す。存在しなければ-1。

### `count`

```cpp
template <std::ranges::input_range Range> int count(const Range& word) const
```

O(length log sigma)。wordが追加された回数を返す。

### `prefix_count`

```cpp
template <std::ranges::input_range Range> int prefix_count(const Range& prefix) const
```

O(length log sigma)。prefixを接頭辞に持つ追加済みword数を返す。

### `next`

```cpp
int next(int node, const Symbol& symbol) const
```

O(log sigma)。nodeからsymbolで進む頂点。辺がなければ-1。

### `operator[]`

```cpp
const trienode<Symbol>& operator[](int node) const
```

O(1)。nodeの情報を返す。

### `terminal`

```cpp
int terminal(int id) const
```

O(1)。word番号idの終端頂点を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `word_count`

```cpp
int word_count() const noexcept
```

O(1)。重複を含む追加済みword数を返す。

### `ahomatch`

```cpp
struct ahomatch
```

Aho--Corasickの一一致。patternが半開区間[begin,end)へ現れる。

### `ahocorasick`

```cpp
template <class Symbol = char> struct ahocorasick
```

複数pattern automaton: ahocorasick<char> ac('a','z'); add後build; 遷移O(log sigma)。

### `ahocorasick`

```cpp
ahocorasick() : nodes_(1)
```

O(1)。pattern内に現れる文字をalphabetへ自動追加する空automatonを作る。

### `ahocorasick`

```cpp
explicit ahocorasick(std::vector<Symbol> alphabet) : alphabet_(std::move(alphabet)), nodes_(1)
```

O(sigma log sigma)。DP等で使うalphabetを指定する。

### `ahocorasick`

```cpp
ahocorasick(Symbol first, Symbol last) requires std::integral<Symbol> : nodes_(1)
```

O(sigma)。整数文字種の[first,last]をalphabetとして指定する。

### `add`

```cpp
template <std::ranges::input_range Range> int add(const Range& pattern)
```

O(length log sigma)。空でないpatternを追加して番号を返す。追加後はbuild()が必要。

### `build`

```cpp
void build()
```

O(states sigma log sigma)。failure linkと全alphabet遷移を構築する。

### `next`

```cpp
int next(int state, const Symbol& symbol) const
```

O(log sigma)。stateからsymbolを読んだ次状態を返す。build後に使う。

### `patterns`

```cpp
std::vector<int> patterns(int state) const
```

O(一致pattern数)。stateで終わるpattern番号を返す。

### `pattern_mask`

```cpp
template <std::unsigned_integral UInt = unsigned long long> UInt pattern_mask(int state) const
```

O(一致pattern数)。stateで終わるpattern番号をbit maskで返す。pattern数<=UIntのbit数。

### `match`

```cpp
template <std::ranges::input_range Range> std::vector<ahomatch> match(const Range& text) const
```

O(text log sigma + occurrences)。全一致を終了位置順で返す。

### `find_all`

```cpp
template <std::ranges::input_range Range> std::vector<std::vector<int>> find_all(const Range& text) const
```

O(text log sigma + occurrences)。patternごとの出現開始位置を昇順で返す。

### `count`

```cpp
template <std::ranges::input_range Range> std::vector<long long> count(const Range& text) const
```

O(text log sigma + states + patterns)。各patternの重なりを含む出現回数を返す。

### `failure`

```cpp
int failure(int state) const
```

O(1)。stateのfailure linkを返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。automatonの状態数を返す。

### `pattern_count`

```cpp
int pattern_count() const noexcept
```

O(1)。追加済みpattern数を返す。

### `length`

```cpp
int length(int id) const
```

O(1)。pattern番号idの長さを返す。

### `alphabet`

```cpp
const std::vector<Symbol>& alphabet() const noexcept
```

O(1)。使用するalphabetを返す。build後は昇順かつ重複なし。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC287 E - Karuta](https://atcoder.jp/contests/abc287/tasks/abc287_e):
  trieの各nodeを通るword数から、他のwordと共有する最長prefixを求める。
- [AtCoder ABC419 F - All Included](https://atcoder.jp/contests/abc419/tasks/abc419_f):
  Aho--Corasick状態と「既に含んだpattern集合」のbit maskをDP状態にする。

`verify/atcoder_abc287_e.cpp`と`verify/atcoder_abc419_f.cpp`を収録した。
ABC419 FでAho--Corasickを用いる方針は公式解説にも掲載されている。

## 検証

- `tests/api/string/trie.cpp`: 重複word、prefix数、failure、mask、出現位置
- `tests/property/string/trie.cpp`: ランダムpattern/textを愚直部分文字列検索と比較
- 上記2件のABC用verifyコード: コンパイル確認対象
