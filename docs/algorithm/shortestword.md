---
title: "暗黙automatonの最短辞書順語"
documentation_of: //cp/algorithm/shortestword.hpp
---

# 暗黙automatonの最短辞書順語

- Header: `cp/algorithm/shortestword.hpp`
- Symbol: `poe::shortest_lexicographic_word`
- Status: experimental

## どんな問題に使えるか

有限状態automatonを明示的に辺配列へせず、受理される最短語をBFSで探して親から復元する。
同じ長さでは辞書順最小を返す。剰余automaton、禁止pattern、数字だけからなる最小倍数に使える。

```cpp
#include "algorithm/shortestword.hpp"

auto word = shortest_lexicographic_word<char>(states, start, accept,
    [&](int state, auto relax) {
        for (char symbol : alphabet) relax(next(state, symbol), symbol);
    });
```

`expand`はsymbolの辞書順に`relax(next,symbol)`を呼ぶ。`relax`は新規状態ならtrue、訪問済みならfalseを返すため、
遷移に単調な支配関係がある場合は途中で打ち切れる。調べた辺数を`E`として`O(V+E)`時間、`O(V)`領域。
受理語がなければ`nullopt`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `shortest_lexicographic_word`

```cpp
template <class Symbol = char, class Accept, class Expand> std::optional<std::vector<Symbol>> shortest_lexicographic_word( int state_count, int start, Accept accept, Expand expand )
```

O(V+E)。暗黙automatonで最短、同長なら辞書順最小の受理語を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC443 F - Non-Increasing Number](https://atcoder.jp/contests/abc443/tasks/abc443_f)では、
`{Nでの剰余, 最後の桁}`を状態にし、後ろへ同じか大きい桁を付ける。
`verify/atcoder_abc443_f.cpp`で公式sampleを確認済み。
