---
title: "隣接差・傾き制約"
documentation_of: //cp/algorithm/slopeconstraint.hpp
---

# 隣接差・傾き制約


## 概要

各位置の上限`upper[i]`を超えず、隣接差の絶対値が`slope`以下となる列のうち、成分ごとに最大の列を返す。
高さ制限の伝播、傾斜制約付き地形、距離変換`min_j(upper[j]+slope*abs(i-j))`に使える。
離散Lipschitz下包絡とも呼ばれる処理である。

## 厳密な定義

長さ$n$の列$U$と非負の値$s$に対し、すべての$i$で$H_i\le U_i$、すべての
$0\le i+1<n$で$|H_{i+1}-H_i|\le s$を満たす列$H$のうち、各成分が最大のものを返す。
同値に、$H_i=\min_j(U_j+s|i-j|)$である。

## Include

```cpp
#include "algorithm/slopeconstraint.hpp"

auto height = poe::slope_minorant(upper, 1LL);
```

左右から一回ずつ制約を伝播するため、`O(n)`時間、返り値を除き`O(1)`追加領域。
整数型で`upper[i]+slope`がoverflowしない範囲を使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `slope_minorant`

```cpp
template <class T> std::vector<T> slope_minorant(std::vector<T> upper, T slope)
```

O(n)。upper以下で隣接差の絶対値がslope以下となる最大の列を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC443 D - Pawn Line](https://atcoder.jp/contests/abc443/tasks/abc443_d)では、
元の行番号を上限、傾きを1として最適な移動後の行を求める。
`verify/atcoder_abc443_d.cpp`で公式sampleを確認済み。
