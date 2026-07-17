---
title: "右向き移動による単調化"
documentation_of: //cp/algorithm/isotonic.hpp
---

# 右向き移動による単調化

- Header: `cp/algorithm/isotonic.hpp`
- Symbols: `poe::righttransferisotonicresult`, `poe::right_transfer_isotonic`
- Status: experimental

## どんな問題に使えるか

整数列に対して「位置 `i` から `i+1` へ値を1だけ移す」操作を繰り返し、
広義単調増加列にする最小操作回数と最終列を求める。
隣接する違反区間を均等にならす処理をblockとして併合するため、要素を一単位ずつ動かさない。

```cpp
#include "algorithm/isotonic.hpp"

auto result = poe::right_transfer_isotonic(values);
cout << result.operations << '\n';
```

計算量は`O(n)`、領域は`O(n)`。総和は保存される。値が負になることを許す。
左向き移動、要素ごとの移動費用、上下限制約にはそのまま使えない。

狭義単調増加にしたいときは、0-indexedの各要素から添字を引いてから呼び出す。
返された列へ添字を足せば、狭義単調増加の最終列になる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `righttransferisotonicresult`

```cpp
struct righttransferisotonicresult
```

right_transfer_isotonicの結果。valuesは操作後の広義単調増加列、operationsは最小操作回数。

### `right_transfer_isotonic`

```cpp
inline righttransferisotonicresult right_transfer_isotonic(const std::vector<long long>& values)
```

O(n)。各操作でvalues[i]を1減らしvalues[i+1]を1増やして広義単調増加にする最小操作を求める。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC459 F - -1, +1](https://atcoder.jp/contests/abc459/tasks/abc459_f)は、
`A[i] -= i`の後にこの関数を一度呼ぶだけで最小操作回数を得られる。
`verify/atcoder_abc459_f.cpp`で公式sample一致を確認している。judge ACは未確認。
