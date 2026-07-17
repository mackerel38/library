---
title: "用途別区間構造"
documentation_of: //cp/utility/presetsegtree.hpp
---

# 用途別区間構造

## Include

```cpp
#include "utility/presetsegtree.hpp"
```

`cp/utility/presetsegtree.hpp`は、汎用Fenwick Tree・Segtree・Lazy Segtreeを
問題文に現れる更新と取得の名前で包んだ完成品を提供する。公開型名は実装方式ではなく
`point_affine_range_sum`のように「更新方法・取得範囲・取得内容」を表す。

## Point update

- `point_add_range_sum`
- `point_affine_range_sum`
- `point_flip_range_sum`
- `point_set_range_min`
- `point_set_range_max`
- `point_set_prefix_sum_max`
- `point_set_range_manhattan_max`
- `point_set_range_gcd`
- `point_set_range_composite`

## Range update

- `range_add_point_get`
- `range_set_point_get`
- `range_affine_point_get`
- `range_add_range_sum`
- `range_add_range_sum_square`
- `range_set_range_sum`
- `range_affine_range_sum`
- `range_add_range_min`
- `range_subtract_clamp_zero_sum`
- `range_add_range_max`
- `range_set_range_min`
- `range_set_range_max`
- `range_add_range_gcd`
- `range_set_range_gcd`
- `range_flip_range_sum`
- `range_flip_range_inversion`
- `range_active_add_range_clear_flip_max`
- `range_chmin_chmax_add_range_sum`

すべて半開区間`[left,right)`を使う。構築は`O(n)`、一点・区間更新と区間取得は
特記がない限り`O(log n)`。配列として意味のある型は`operator[]`と一点`set`を持つ。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `point_add_range_sum`

```cpp
template <class T> struct point_add_range_sum
```

一点加算・一点代入・区間和: point_add_range_sum<long long> data(values)。

### `point_add_range_sum`

```cpp
explicit point_add_range_sum(int n) : data_(n)
```

O(n)。n個の0から構築する。

### `point_add_range_sum`

```cpp
explicit point_add_range_sum(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `add`

```cpp
void add(int index, const T& delta)
```

O(log n)。a[index] += deltaを行う。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `sum`

```cpp
T sum(int right) const
```

O(log n)。[0,right)の和を返す。

### `sum`

```cpp
T sum(int left, int right) const
```

O(log n)。[left,right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(log n)。全要素の和を返す。

### `operator[]`

```cpp
T operator[](int index) const
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `point_affine_range_sum`

```cpp
template <class T> struct point_affine_range_sum
```

一点一次変換・一点代入・区間和: data.affine(i,multiply,add)。

### `point_affine_range_sum`

```cpp
explicit point_affine_range_sum(int n) : data_(n)
```

O(n)。n個の0から構築する。

### `point_affine_range_sum`

```cpp
explicit point_affine_range_sum(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `affine`

```cpp
void affine(int index, const T& multiply, const T& add)
```

O(log n)。a[index] = multiply*a[index]+addを行う。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `sum`

```cpp
T sum(int right) const
```

O(log n)。[0,right)の和を返す。

### `sum`

```cpp
T sum(int left, int right) const
```

O(log n)。[left,right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(log n)。全要素の和を返す。

### `operator[]`

```cpp
T operator[](int index) const
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `point_flip_range_sum`

```cpp
struct point_flip_range_sum
```

一点反転・一点代入・区間1数: point_flip_range_sum data(bits)。

### `point_flip_range_sum`

```cpp
explicit point_flip_range_sum(int n) : data_(n)
```

O(n)。n個の0から構築する。

### `point_flip_range_sum`

```cpp
explicit point_flip_range_sum(const std::vector<int>& bits) : data_(bits)
```

O(n)。0/1列bitsから構築する。

### `flip`

```cpp
void flip(int index)
```

O(log n)。a[index]を0と1で反転する。

### `set`

```cpp
void set(int index, int value)
```

O(log n)。a[index]を0または1へ代入する。

### `sum`

```cpp
int sum(int right) const
```

O(log n)。[0,right)にある1の個数を返す。

### `sum`

```cpp
int sum(int left, int right) const
```

O(log n)。[left,right)にある1の個数を返す。

### `sum`

```cpp
int sum() const
```

O(log n)。全体にある1の個数を返す。

