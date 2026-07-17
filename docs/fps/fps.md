---
title: "形式的冪級数"
documentation_of: //cp/fps/fps.hpp
---

# 形式的冪級数

- Header: `cp/fps/fps.hpp`
- Symbol: `poe::fps<mod>`, `poe::fps998244353`
- Status: experimental

## 設計

係数列を`vector`と同じ添字・iteratorで扱い、FPS固有の打ち切り演算だけをメソッドとして追加する。
`f.inv(n)`の`n`は「次数」ではなく返す項数で、常に`x^n`未満を返す。省略時は`f.size()`を使う。
範囲外係数を0として読む場合は`coeff(i)`、通常の配列アクセスには`operator[]`を使う。

```cpp
#include "fps/fps.hpp"

using fps = poe::fps998244353;
fps f = {1, 2, 3};
fps inverse = f.inv(10);
fps power = f.pow(100, 10);
```

## 主な操作

| 操作 | 前提 | 計算量 |
| --- | --- | --- |
| `+`, `-`, scalar積 | なし | `O(n)` |
| `*` | 法がNTT可能、またはconvolutionのCRT条件内 | `O(M(n))` |
| `diff`, `integral` | 積分は項数が法未満 | `O(n)` |
| `inv(n)` | 定数項が非0 | `O(M(n) log n)` |
| `log(n)` | 定数項が1 | `O(M(n) log n)` |
| `exp(n)` | 定数項が0 | `O(M(n) log n)` |
| `pow(k,n)` | `k>=0` | `O(M(n) log n)` |
| `eval(x)` | なし | `O(n)` |

`M(n)`は長さ`n`程度の畳み込み時間。現在の実装は各Newton段階で既存convolutionを呼ぶため、
厳密には上表の`log n`因子を持つ。専用NTT最適化より少し遅いが、前提と実装を共有しやすい構成である。

## よくある誤り

- `inv`は定数項0の級数には使えない。
- `log`の定数項は1、`exp`の定数項は0でなければならない。
- `pow(0,n)`は入力に関係なく`1 mod x^n`を返す。
- 係数列の末尾0は自動削除しない。多項式次数が必要なら`shrink()`を呼ぶ。
- `operator*`は全積を返す。必要項数が小さい場合は直後に`prefix(n)`で切る。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `fps`

```cpp
template<int mod> struct fps : std::vector<staticmodint<mod>>
```

fps<mod> f(n): mod上の形式的冪級数をvectorと同じ添字で持つ。
f.inv(n), f.log(n), f.exp(n), f.pow(k,n)は先頭n項を返す。

### `size_int`

```cpp
int size_int() const noexcept
```

O(1)。f.size_int(): 項数をintで返す。

### `coeff`

```cpp
mint coeff(int index) const
```

O(1)。f.coeff(i): 範囲外を0としてx^iの係数を返す。

### `prefix`

```cpp
fps prefix(int size) const
```

O(n)。f.prefix(n): 0埋めまたは切り詰めで先頭n項を返す。

### `shrink`

```cpp
fps& shrink()
```

O(n)。f.shrink(): 末尾の0を削除する。

### `operator+=`

```cpp
fps& operator+=(const fps& other)
```

O(n)。f += g: 係数ごとに加算し、長い方の項数に合わせる。

### `operator-=`

```cpp
fps& operator-=(const fps& other)
```

O(n)。f -= g: 係数ごとに減算し、長い方の項数に合わせる。

### `operator*=`

```cpp
fps& operator*=(const fps& other)
```

O(M(n))。f *= g: 畳み込みで多項式積を代入する。

### `operator*=`

```cpp
fps& operator*=(mint scalar)
```

O(n)。f *= c: 全係数をc倍する。

### `operator/=`

```cpp
fps& operator/=(mint scalar)
```

O(n log mod)。f /= c: 全係数をcで割る。c!=0。

### `operator-`

```cpp
fps operator-() const
```

O(n)。-f: 全係数の符号を反転する。

### `diff`

```cpp
fps diff() const
```

O(n)。f.diff(): 形式微分を返す。

### `integral`

```cpp
fps integral() const
```

O(n log mod)。f.integral(): 定数項0の形式積分を返す。n<mod。

### `inv`

```cpp
fps inv(int size = -1) const
```

O(M(n) log n)。f.inv(n): 1/f mod x^nを返す。f[0]!=0。

### `div`

```cpp
fps div(const fps& denominator, int size = -1) const
```

O(M(n) log n)。f.div(g,n): f/g mod x^nを返す。g[0]!=0。

### `log`

```cpp
fps log(int size = -1) const
```

O(M(n) log n)。f.log(n): log(f) mod x^nを返す。f[0]=1、n<mod。

### `exp`

```cpp
fps exp(int size = -1) const
```

O(M(n) log n)。f.exp(n): exp(f) mod x^nを返す。f[0]=0、n<mod。

### `pow`

```cpp
fps pow(long long exponent, int size = -1) const
```

O(M(n) log n)。f.pow(k,n): f^k mod x^nを返す。k>=0、n<mod。

### `eval`

```cpp
mint eval(mint x) const
```

O(n)。f.eval(x): 多項式としてxへ代入した値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

基礎演算はLibrary CheckerのInv / Log / Exp / Pow of Formal Power Seriesに対応するverifyコードを
追加する。FPS24題ではC、K、L、M、Nなどの下位部品になる。judge ACは未確認で、現在は愚直級数との
property testで確認する。

[AtCoder ABC318 Ex - Count Strong Test Cases](https://atcoder.jp/contests/abc318/tasks/abc318_h)では、
長さ`i`の独立なcycle成分の重み`1/i^2`を係数に持つFPSを`exp`し、集合としてのcycle分解を数える。
`verify/atcoder_abc318_h.cpp`で公式sample 4件を確認済み。judge ACは未確認。

## FPS 24題との対応

全24題にverifyコードを用意している。いずれもrepo内ライブラリを使用しており、judge ACは未確認。
FはFPS演算そのものを必要とせず、`math/modint.hpp`のverifyとして残している。

| 問題 | 主に使う部品 |
| --- | --- |
| A | `sparse_power` |
| B | `rationalfps::coeff` |
| C | `fps::pow` |
| D | `fps::pow`, `fps::inv` |
| E | `polynomial_product` |
| F | `modint` |
| G | `multiply_binomial`, `divide_binomial` |
| H | `binomial_transform` |
| I | `polynomial_product` |
| J | `online_convolution` |
| K | `fps::inv` |
| L | `fps::exp` |
| M | `fps::log` |
| N | `fps::exp` |
| O | `fps::pow` |
| P | `multipoint_evaluation` |
| Q | `power_sums`, `binomial_convolution` |
| R | `cyclic_power` |
| S | `newton_fps` |
| T | `rational_sum`, `rationalfps::coeff` |
| U | `holonomic_sequence` |
| V | `fps`の積 |
| W | `setfps_log`, `setfps_exp`, `setfps_articulation_transform` |
| X | `functional_power` |
