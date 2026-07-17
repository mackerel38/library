---
title: "累積和"
documentation_of: //cp/structure/prefixsum.hpp
---

# 累積和

<!-- API REFERENCE: COLLAPSED -->

## 概要

値を変更しない配列の区間和を何度も求めるときや、多数の区間加算を最後にまとめて反映するときに使う。
一次元、二次元、円環、多次元の各形式をこのheaderにまとめている。

## 厳密な定義

数列`A`の一次元累積和を

```text
S[0] = 0
S[k] = sum(A[i])  （0 <= i < k）
```

と定義する。このとき半開区間`[left, right)`の和は`S[right] - S[left]`である。

二次元累積和`S[bottom][right]`は、`0 <= row < bottom`かつ`0 <= column < right`を満たす
全セルの和である。長方形`[top, bottom) x [left, right)`の和は包除原理で求める。

差分配列は区間加算`[left, right)`へ`value`を加える操作を、差分の
`D[left] += value, D[right] -= value`として記録する。最後に`D`の累積和を取ると各要素が復元される。
二次元差分配列では、長方形の四隅へ符号付きで値を加え、縦横の累積和で復元する。

## Include

```cpp
#include "structure/prefixsum.hpp"
```

## 型の選び方

| やりたいこと | 型 |
| --- | --- |
| 多数の区間加算後に各要素を得る | `differencearray` |
| 多数の長方形加算後に各セルを得る | `differencearray2d` |
| 静的な区間和 | `prefixsum` |
| 区間内の全部分配列和 | `subarrayprefixsum` |
| 円環上の静的区間和 | `circularprefixsum` |
| 静的な長方形和 | `prefixsum2d` |
| 任意次元の静的直方体和 | `multidimensionalprefixsum` |

## 最短の使用例

```cpp
prefixsum<long long> sums(values);
long long answer = sums.sum(left, right);
```

```cpp
differencearray2d<long long> diff(initial);
diff.add(top, left, bottom, right, value);
diff.build();
long long result = diff(row, column);
```

## 基本的な使い方

`differencearray<T>`は多数の区間加算を`add(left, right, value)`で予約し、`build()`後に
`operator[]`または`values()`で完成列を取得する。`build()`後にさらに追加して再buildしてもよい。
`differencearray2d<T>`は同じ操作を半開長方形へ拡張し、`operator()(row,column)`で取得する。
どちらも要素数だけでなく、初期値の一次元・二次元`vector`から構築できる。

```cpp
differencearray<int> count(1000001);
count.add(left, right, 1);
count.build();
int covered = count[time];
```

```cpp
prefixsum<long long> ps({2, 5, -1, 4});
long long a = ps.sum(1, 4);  // 8

prefixsum2d<int> grid({{1, 2}, {3, 4}});
int b = grid.sum(0, 1, 2, 2);  // 6
```

`prefixsum<T>`は`sum(right)`、`sum(left, right)`、`sum()`と元要素の`operator[]`を持つ。
`prefixsum2d<T>`は`sum(bottom, right)`、`sum(top, left, bottom, right)`、`sum()`と
元要素の`operator()(row, column)`を持つ。

## 全部分配列の和

`subarrayprefixsum<T>`は、指定区間に完全に含まれる全非空部分配列について、その要素和をさらに
合計する静的queryを扱う。`sum(left,right)`は`values[left:right]`内の答えを返し、
`operator[]`で元要素にもアクセスできる。

```cpp
subarrayprefixsum<long long> sums({2, 1, 3, 3, 1});
long long answer = sums.sum(1, 4);  // 24
```

各要素の寄与回数`(index-left+1)(right-index)`を二次式へ展開し、
`A[i]`、`i*A[i]`、`i*i*A[i]`の三つの累積和で計算する。

## 円環列

`circularprefixsum<T>`は非空の静的列について、論理的な左回転と区間和をともに`O(1)`で扱う。

```cpp
circularprefixsum<long long> data(values);
data.rotate(left);
long long answer = data.sum(begin, end);
```

