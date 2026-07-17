---
title: "追加共有列の辞書順"
documentation_of: //cp/string/persistentsequence.hpp
---

# 追加共有列の辞書順

## 概要

永続追加列の辞書順と連結DAGの巨大列添字アクセスを扱う。

## 厳密な定義

- `persistentsequenceorder`: 既存列への一要素追加で作る列群を辞書順化する: persistentsequenceorder<int> sequences; O(log sigma)/追加。
- `persistentconcatenation`: 共有列の連結DAG: persistentconcatenation<char> seq({'0','1'}, limit); add後も旧versionを保持する。



既存versionの末尾へ一要素を追加して作る多数の列を、列を実体化せず共有trieへ格納し辞書順に並べる。
同じ列を表すversionは番号順になる。途中要素の変更や削除には使えない。

## Include

```cpp
#include "string/persistentsequence.hpp"
poe::persistentsequenceorder<int> sequences;
int version = sequences.add(parent_version, value);
auto order = sequences.order();
```

追加は`O(log sigma)`、全versionの整列済み列挙は`O(V+nodes)`、領域`O(V+nodes)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `persistentsequenceorder`

```cpp
template <class Symbol> struct persistentsequenceorder
```

既存列への一要素追加で作る列群を辞書順化する: persistentsequenceorder<int> sequences; O(log sigma)/追加。

### `persistentsequenceorder`

```cpp
persistentsequenceorder() : nodes_(1), version_node_(1, 0)
```

O(1)。空列version 0だけを持つ。

### `add`

```cpp
int add(int version, const Symbol& symbol)
```

O(log sigma)。versionの末尾にsymbolを加えた新version番号を返す。

### `order`

```cpp
std::vector<int> order() const
```

O(total versions + trie nodes)。空列を除くversion番号を列の辞書順・同値なら番号順で返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。空列を含むversion数を返す。

### `node`

```cpp
int node(int version) const
```

O(1)。versionに対応する共有trie頂点を返す。

### `persistentconcatenation`

```cpp
template<class Symbol> struct persistentconcatenation
```

共有列の連結DAG: persistentconcatenation<char> seq({'0','1'}, limit); add後も旧versionを保持する。

### `persistentconcatenation`

```cpp
explicit persistentconcatenation( const std::vector<Symbol>& initial, unsigned long long limit = std::numeric_limits<unsigned long long>::max() ) : limit_(limit)
```

O(initial)。各要素を長さ1の初期versionとして作る。参照する最大長limitを指定する。

### `add`

```cpp
int add(int left, int right)
```

O(log V)。leftとrightを連結し先頭limit要素だけを表す新version番号を返す。

### `get`

```cpp
const Symbol& get(int version, unsigned long long index) const
```

O(log V log limit)。versionの0-indexed位置indexにある要素を返す。

### `length`

```cpp
unsigned long long length(int version) const
```

O(1)。versionの上限切り詰め後の長さを返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。現在のversion数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC437 E - Sort Arrays](https://atcoder.jp/contests/abc437/tasks/abc437_e)へ直接使える。
`verify/atcoder_abc437_e.cpp`は公式サンプル一致。

## 永続連結列

`persistentconcatenation<Symbol>`は初期の一要素列と、既存version同士を連結して作る列DAGを保持する。
長さは構築時の`limit`で飽和させ、それ以降の要素を保存しない。巨大な文字列・配列を実体化せず、
任意versionの`get(version, index)`で0-indexed添え字アクセスできる。

```cpp
poe::persistentconcatenation<char> sequences({'0', '1'}, 1'000'000'000'000'000'000ULL);
int version = sequences.add(left_version, right_version);
char value = sequences.get(version, index);
```

version数を`V`として、`add`は`O(log V)`時間、`get`は`O(log V log limit)`時間。
binary lifting表の領域は`O(V log V)`。連結後のsuffixや`limit`より後ろを参照する用途には使えない。

[AtCoder ABC417 G - Binary Cat](https://atcoder.jp/contests/abc417/tasks/abc417_g)では、
初期versionを`0`,`1`として各queryを`add`し、指定位置を`get`する。
`verify/atcoder_abc417_g.cpp`で公式sampleを確認済み。
