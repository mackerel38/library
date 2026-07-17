---
title: "入れ子区間列"
documentation_of: //cp/structure/nestedintervals.hpp
---

# 入れ子区間列


## 概要

長さが厳密増加する区間列を、最初はすべて左端0で置く。その後、prefix全体の左端または右端を
一つの座標へ揃える操作を繰り返しながら、各区間や一点を覆う区間数を取得する。

## 厳密な定義

- `nestedintervals`: 長さ昇順の入れ子区間列をprefix左端・右端整列で管理する: nestedintervals<long long> a(lengths)。

## Include

```cpp
#include "structure/nestedintervals.hpp"

nestedintervals<long long> intervals(lengths);
intervals.align_right(v); // [0,v]を区間vの現在の右端へ揃える
intervals.align_left(v);  // [0,v]を区間vの現在の左端へ揃える
cout << intervals.count_covering(x) << '\n';
```

`set_prefix_left(v,x)`と`set_prefix_right(v,x)`なら揃える座標を直接指定できる。
`interval(i)`と`operator[]`は現在の`[left,right]`を返す。`count_covering(x)`の判定は
半開区間`[left,right)`であり、整数端点の区間が半整数`x+0.5`を含むか調べる用途にもそのまま使える。

操作後も短い区間が長い区間に含まれることを利用する。端点の揃い方が同じ連続添字をblockに圧縮し、
prefix更新で不要になったblockをstackから除く。被覆数はblockとblock内で二分探索する。

## 計算量

構築`O(n)`、区間取得と被覆数queryは`O(log n)`。prefix更新一回は最悪`O(n)`だが、
削除されたblockは将来再び削除されないため、`q`回全体で`O(q log n)`時間、`O(n+q)`領域。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `nestedintervals`

```cpp
template <class T> struct nestedintervals
```

長さ昇順の入れ子区間列をprefix左端・右端整列で管理する: nestedintervals<long long> a(lengths)。

### `nestedintervals`

```cpp
explicit nestedintervals(std::vector<T> lengths) : lengths_(std::move(lengths))
```

O(n)。厳密増加するlengthsの区間[0,lengths[i]]から構築する。

### `size`

```cpp
int size() const noexcept
```

O(1)。区間数を返す。

### `interval`

```cpp
std::pair<T, T> interval(int index) const
```

O(log n)。index番目の[left,right]を返す。

### `operator[]`

```cpp
std::pair<T, T> operator[](int index) const
```

O(log n)。a[index]はindex番目の[left,right]を返す。

### `align_left`

```cpp
void align_left(int last)
```

償却O(log n)。[0,last]の左端を現在の区間lastの左端へ揃える。

### `align_right`

```cpp
void align_right(int last)
```

償却O(log n)。[0,last]の右端を現在の区間lastの右端へ揃える。

### `set_prefix_left`

```cpp
void set_prefix_left(int last, const T& left)
```

償却O(1)、最悪O(n)。[0,last]の左端をleftへ揃える。

### `set_prefix_right`

```cpp
void set_prefix_right(int last, const T& right)
```

償却O(1)、最悪O(n)。[0,last]の右端をrightへ揃える。

### `count_covering`

```cpp
int count_covering(const T& coordinate) const
```

O(log n)。半開区間[left,right)がcoordinateを含む区間数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC428 F - Pyramid Alignment](https://atcoder.jp/contests/abc428/tasks/abc428_f):
  prefixの左端・右端整列と半整数点を含む区間数を処理する。

`verify/atcoder_abc428_f.cpp`に提出用コードを収録し、公式sampleとの一致を確認済み。


`tests/property/structure/nestedintervals.cpp`では、全区間を直接移動する愚直実装とランダム操作を比較する。