`rotate(left)`は正なら左回転、負なら右回転する。`sum`の添字は回転後の論理列に対する半開区間。
`operator[]`、`sum()`、現在の左回転量を返す`offset()`も持つ。構築は元列を二つ連結するため
`O(n)`時間・領域で、その後の操作はすべて`O(1)`。

## 多次元版

`multidimensionalprefixsum<T>(shape, values)`は、任意次元の固定格子をrow-major順の一次元配列で受け取る。
`sum(lower, upper)`で各次元の半開区間`[lower[d], upper[d])`の直積領域を取得する。
`operator[]`は平坦化添字、`operator()(coordinates)`は多次元座標で元要素を返す。

全要素数を`N`、次元数を`d`とすると構築`O(dN)`、領域`O(N)`、原点からの領域和は`O(d)`、
一般の直方体領域和は包除原理により`O(d 2^d)`。次元数が小さく、各軸の積を全列挙できる場合に使う。

## 計算量

| 型 | 構築 | 取得 | 領域 |
| --- | --- | --- | --- |
| `differencearray` | 区間加算`O(1)`、反映`O(n)` | `O(1)` | `O(n)` |
| `differencearray2d` | 長方形加算`O(1)`、反映`O(hw)` | `O(1)` | `O(hw)` |
| `prefixsum` | `O(n)` | `O(1)` | `O(n)` |
| `subarrayprefixsum` | `O(n)` | `O(1)` | `O(n)` |
| `circularprefixsum` | `O(n)` | 回転・和`O(1)` | `O(n)` |
| `prefixsum2d` | `O(hw)` | `O(1)` | `O(hw)` |
| `multidimensionalprefixsum` | `O(dN)` | `O(d 2^d)` | `O(N)` |

`T`には`T{}`、加算、減算が必要である。浮動小数点数では、引き算による誤差に注意する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
<details class="api-reference" markdown="1">
<summary>すべてのAPI宣言を表示</summary>

### 完全なAPIリファレンス

headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `differencearray`

```cpp
template<class T> struct differencearray
```

区間加算をまとめて反映する差分配列: differencearray<long long> diff(n); add後にbuildする。

### `differencearray`

```cpp
explicit differencearray(int n) : difference_(static_cast<std::size_t>(std::max(0, n) + 1)), values_(static_cast<std::size_t>(std::max(0, n)))
```

O(n)。長さnの0列を作る。

### `differencearray`

```cpp
explicit differencearray(const std::vector<T>& values) : difference_(values.size() + 1), values_(values)
```

O(n)。valuesを初期値とする差分配列を作る。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `add`

```cpp
void add(int left, int right, const T& value)
```

O(1)。半開区間[left,right)へvalueを加える。取得前にbuildを呼ぶ。

### `build`

```cpp
void build()
```

O(n)。追加をすべて反映し、operator[]で取得できるようにする。

### `operator[]`

```cpp
const T& operator[](int index) const
```

O(1)。build後のindex番目を返す。

### `values`

```cpp
const std::vector<T>& values() const
```

O(1)。build後の列を返す。

### `differencearray2d`

```cpp
template <class T> struct differencearray2d
```

長方形加算をまとめて反映する2次元差分配列: differencearray2d<int> diff(h,w); add後にbuildする。

### `differencearray2d`

```cpp
differencearray2d(int height, int width) : height_(height), width_(width), difference_(std::max(0, height) + 1, std::vector<T>(std::max(0, width) + 1)), values_(std::max(0, height), std::vector<T>(std::max(0, width)))
```

O(hw)。height行width列の0表を作る。

### `differencearray2d`

```cpp
explicit differencearray2d(const std::vector<std::vector<T>>& values) : differencearray2d( static_cast<int>(values.size()), values.empty() ? 0 : static_cast<int>(values.front().size()))
```

O(hw)。valuesを初期値とする2次元差分配列を作る。

### `height`

```cpp
int height() const noexcept
```

O(1)。行数を返す。

### `width`

```cpp
int width() const noexcept
```

O(1)。列数を返す。

### `add`

```cpp
void add(int top, int left, int bottom, int right, const T& value)
```

