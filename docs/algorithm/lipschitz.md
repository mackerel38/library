---
title: "離散Lipschitz下包絡"
documentation_of: //cp/algorithm/lipschitz.hpp
---

# 離散Lipschitz下包絡

- Header: `cp/algorithm/lipschitz.hpp`
- Symbol: `poe::lipschitz_minorant`
- Status: experimental

## どんな問題に使えるか

各位置の上限`upper[i]`を超えず、隣接差の絶対値が`slope`以下となる列のうち、成分ごとに最大の列を返す。
高さ制限の伝播、傾斜制約付き地形、距離変換`min_j(upper[j]+slope*abs(i-j))`に使える。

```cpp
#include "algorithm/lipschitz.hpp"

auto height = lipschitz_minorant(upper, 1LL);
```

左右から一回ずつ制約を伝播するため、`O(n)`時間、返り値を除き`O(1)`追加領域。
整数型で`upper[i]+slope`がoverflowしない範囲を使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lipschitz_minorant`

```cpp
template <class T> std::vector<T> lipschitz_minorant(std::vector<T> upper, T slope)
```

O(n)。upper以下で隣接差の絶対値がslope以下となる最大の列を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC443 D - Pawn Line](https://atcoder.jp/contests/abc443/tasks/abc443_d)では、
元の行番号を上限、傾きを1として最適な移動後の行を求める。
`verify/atcoder_abc443_d.cpp`で公式sampleを確認済み。judge ACは未確認。
