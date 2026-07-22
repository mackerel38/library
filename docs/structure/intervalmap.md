---
title: "区間定数写像"
documentation_of: //cp/structure/intervalmap.hpp
---

# 区間定数写像

## 概要

巨大な一次元区間上の値を、同じ値が続く極大区間だけに圧縮して管理する。区間代入を行うたびに、
上書きされる前の区間片を取得できる。座標の幅が巨大でも、これまでの代入回数が少ない場合に使える。

## 厳密な定義

初期領域$[L,R)$の各座標に同じ値を持つ写像を管理する。
`assign(left,right,value)`は全ての$x\in[left,right)$について$f(x)\leftarrow value$とし、更新前の
$f$が一定である極大区間へ分割した

$$
[l_0,r_0),[l_1,r_1),\ldots,[l_{k-1},r_{k-1})
$$

と各区間の更新前の値を返す。返される区間は互いに素で、昇順に並び、その和集合は
$[left,right)$と一致する。更新後は隣接する同値区間を自動的に併合する。

## Include

```cpp
#include "structure/intervalmap.hpp"
```

## 使い方

```cpp
poe::intervalmap<long long, int> color(0, 1000000000000000000LL, 0);
for (auto old : color.assign(10, 20, 3)) {
    // old.left, old.right, old.valueは上書き前の区間片
}
int value = color[15]; // 3
```

### `intervalmapsegment`

```cpp
template<class Coordinate, class Value>
struct intervalmapsegment {
    Coordinate left;
    Coordinate right;
    Value value;
};
```

半開区間`[left,right)`と、その区間で一定な値`value`を表す。

### `intervalmap`

```cpp
template<class Coordinate, class Value>
intervalmap(Coordinate left, Coordinate right, Value initial);
```

`[left,right)`全体の値を`initial`として構築する。`left < right`が必要である。
`Coordinate`には全順序、`Value`にはコピーと`==`が必要である。

### `assign`

```cpp
vector<segment> assign(Coordinate left, Coordinate right, const Value& value);
```

`[left,right)`を`value`へ代入し、更新前の区間片を返す。空区間なら何も変更せず空vectorを返す。
置換される区間片数を$k$、現在の区間数を$n$として$O((k+1)\log n)$時間、返り値に$O(k)$領域を使う。
一度消えた区間は、後の代入で新たに作られない限り再び走査されない。

### `operator[]`

```cpp
const Value& operator[](Coordinate coordinate) const;
```

座標`coordinate`の値への`const`参照を$O(\log n)$で返す。`left <= coordinate < right`が必要である。

### `segment_count`

```cpp
int segment_count() const noexcept;
```

現在の極大な定数区間数を$O(1)$で返す。

## 使用例

[AtCoder ABC255 Ex - Range Harvest Query](https://atcoder.jp/contests/abc255/tasks/abc255_h)では、
各木を最後に収穫した日を値として管理する。収穫区間を現在の日へ代入し、返された各区間片について
木番号の等差数列和と前回日からの経過日数を掛ければ、巨大な木番号の範囲を列挙せず収穫量を求められる。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `intervalmapsegment`

```cpp
template<class Coordinate, class Value> struct intervalmapsegment
```

intervalmapの区間片。半開区間[left,right)の値がvalueである。

### `intervalmap`

```cpp
template<class Coordinate, class Value> struct intervalmap
```

区間定数な写像: intervalmap<long long,int> data(left,right,initial); assignは置換前の区間片を返す。

### `intervalmap`

```cpp
intervalmap(Coordinate left, Coordinate right, Value initial) : domain_left_(left), domain_right_(right)
```

O(1)。[left,right)をinitialで初期化する。left<right。

### `assign`

```cpp
std::vector<segment> assign(Coordinate left, Coordinate right, const Value& value)
```

O((k+1)log n)。[left,right)をvalueへ代入し、置換前のk個の区間片を返す。

### `operator[]`

```cpp
const Value& operator[](Coordinate coordinate) const
```

O(log n)。coordinateを含む区間の値へのconst参照を返す。

### `segment_count`

```cpp
int segment_count() const noexcept
```

O(1)。現在の極大な定数区間数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->
