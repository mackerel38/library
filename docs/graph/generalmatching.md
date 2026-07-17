---
title: "一般グラフの完全マッチング"
documentation_of: //cp/graph/generalmatching.hpp
---

# 一般グラフの完全マッチング


## 概要

二部とは限らない無向グラフで、全頂点を互いに素な辺で覆う完全マッチングのうち、
重み0または1の辺の合計が最小のものを求める。

## 厳密な定義

- `binarymatchingedge`: minimum_binary_perfect_matching_weightへ渡す重み0または1の無向辺。
- `minimum_binary_perfect_matching_weight`: O(n^4)。重み0/1の一般無向グラフで完全matchingの最小重みを高確率で返す。存在しなければnullopt。

## Include

```cpp
#include "graph/generalmatching.hpp"

vector<binarymatchingedge> edges{{u, v, weight}};
optional<int> answer =
    minimum_binary_perfect_matching_weight(vertex_count, edges);
```

完全マッチングが存在しなければ`nullopt`を返す。辺の選び方そのものは復元しない。
Tutte行列の各辺変数を有限体上の乱数へ置き換え、重み1の辺へ変数`y`を掛ける。
そのPfaffian多項式の最小非零次数が答えになる。行列式を使う方法より多項式次数が半分になり、
必要な評価点も減る。

乱択アルゴリズムであり、理論上は完全マッチングを見逃す小さい確率がある。
固定seedと法`998244353`を使うため実行結果は再現可能。

## 計算量

頂点数を`n`として`O(n^4)`時間、`O(n^2)`領域。
重みが一般整数の場合やmatchingの復元が必要な場合は、重み付きEdmonds blossomなど別の実装が必要。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `binarymatchingedge`

```cpp
struct binarymatchingedge
```

minimum_binary_perfect_matching_weightへ渡す重み0または1の無向辺。

### `minimum_binary_perfect_matching_weight`

```cpp
inline std::optional<int> minimum_binary_perfect_matching_weight( int vertex_count, const std::vector<binarymatchingedge>& edges)
```

O(n^4)。重み0/1の一般無向グラフで完全matchingの最小重みを高確率で返す。存在しなければnullopt。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC412 G - Degree Harmony](https://atcoder.jp/contests/abc412/tasks/abc412_g)では、
頂点`i`を`A[i]`個に複製し、同じ元頂点間を重み0、元グラフの辺に対応する複製間を重み1で結ぶ。
`verify/atcoder_abc412_g.cpp`で公式sampleを確認済み。

## 検証

- `tests/api/graph/generalmatching.cpp`: 存在・不存在・空グラフ
- `tests/property/graph/generalmatching.cpp`: 12頂点以下の部分集合DPとの乱択比較