### `operator[]`

```cpp
int operator[](int index) const
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `point_set_range_min`

```cpp
template <class T> struct point_set_range_min
```

一点代入・区間最小値: point_set_range_min<long long> data(values)。

### `point_set_range_min`

```cpp
explicit point_set_range_min(int n) : data_(n)
```

O(n)。n個のmax()から構築する。

### `point_set_range_min`

```cpp
explicit point_set_range_min(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `min`

```cpp
T min(int left, int right) const
```

O(log n)。[left,right)の最小値を返す。

### `min`

```cpp
T min() const
```

O(1)。全要素の最小値を返す。

### `operator[]`

```cpp
T operator[](int index) const
```

O(1)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `point_set_range_max`

```cpp
template <class T> struct point_set_range_max
```

一点代入・区間最大値: point_set_range_max<long long> data(values)。

### `point_set_range_max`

```cpp
explicit point_set_range_max(int n) : data_(n)
```

O(n)。n個のlowest()から構築する。

### `point_set_range_max`

```cpp
explicit point_set_range_max(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `max`

```cpp
T max(int left, int right) const
```

O(log n)。[left,right)の最大値を返す。

### `max`

```cpp
T max() const
```

O(1)。全要素の最大値を返す。

### `operator[]`

```cpp
T operator[](int index) const
```

O(1)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `point_set_prefix_sum_max`

```cpp
template <class T> struct point_set_prefix_sum_max
```

一点代入とprefix和・最大prefix和・最初の到達位置を扱う。

### `point_set_prefix_sum_max`

```cpp
explicit point_set_prefix_sum_max(const std::vector<T>& values) : data_(make_values(values))
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `add`

```cpp
void add(int index, const T& delta)
```

O(log n)。a[index] += deltaを行う。

### `operator[]`

```cpp
T operator[](int index) const
```

O(1)。a[index]を返す。

### `sum`

```cpp
T sum(int right) const
```

O(log n)。[0,right)の和を返す。

### `sum`

```cpp
T sum(int left, int right) const
```

O(log n)。[left,right)の和を返す。

### `max_prefix`

```cpp
T max_prefix() const
```

O(1)。非空prefix和の最大値を返す。空列ではlowest()。

### `first_prefix_at_least`

```cpp
int first_prefix_at_least(const T& target) const
```

O(log n)。prefix和がtarget以上になる最初の要素index。なければsize()。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `point_set_range_manhattan_max`

```cpp
template <class T> struct point_set_range_manhattan_max
```

点列の一点変更と区間内最遠Manhattan距離: point_set_range_manhattan_max<long long> data(points)。

### `point_set_range_manhattan_max`

```cpp
explicit point_set_range_manhattan_max(const std::vector<point>& points) : points_(points), sum_(make_sum(points)), difference_(make_difference(points))
```

O(n)。pointsから構築する。

### `set`

```cpp
void set(int index, T x, T y)
```

O(log n)。point[index] = (x, y)を行う。

### `max`

```cpp
T max(int left, int right, T x, T y) const
```

O(log n)。[left,right)内の点と(x,y)とのManhattan距離の最大値を返す。

### `operator[]`

```cpp
point operator[](int index) const
```

O(1)。point[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。点数を返す。

### `point_set_range_gcd`

```cpp
template <class T> struct point_set_range_gcd
```

一点代入・区間gcd: point_set_range_gcd<long long> data(values)。

### `point_set_range_gcd`

```cpp
explicit point_set_range_gcd(int n) : data_(n)
```

O(n)。n個の0から構築する。

### `point_set_range_gcd`

```cpp
explicit point_set_range_gcd(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `gcd`

```cpp
T gcd(int left, int right) const
```

O(log n)。[left,right)のgcdを返す。空区間は0。

### `gcd`

```cpp
T gcd() const
```

O(1)。全要素のgcdを返す。

### `operator[]`

```cpp
T operator[](int index) const
```

O(1)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `point_set_range_composite`

```cpp
template <class T> struct point_set_range_composite
```

一点関数代入・区間関数合成: point_set_range_composite<modint> data(functions)。

### `point_set_range_composite`

```cpp
explicit point_set_range_composite(int n) : data_(n)
```

O(n)。n個の恒等関数から構築する。

### `point_set_range_composite`

```cpp
explicit point_set_range_composite(const std::vector<affine>& functions) : data_(functions)
```

O(n)。functionsから構築する。

### `set`

```cpp
void set(int index, const T& multiply, const T& add)
```

O(log n)。index番目の関数をmultiply*x+addへ置き換える。

### `composite`

```cpp
affine composite(int left, int right) const
```

O(log n)。[left,right)を添字順に適用した合成関数を返す。

### `apply`

```cpp
T apply(int left, int right, const T& value) const
```

O(log n)。[left,right)を添字順にvalueへ適用する。

### `operator[]`

```cpp
affine operator[](int index) const
```

O(1)。data[index]はindex番目の関数を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_add_range_sum`

```cpp
template <class T> struct range_add_range_sum
```

区間加算・区間和: range_add_range_sum<long long> data(values)。

### `range_add_range_sum`

```cpp
explicit range_add_range_sum(int n) : data_(detail::make_zero_sum_nodes<T>(n))
```

O(n)。n個の0から構築する。

### `range_add_range_sum`

```cpp
explicit range_add_range_sum(const std::vector<T>& values) : data_(detail::make_sum_nodes(values))
```

O(n)。valuesから構築する。

### `add`

```cpp
void add(int left, int right, const T& delta)
```

O(log n)。[left,right)の各要素へdeltaを加える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `sum`

```cpp
T sum(int right)
```

O(log n)。[0,right)の和を返す。

### `sum`

```cpp
T sum(int left, int right)
```

O(log n)。[left,right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。全要素の和を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_add_range_sum_square`

```cpp
template <class T> struct range_add_range_sum_square
```

区間加算・区間和・区間二乗和: range_add_range_sum_square<mint> data(values)。

### `range_add_range_sum_square`

```cpp
explicit range_add_range_sum_square(int n) : data_(detail::make_zero_sum_square_nodes<T>(n))
```

O(n)。n個の0から構築する。

### `range_add_range_sum_square`

```cpp
explicit range_add_range_sum_square(const std::vector<T>& values) : data_(detail::make_sum_square_nodes(values))
```

O(n)。valuesから構築する。

### `add`

```cpp
void add(int left, int right, const T& delta)
```

O(log n)。[left,right)の各要素へdeltaを加える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `sum`

```cpp
T sum(int right)
```

O(log n)。[0,right)の和を返す。

### `sum`

```cpp
T sum(int left, int right)
```

O(log n)。[left,right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。全要素の和を返す。

### `square_sum`

```cpp
T square_sum(int right)
```

O(log n)。[0,right)の二乗和を返す。

### `square_sum`

```cpp
T square_sum(int left, int right)
```

O(log n)。[left,right)の二乗和を返す。

### `square_sum`

```cpp
T square_sum() const
```

O(1)。全要素の二乗和を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_set_range_sum`

```cpp
template <class T> struct range_set_range_sum
```

区間代入・区間和: range_set_range_sum<long long> data(values)。

### `range_set_range_sum`

```cpp
explicit range_set_range_sum(int n) : data_(detail::make_zero_sum_nodes<T>(n))
```

O(n)。n個の0から構築する。

### `range_set_range_sum`

```cpp
explicit range_set_range_sum(const std::vector<T>& values) : data_(detail::make_sum_nodes(values))
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int left, int right, const T& value)
```

O(log n)。[left,right)の各要素をvalueへ置き換える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `sum`

```cpp
T sum(int right)
```

O(log n)。[0,right)の和を返す。

### `sum`

```cpp
T sum(int left, int right)
```

O(log n)。[left,right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。全要素の和を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_affine_range_sum`

```cpp
template <class T> struct range_affine_range_sum
```

区間一次変換・区間和: data.affine(left,right,multiply,add)。

### `range_affine_range_sum`

```cpp
explicit range_affine_range_sum(int n) : data_(detail::make_zero_sum_nodes<T>(n))
```

O(n)。n個の0から構築する。

### `range_affine_range_sum`

```cpp
explicit range_affine_range_sum(const std::vector<T>& values) : data_(detail::make_sum_nodes(values))
```

O(n)。valuesから構築する。

### `affine`

```cpp
void affine(int left, int right, const T& multiply, const T& add)
```

O(log n)。[left,right)をmultiply*x+addで更新する。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `sum`

```cpp
T sum(int right)
```

O(log n)。[0,right)の和を返す。

### `sum`

```cpp
T sum(int left, int right)
```

O(log n)。[left,right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。全要素の和を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_add_range_min`

```cpp
template <class T> struct range_add_range_min
```

区間加算・区間最小値: range_add_range_min<long long> data(values)。

### `range_add_range_min`

```cpp
explicit range_add_range_min(int n) : data_(std::vector<T>(n))
```

O(n)。n個の0から構築する。

### `range_add_range_min`

```cpp
explicit range_add_range_min(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `add`

```cpp
void add(int left, int right, const T& delta)
```

O(log n)。[left,right)の各要素へdeltaを加える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `min`

```cpp
T min(int left, int right)
```

O(log n)。[left,right)の最小値を返す。

### `min`

```cpp
T min() const
```

O(1)。全要素の最小値を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_subtract_clamp_zero_sum`

```cpp
template <class T> struct range_subtract_clamp_zero_sum
```

区間消費・0下限・消費量和: data.subtract(left,right,amount)。

### `range_subtract_clamp_zero_sum`

```cpp
explicit range_subtract_clamp_zero_sum(int n) : range_subtract_clamp_zero_sum(std::vector<T>(n))
```

O(n)。n個の0から構築する。

### `range_subtract_clamp_zero_sum`

```cpp
explicit range_subtract_clamp_zero_sum(const std::vector<T>& values) : data_(detail::make_consume_nodes(values))
```

O(n)。非負列valuesから構築する。

### `subtract`

```cpp
T subtract(int left, int right, const T& amount)
```

O((z+1)log n)。[left,right)の各値からmin(value,amount)を引き、消費量和を返す。
zはこの操作で初めて0になる要素数。全操作では各要素が高々一度だけzへ数えられる。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueとして再び有効化する。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]は現在値を返す。

### `active`

```cpp
bool active(int index)
```

O(log n)。indexがまだ消費対象ならtrueを返す。値0でもset直後はtrue。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_add_range_max`

```cpp
template <class T> struct range_add_range_max
```

区間加算・区間最大値: range_add_range_max<long long> data(values)。

### `range_add_range_max`

```cpp
explicit range_add_range_max(int n) : data_(std::vector<T>(n))
```

O(n)。n個の0から構築する。

### `range_add_range_max`

```cpp
explicit range_add_range_max(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `add`

```cpp
void add(int left, int right, const T& delta)
```

O(log n)。[left,right)の各要素へdeltaを加える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `max`

```cpp
T max(int left, int right)
```

O(log n)。[left,right)の最大値を返す。

### `max`

```cpp
T max() const
```

O(1)。全要素の最大値を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_set_range_min`

```cpp
template <class T> struct range_set_range_min
```

区間代入・区間最小値: range_set_range_min<long long> data(values)。

### `range_set_range_min`

```cpp
explicit range_set_range_min(int n) : data_(n)
```

O(n)。n個のmax()から構築する。

### `range_set_range_min`

```cpp
explicit range_set_range_min(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int left, int right, const T& value)
```

O(log n)。[left,right)の各要素をvalueへ置き換える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `min`

```cpp
T min(int left, int right)
```

O(log n)。[left,right)の最小値を返す。

### `min`

```cpp
T min() const
```

O(1)。全要素の最小値を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_set_range_max`

```cpp
template <class T> struct range_set_range_max
```

区間代入・区間最大値: range_set_range_max<long long> data(values)。

### `range_set_range_max`

```cpp
explicit range_set_range_max(int n) : data_(n)
```

O(n)。n個のlowest()から構築する。

### `range_set_range_max`

```cpp
explicit range_set_range_max(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int left, int right, const T& value)
```

O(log n)。[left,right)の各要素をvalueへ置き換える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `max`

```cpp
T max(int left, int right)
```

O(log n)。[left,right)の最大値を返す。

### `max`

```cpp
T max() const
```

O(1)。全要素の最大値を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_set_range_gcd`

```cpp
template <class T> struct range_set_range_gcd
```

区間代入・区間gcd: range_set_range_gcd<long long> data(values)。

### `range_set_range_gcd`

```cpp
explicit range_set_range_gcd(int n) : data_(std::vector<node>(n, node
```

O(n)。n個の0から構築する。

### `range_set_range_gcd`

```cpp
explicit range_set_range_gcd(const std::vector<T>& values) : data_(make_nodes(values))
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int left, int right, const T& value)
```

O(log n)。[left,right)の各要素をvalueへ置き換える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `gcd`

```cpp
T gcd(int left, int right)
```

O(log n)。[left,right)のgcdを返す。空区間は0。

### `gcd`

```cpp
T gcd() const
```

O(1)。全要素のgcdを返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_add_range_gcd`

```cpp
template <class T> struct range_add_range_gcd
```

区間加算・区間gcd: range_add_range_gcd<long long> data(values)。

### `range_add_range_gcd`

```cpp
explicit range_add_range_gcd(int n) : range_add_range_gcd(std::vector<T>(n))
```

O(n)。n個の0から構築する。

### `range_add_range_gcd`

```cpp
explicit range_add_range_gcd(const std::vector<T>& values) : difference_(make_difference(values)), gcd_(make_difference(values))
```

O(n)。valuesから構築する。

### `add`

```cpp
void add(int left, int right, const T& delta)
```

O(log n)。[left,right)の各要素へdeltaを加える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `gcd`

```cpp
T gcd(int left, int right) const
```

O(log n)。[left,right)のgcdを返す。空区間は0。

### `gcd`

```cpp
T gcd() const
```

O(log n)。全要素のgcdを返す。

### `operator[]`

```cpp
T operator[](int index) const
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_add_point_get`

```cpp
template <class T> struct range_add_point_get
```

区間加算・一点取得: range_add_point_get<long long> data(values)。

### `range_add_point_get`

```cpp
explicit range_add_point_get(int n) : data_(n)
```

O(n)。n個の0から構築する。

### `range_add_point_get`

```cpp
explicit range_add_point_get(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `add`

```cpp
void add(int left, int right, const T& delta)
```

O(log n)。[left,right)の各要素へdeltaを加える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_set_point_get`

```cpp
template <class T> struct range_set_point_get
```

区間代入・一点取得: range_set_point_get<long long> data(values)。

### `range_set_point_get`

```cpp
explicit range_set_point_get(int n) : data_(n)
```

O(n)。n個の0から構築する。

### `range_set_point_get`

```cpp
explicit range_set_point_get(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `set`

```cpp
void set(int left, int right, const T& value)
```

O(log n)。[left,right)の各要素をvalueへ置き換える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_affine_point_get`

```cpp
template <class T> struct range_affine_point_get
```

区間一次変換・一点取得: range_affine_point_get<long long> data(values)。

### `range_affine_point_get`

```cpp
explicit range_affine_point_get(int n) : data_(n)
```

O(n)。n個の0から構築する。

### `range_affine_point_get`

```cpp
explicit range_affine_point_get(const std::vector<T>& values) : data_(values)
```

O(n)。valuesから構築する。

### `affine`

```cpp
void affine(int left, int right, const T& multiply, const T& add)
```

O(log n)。[left,right)をmultiply*x+addで更新する。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_flip_range_sum`

```cpp
struct range_flip_range_sum
```

区間反転・区間1数: range_flip_range_sum data(bits)。

### `range_flip_range_sum`

```cpp
explicit range_flip_range_sum(int n) : range_flip_range_sum(std::vector<int>(n))
```

O(n)。n個の0から構築する。

### `range_flip_range_sum`

```cpp
explicit range_flip_range_sum(const std::vector<int>& bits) : data_(make_nodes(bits))
```

O(n)。0/1列bitsから構築する。

### `flip`

```cpp
void flip(int left, int right)
```

O(log n)。[left,right)を0と1で反転する。

### `set`

```cpp
void set(int index, int value)
```

O(log n)。a[index]を0または1へ代入する。

### `sum`

```cpp
long long sum(int left, int right)
```

O(log n)。[left,right)にある1の個数を返す。

### `sum`

```cpp
long long sum() const
```

O(1)。全体にある1の個数を返す。

### `operator[]`

```cpp
int operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_flip_range_inversion`

```cpp
struct range_flip_range_inversion
```

区間反転・区間転倒数: range_flip_range_inversion data(bits)。

### `range_flip_range_inversion`

```cpp
explicit range_flip_range_inversion(int n) : range_flip_range_inversion(std::vector<int>(n))
```

O(n)。n個の0から構築する。

### `range_flip_range_inversion`

```cpp
explicit range_flip_range_inversion(const std::vector<int>& bits) : data_(make_nodes(bits))
```

O(n)。0/1列bitsから構築する。

### `flip`

```cpp
void flip(int left, int right)
```

O(log n)。[left,right)を0と1で反転する。

### `set`

```cpp
void set(int index, int value)
```

O(log n)。a[index]を0または1へ代入する。

### `inversion`

```cpp
long long inversion(int left, int right)
```

O(log n)。[left,right)にある(1,0)の添字対数を返す。

### `sum`

```cpp
long long sum(int left, int right)
```

O(log n)。[left,right)にある1の個数を返す。

### `operator[]`

```cpp
int operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_active_add_range_clear_flip_max`

```cpp
template <class T> struct range_active_add_range_clear_flip_max
```

区間active加算・全値clear後のactive反転・区間最大値をO(log n)で扱う。

### `range_active_add_range_clear_flip_max`

```cpp
explicit range_active_add_range_clear_flip_max(int n) : data_(make_zero_nodes(n))
```

O(n)。activeかつ値0のn要素から構築する。

### `range_active_add_range_clear_flip_max`

```cpp
explicit range_active_add_range_clear_flip_max(const std::vector<T>& values) : data_(make_nodes(values))
```

O(n)。すべてactiveなvaluesから構築する。

### `add`

```cpp
void add(int left, int right, const T& delta)
```

O(log n)。[left,right)のactive要素だけへdeltaを加える。

### `clear_flip`

```cpp
void clear_flip(int left, int right)
```

O(log n)。[left,right)の全値を0にしてactive状態を反転する。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index]をvalueかつactiveへ置き換える。

### `deactivate`

```cpp
void deactivate(int index)
```

O(log n)。a[index]を0かつinactiveへ置き換える。

### `max`

```cpp
T max(int left, int right)
```

O(log n)。[left,right)の全要素の最大値を返す。区間は空でないこと。

### `max`

```cpp
T max() const
```

O(1)。全要素の最大値を返す。要素数は正であること。

### `active`

```cpp
bool active(int index)
```

O(log n)。index番目がactiveならtrueを返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はindex番目の値を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

### `range_chmin_chmax_add_range_sum`

```cpp
template<class T> struct range_chmin_chmax_add_range_sum
```

区間chmin・chmax・加算・一点代入・区間和: Segment Tree Beatsで各操作を償却O(log n)で行う。

### `range_chmin_chmax_add_range_sum`

```cpp
explicit range_chmin_chmax_add_range_sum(int n) : range_chmin_chmax_add_range_sum(std::vector<T>(n))
```

O(n)。n個の0から構築する。

### `range_chmin_chmax_add_range_sum`

```cpp
explicit range_chmin_chmax_add_range_sum(const std::vector<T>& values) : n_(static_cast<int>(values.size())), nodes_(std::max(1, 4 * n_))
```

O(n)。valuesから構築する。

### `chmin`

```cpp
void chmin(int left, int right, const T& value)
```

償却O(log n)。[left,right)の各要素をvalue以下へ切り詰める。

### `chmax`

```cpp
void chmax(int left, int right, const T& value)
```

償却O(log n)。[left,right)の各要素をvalue以上へ切り上げる。

### `add`

```cpp
void add(int left, int right, const T& value)
```

償却O(log n)。[left,right)の各要素へvalueを加える。

### `set`

```cpp
void set(int index, const T& value)
```

O(log n)。a[index] = valueを行う。

### `sum`

```cpp
T sum(int left, int right)
```

O(log n)。[left,right)の和を返す。

### `sum`

```cpp
T sum() const
```

O(1)。全要素の和を返す。

### `operator[]`

```cpp
T operator[](int index)
```

O(log n)。data[index]はa[index]を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。要素数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Range Affine Range Sum](https://judge.yosupo.jp/problem/range_affine_range_sum)。

```cpp
poe::range_affine_range_sum<poe::modint998244353> data(values);
if (type == 0) data.affine(left, right, multiply, add);
else std::cout << data.sum(left, right).val() << '\n';
```

[Library Checker - Point Set Range Composite](https://judge.yosupo.jp/problem/point_set_range_composite)では
`point_set_range_composite`を使う。検証コードは
`verify/preset_range_affine_range_sum.test.cpp`と`verify/preset_point_set_range_composite.test.cpp`。
2026-07-15に両方の全ケースAC確認済み。

[AtCoder ABC455 F - Merge Slimes 2](https://atcoder.jp/contests/abc455/tasks/abc455_f)では、
`range_add_range_sum_square`で区間加算後の和`S`と二乗和`Q`を取得し、
異なる二要素の積の総和を`(S*S-Q)/2`として求める。
`verify/atcoder_abc455_f.cpp`に使用例を収録した。judge未提出。

`point_set_prefix_sum_max<T>`は一点更新、prefix和、最大prefix和と
「prefix和が指定値以上になる最初の要素index」を扱う。

```cpp
poe::point_set_prefix_sum_max<long long> data(values);
data.set(index, value);
int first = data.first_prefix_at_least(0);
long long prefix = data.sum(first + 1);
```

[AtCoder ABC292 Ex - Rating Estimator](https://atcoder.jp/contests/abc292/tasks/abc292_h)では、
各値から基準値を引いた列で最初にprefix和が0以上となる位置を求める。
`verify/atcoder_abc292_h.cpp`で公式sampleを確認済み。judge未提出。

[AtCoder ABC441 G - Takoyaki and Flip](https://atcoder.jp/contests/abc441/tasks/abc441_g)では、
各要素のactive/inactive状態を持ち、`add`でactive要素だけに加算する。
`clear_flip`は区間の全値を0にしてから状態を反転し、`max`で状態によらない最大値を得る。

```cpp
poe::range_active_add_range_clear_flip_max<long long> data(n);
data.add(left, right, value);
data.clear_flip(left, right);
std::cout << data.max(left, right) << '\n';
```

構築は`O(n)`、各操作は`O(log n)`。`operator[]`、`active`、一点`set`、
一点`deactivate`も備える。`verify/atcoder_abc441_g.cpp`で公式sampleを確認済み、judge未提出。

## Segment Tree Beats

`range_chmin_chmax_add_range_sum<T>`は、次の操作を一つの配列に対して行う。

- `chmin(left, right, value)`: 区間の各値を`min(a[i], value)`へ更新
- `chmax(left, right, value)`: 区間の各値を`max(a[i], value)`へ更新
- `add(left, right, value)`: 区間加算
- `set(index, value)`: 一点代入
- `sum(left, right)`、`operator[]`: 区間和・一点取得

構築は`O(n)`。Segment Tree Beatsの償却解析により、各区間操作は償却`O(log n)`、
一点代入・一点取得も`O(log n)`となる。値型`T`には加減算、大小比較、整数長との乗算、
`numeric_limits<T>`が必要である。

[Library Checker - Range Chmin Chmax Add Range Sum](https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum)
向けのコードを`verify/range_chmin_chmax_add_range_sum.test.cpp`に用意している。

`point_set_range_manhattan_max<T>`は点の一点変更と、添字区間内の点から指定点までの
Manhattan距離最大値を`O(log n)`で扱う。[ABC437 F](https://atcoder.jp/contests/abc437/tasks/abc437_f)の
`verify/atcoder_abc437_f.cpp`は公式サンプル一致、judge未提出。
property testでは愚直配列との乱択照合を行っているが、judgeへの提出確認は未実施。

## 0を下限とする区間消費

`range_subtract_clamp_zero_sum<T>`は非負列に対して、区間内の各値`a[i]`を
`max(0, a[i]-amount)`へ更新し、実際に引けた量の総和を返す。

```cpp
poe::range_subtract_clamp_zero_sum<long long> stock(values);
long long removed = stock.subtract(left, right, amount);
```

構築は`O(n)`。一回の`subtract`は、その操作で初めて0へ到達する要素数を`z`として
`O((z+1) log n)`であり、一度0になった要素は`set`されない限り再探索しない。
したがって`set`を使わない一連の操作全体では`O((n+q) log n)`となる。
`operator[]`、一点`set`、消費対象か調べる`active`も持つ。

[AtCoder ABC426 F - Clearance](https://atcoder.jp/contests/abc426/tasks/abc426_f)では、
各商品の在庫から注文数までを引き、返された総量をそのまま出力できる。
`verify/atcoder_abc426_f.cpp`で公式sample一致、judge未提出。
