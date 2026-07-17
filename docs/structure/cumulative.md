---
title: "多次元累積変換"
documentation_of: //cp/structure/cumulative.hpp
---

# 多次元累積変換

## Include

```cpp
#include "structure/cumulative.hpp"
```

`multidimensional_cumulative(shape, values, operation, direction)`は、row-major順に平坦化した
固定多次元配列をproduct order上で累積する。

- `cumulativedirection::prefix`: 座標`x`以下の全座標を集約する。
- `cumulativedirection::suffix`: 座標`x`以上の全座標を集約する。

全要素数を`N`、次元数を`d`とすると計算量`O(dN)`、追加領域`O(d)`。
`operation`には結合的な二項演算を渡す。最大・最小・gcd・bit OR・加算などに使える。
元配列を値渡しして変換後の一次元配列を返すため、必要なら`std::move(values)`を渡す。

これは「原点からの累積値を全点について作る」変換であり、任意直方体queryのAPIではない。
加算で一般直方体和が必要なら`structure/prefixsum.hpp`の`multidimensionalprefixsum`を使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `cumulativedirection`

```cpp
enum class cumulativedirection
```

多次元累積変換の向き。prefixは各座標以下、suffixは各座標以上を集約する。

### `multidimensional_cumulative`

```cpp
template<class T, class Operation> std::vector<T> multidimensional_cumulative( const std::vector<int>& shape, std::vector<T> values, Operation operation, cumulativedirection direction = cumulativedirection::prefix )
```

O(dN)。row-major多次元配列を各座標以下または以上のoperation累積へ変換する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder Beginner Contest 464 E - Fill-Rect Query](https://atcoder.jp/contests/abc464/tasks/abc464_e)。
各query番号を右下端点へ置き、suffix maximumを取ると、各マスを最後に上書きしたquery番号になる。

```cpp
auto latest = multidimensional_cumulative(
    vector{height, width}, values,
    [](int a, int b) { return max(a, b); },
    cumulativedirection::suffix
);
```

`verify/atcoder_abc464_e.cpp`に提出用コードを用意し、公式サンプル1との一致を確認済み。
judgeへの提出確認は未実施。