O(1)。半開長方形[top,bottom)x[left,right)へvalueを加える。取得前にbuildを呼ぶ。

### `build`

```cpp
void build()
```

O(hw)。追加をすべて反映し、operator()で取得できるようにする。

### `operator()`

```cpp
const T& operator()(int row, int column) const
```

O(1)。build後の(row,column)を返す。

### `values`

```cpp
const std::vector<std::vector<T>>& values() const
```

O(1)。build後の表を返す。

### `prefixsum`

```cpp
template <class T> struct prefixsum
```

静的な列の区間和: prefixsum<long long> ps(values); 構築O(n)、取得O(1)。

### `prefixsum`

```cpp
explicit prefixsum(const std::vector<T>& values) : values_(values), sums_(values.size() + 1)
```

O(n)。valuesから累積和を構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `operator[]`

```cpp
const T& operator[](int index) const
```

O(1)。ps[index]で元の列の要素を返す。

### `sum`

```cpp
T sum(int right) const
```

O(1)。ps.sum(right)で[0, right)の和を返す。

### `sum`

```cpp
T sum(int left, int right) const
```

O(1)。ps.sum(left, right)で[left, right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。ps.sum()で全要素の和を返す。

### `subarrayprefixsum`

```cpp
template <class T> struct subarrayprefixsum
```

全部分配列和の静的区間query: subarrayprefixsum<long long> sums(values); sum(l,r)は[l,r)内。

### `subarrayprefixsum`

```cpp
explicit subarrayprefixsum(const std::vector<T>& values) : values_(values)
```

O(n)。valuesと添字一次・二次momentの累積和を構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `operator[]`

```cpp
const T& operator[](int index) const
```

O(1)。元の列のindex番目を返す。

### `sum`

```cpp
T sum(int left, int right) const
```

O(1)。values[left:right]の全非空部分配列の要素和を合計して返す。

### `sum`

```cpp
T sum() const
```

O(1)。列全体の全非空部分配列の要素和を合計して返す。

### `circularprefixsum`

```cpp
template <class T> struct circularprefixsum
```

静的円環列の左回転と区間和: circularprefixsum<long long> data(values)。

### `circularprefixsum`

```cpp
explicit circularprefixsum(const std::vector<T>& values) : data_(doubled(values)), size_(static_cast<int>(values.size()))
```

O(n)。非空列valuesから構築する。

### `rotate`

```cpp
void rotate(long long left)
```

O(1)。論理列をleft個だけ左回転する。負なら右回転する。

### `sum`

```cpp
T sum(int right) const
```

O(1)。現在の論理列の[0,right)の和を返す。

### `sum`

```cpp
T sum(int left, int right) const
```

O(1)。現在の論理列の[left,right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。全要素の和を返す。

### `operator[]`

```cpp
const T& operator[](int index) const
```

O(1)。data[index]は現在の論理列の要素を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `offset`

```cpp
int offset() const noexcept
```

O(1)。元の列からの左回転量を返す。

### `prefixsum2d`

```cpp
template <class T> struct prefixsum2d
```

静的な長方形領域の和: prefixsum2d<long long> ps(grid); 構築O(hw)、取得O(1)。

### `prefixsum2d`

```cpp
explicit prefixsum2d(const std::vector<std::vector<T>>& grid) : values_(grid), height_(static_cast<int>(grid.size())), width_(grid.empty() ? 0 : static_cast<int>(grid.front().size())), sums_(height_ + 1, std::vector<T>(width_ + 1))
```

O(hw)。長方形のgridから2次元累積和を構築する。

### `height`

```cpp
int height() const noexcept
```

O(1)。行数を返す。

### `width`

```cpp
int width() const noexcept
```

O(1)。列数を返す。

### `operator()`

```cpp
const T& operator()(int row, int column) const
```

O(1)。ps(row, column)で元の表の要素を返す。

### `sum`

```cpp
T sum(int bottom, int right) const
```

O(1)。ps.sum(bottom, right)で[0,bottom)x[0,right)の和を返す。

### `sum`

```cpp
T sum(int top, int left, int bottom, int right) const
```

O(1)。ps.sum(top, left, bottom, right)で半開長方形の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。ps.sum()で全要素の和を返す。

### `multidimensionalprefixsum`

```cpp
template<class T> struct multidimensionalprefixsum
```

静的な直方体領域の和: multidimensionalprefixsum<long long> ps(shape, values); 構築O(dN)。

### `multidimensionalprefixsum`

```cpp
multidimensionalprefixsum(std::vector<int> shape, std::vector<T> values) : shape_(std::move(shape)), values_(std::move(values)), strides_(shape_.size())
```

O(dN)。各次元の長さshapeとrow-major順のvaluesから多次元累積和を構築する。

### `dimensions`

```cpp
int dimensions() const noexcept
```

O(1)。次元数を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。全要素数を返す。

### `shape`

```cpp
const std::vector<int>& shape() const noexcept
```

O(1)。各次元の長さを返す。

### `operator[]`

```cpp
const T& operator[](int index) const
```

O(1)。ps[index]でrow-major順index番目の元要素を返す。

### `operator()`

```cpp
const T& operator()(const std::vector<int>& coordinates) const
```

O(d)。ps(coordinates)で指定座標の元要素を返す。

### `sum`

```cpp
T sum(const std::vector<int>& upper) const
```

O(d)。各次元の[0,upper[d])からなる直方体領域の和を返す。

### `sum`

```cpp
T sum(const std::vector<int>& lower, const std::vector<int>& upper) const
```

O(d 2^d)。各次元の[lower[d],upper[d])からなる直方体領域の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。全要素の和を返す。

</details>

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [Library Checker - Static Range Sum](https://judge.yosupo.jp/problem/static_range_sum):
  `prefixsum<long long>`を使うverifyコードを収録。
- [AtCoder ABC106 D - AtCoder Express 2](https://atcoder.jp/contests/abc106/tasks/abc106_d):
  区間の両端を2次元表へ置く`prefixsum2d`の適用例。
- [AtCoder ABC465 F - Sjeltzer?](https://atcoder.jp/contests/abc465/tasks/abc465_f):
  10要素の軸を6本持つ格子へIDを置き、多次元累積和で各queryを`O(6*2^6)`処理する。
- [AtCoder ABC462 D - Accomplice](https://atcoder.jp/contests/abc462/tasks/abc462_d):
  各人物が犯行開始から終了まで滞在できる開始時刻区間を加算する。
- [AtCoder ABC434 D - Clouds](https://atcoder.jp/contests/abc434/tasks/abc434_d):
  雲の長方形へ被覆数とID和を加え、各マスが未被覆か単独被覆かを判定する。
- [AtCoder ABC425 C - Rotate and Sum Query](https://atcoder.jp/contests/abc425/tasks/abc425_c):
  `rotate(c)`で先頭から末尾への移動を反映し、`sum(l,r)`で現在列の区間和を得る。
- [AtCoder ABC423 E - Sum of Subarrays](https://atcoder.jp/contests/abc423/tasks/abc423_e):
  `subarrayprefixsum<long long>`で各指定区間に含まれる全部分配列和を`O(1)`で得る。

## 検証

- `tests/api/structure/prefixsum.cpp`: 公開APIのsmoke test
- `tests/property/structure/prefixsum.cpp`: 1次元・2次元の愚直和との固定seed比較
- `verify/librarychecker_static_range_sum.cpp`: 公開問題用コード。
- `verify/atcoder_abc465_f.cpp`: ABC465 F提出用コード。公式サンプル1確認済み、
- `verify/atcoder_abc462_d.cpp`: ABC462 D提出用コード。公式サンプル確認済み、
- `verify/atcoder_abc434_d.cpp`: ABC434 D提出用コード。公式サンプル確認済み、
- `verify/atcoder_abc425_c.cpp`: ABC425 C提出用コード。公式サンプル一致、
- `verify/atcoder_abc423_e.cpp`: ABC423 E提出用コード。公式サンプル一致、
