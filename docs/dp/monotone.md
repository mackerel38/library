---
title: "Monotone minima"
documentation_of: //cp/dp/monotone.hpp
---

# Monotone minima

- Header: `cp/dp/monotone.hpp`
- Symbol: `poe::monotone_minima`
- Status: experimental

## どんな問題に使えるか

行列の各行の最小要素位置が行に対して広義単調増加するとき、全要素を調べず各行の最小値と列を求める。
Monge DP、凸費用の遷移、max/min-plus convolutionなどの内部部品として使える。

```cpp
#include "dp/monotone.hpp"

auto minimum = poe::monotone_minima<long long>(rows, columns, [&](int row, int column) {
    return previous[column] + cost(column, row);
});
```

同値なら最小列を選ぶ。最適列の単調性を証明できない場合は使えない。
`O(columns log rows + rows)`回の評価、`O(rows)`領域。

## 使う前の確認

- 全行・全列の組について`value(row, column)`を評価できること。
- 各行の最小列が広義単調増加し、同値なら最小列を選んだ場合も単調であること。
- 総計算量は上記の評価回数に一回の`value`評価時間を掛けたものになる。
- 最大値が必要なら最小化へ変換し、符号反転でoverflowしないこと。

これは層DPではなく行最小値を求める部品である。前のDP列を更新しながら複数層を計算するなら
`divide_conquer_dp`を検討する。SMAWKは現在未実装である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `monotone_minima`

```cpp
template<class T, class Value> std::vector<std::pair<T, int>> monotone_minima(int rows, int columns, Value value)
```

O(columns log rows + rows)。monotone_minima<T>(rows, columns, value): 各行の最小値と列を返す。
最小列が行に対して広義単調増加であること。同値なら最小の列を選ぶ。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

ナップサックの重さ別max-plus convolutionなど、多数の問題の部分遷移に現れる。
単独でAPI全体を検証する適切な公開問題は未選定。現在はランダムなMonge行列を全探索と比較している。
