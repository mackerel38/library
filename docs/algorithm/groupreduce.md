---
title: "隣接群の縮約"
documentation_of: //cp/algorithm/groupreduce.hpp
---

# 隣接群の縮約

- Header: `cp/algorithm/groupreduce.hpp`
- Symbol: `poe::reduce_adjacent_groups`
- Status: experimental

## どんな問題に使えるか

列中の同じ値が隣接して`k`個揃うたびに消える操作を、消去後に新しく接した要素も含めて処理する。
返り値は縮約後の列なので、長さだけでなく残った要素も利用できる。入力順を保てない消去規則や、値ごとに
消去個数が異なる規則には使えない。

## API・計算量

```cpp
#include "algorithm/groupreduce.hpp"

auto reduced = poe::reduce_adjacent_groups(values, group_size);
```

`O(n)`時間・`O(n)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `reduce_adjacent_groups`

```cpp
template <class T, class Equal = std::equal_to<T>> std::vector<T> reduce_adjacent_groups(const std::vector<T>& values, int group_size, Equal equal = Equal
```

隣接する同値要素がgroup_size個揃うたび消す: reduce_adjacent_groups(values, k); O(n)。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC438 C - 1D puyopuyo](https://atcoder.jp/contests/abc438/tasks/abc438_c)では、
`group_size=4`で縮約し、返った列の長さを出力する。
`verify/atcoder_abc438_c.cpp`は公式サンプル一致を確認済み。
