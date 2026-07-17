---
title: "多項式アルゴリズム"
documentation_of: //cp/fps/polynomial.hpp
---

# 多項式アルゴリズム


## 概要

多数の多項式の積、商と余り、多数の点での評価、指定点からの補間、変数の平行移動を扱う。
単一のFPSに対する`inv`や`exp`とは分け、有限次数の多項式として末尾0を無視する操作をまとめている。

## 厳密な定義

- `polynomial_product`: O(M(S) log k)。polynomial_product(polynomials, limit): 多項式列の積を返す。 limit>=0なら各段階でx^limit未満へ切り詰める。空積は1。
- `polynomial_matrix_product`: O(d^3 M(S) log k)。polynomial_matrix_product(matrices,limit): 行列列を先頭から掛ける。 各要素は多項式で、limit>=0なら各段階でx^limit未満へ切り詰める。行列列は空でないこと。
- `polynomial_divmod`: O(M(n) log n)。polynomial_divmod(f,g): 多項式除算の商と余りを返す。g!=0。
- `multipoint_evaluation`: O(M(n) log^2 n)。multipoint_evaluation(f, points): 全pointsでfを評価する。
- `polynomial_interpolation`: O(M(n) log^2 n)。polynomial_interpolation(points, values): 指定点を通る次数n未満の多項式を返す。 pointsは相異なること。
- `taylor_shift`: O(M(n))。taylor_shift(f,c): f(x+c)を返す。f.size()<mod。

## Include

```cpp
#include "fps/polynomial.hpp"

using fps = poe::fps998244353;
fps product = poe::polynomial_product<998244353>(factors, limit);
auto matrix_product = poe::polynomial_matrix_product<998244353>(matrices, limit);
auto values = poe::multipoint_evaluation(product, points);
fps restored = poe::polynomial_interpolation<998244353>(points, values);
```

## 前提と計算量

- 多項式列の積は総次数`S`、個数`k`として`O(M(S) log k)`。`limit`で必要項だけ残せる。
- `d`次正方多項式行列列の積は`O(d^3 M(S) log k)`。列の先頭から順に掛け、空列は許さない。
- 除算は除数が0でないこと。`O(M(n) log n)`。
- 多点評価と補間は`O(M(n) log^2 n)`。補間点は相異なること。
- Taylor shiftは`f.size()<mod`で`O(M(n))`。

空多項式と定数0を区別しにくい操作では`shrink()`後の空列を0として扱う。
多点評価が一回だけで点数が少ない場合は、`eval`を繰り返す方が軽い。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `polynomial_product`

```cpp
template<int mod> fps<mod> polynomial_product(std::vector<fps<mod>> polynomials, int limit = -1)
```

O(M(S) log k)。polynomial_product(polynomials, limit): 多項式列の積を返す。
limit>=0なら各段階でx^limit未満へ切り詰める。空積は1。

### `polynomial_matrix_product`

```cpp
template<int mod> matrix<fps<mod>> polynomial_matrix_product( std::vector<matrix<fps<mod>>> matrices, int limit = -1 )
```

O(d^3 M(S) log k)。polynomial_matrix_product(matrices,limit): 行列列を先頭から掛ける。
各要素は多項式で、limit>=0なら各段階でx^limit未満へ切り詰める。行列列は空でないこと。

### `polynomial_divmod`

```cpp
template<int mod> std::pair<fps<mod>, fps<mod>> polynomial_divmod(fps<mod> dividend, fps<mod> divisor)
```

O(M(n) log n)。polynomial_divmod(f,g): 多項式除算の商と余りを返す。g!=0。

### `multipoint_evaluation`

```cpp
template<int mod> std::vector<staticmodint<mod>> multipoint_evaluation( const fps<mod>& polynomial, const std::vector<staticmodint<mod>>& points )
```

O(M(n) log^2 n)。multipoint_evaluation(f, points): 全pointsでfを評価する。

### `polynomial_interpolation`

```cpp
template<int mod> fps<mod> polynomial_interpolation( const std::vector<staticmodint<mod>>& points, const std::vector<staticmodint<mod>>& values )
```

O(M(n) log^2 n)。polynomial_interpolation(points, values): 指定点を通る次数n未満の多項式を返す。
pointsは相異なること。

### `taylor_shift`

```cpp
template<int mod> fps<mod> taylor_shift(const fps<mod>& polynomial, staticmodint<mod> shift)
```

O(M(n))。taylor_shift(f,c): f(x+c)を返す。f.size()<mod。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [FPS 24題 I - スコア](https://atcoder.jp/contests/fps-24/tasks/fps_24_i): 多項式列の積。
- [FPS 24題 P - ボール](https://atcoder.jp/contests/fps-24/tasks/fps_24_p): 多点評価。
- [AtCoder ABC462 G - Completely Wrong](https://atcoder.jp/contests/abc462/tasks/abc462_g):
  色ごとの包除多項式をまとめて積む。
- [AtCoder ABC317 Ex - Walk](https://atcoder.jp/contests/abc317/tasks/abc317_h):
  近傍へだけ進むwalkの母関数漸化式を3次多項式行列積でまとめる。
- Library CheckerのMultipoint Evaluation / Polynomial Interpolationにも対応する。

ABC462 GとABC317 Exは公式sampleを確認済み。
ランダムな小多項式について愚直評価・Lagrange補間とも照合する。
