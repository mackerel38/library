---
title: "Slope trick"
documentation_of: //cp/dp/slopetrick.hpp
---

# Slope trick

- Header: `cp/dp/slopetrick.hpp`
- Symbol: `poe::slopetrick`
- Status: experimental

## どんな問題に使えるか

一次元の区分線形凸関数をDP値として持ち、絶対値、片側距離、区間内からの遷移を加える問題に使う。
関数全体を座標ごとに保持せず、傾きが変わる位置だけを二つのheapで管理する。

```cpp
#include "dp/slopetrick.hpp"

poe::slopetrick<long long> function;
function.add_abs(a);       // |x-a|
function.add_left(a);      // max(a-x, 0)
function.add_right(a);     // max(x-a, 0)
function.shift(left, right);
std::cout << function.min() << '\n';
```

追加操作は`O(log n)`、`shift`と最小値取得は`O(1)`。`operator[]`による一点評価は`O(n log n)`。
`shift(a,b)`は`g(x)=min_{x-b<=y<=x-a} f(y)`を表し、`a<=b`が必要。

## 操作を式から選ぶ

| 操作 | 加える関数・変換 |
| --- | --- |
| `add_abs(a)` | `|x-a|` |
| `add_left(a)` | `max(a-x,0)` |
| `add_right(a)` | `max(x-a,0)` |
| `shift(a,b)` | 指定区間内から遷移した新しい凸関数 |
| `min()` | 関数全体の最小値 |
| `operator[](x)` | 一点`x`での値 |

保持できるのは凸な区分線形関数である。最大化、凹関数、二次関数には直接使えない。
`shift`の向きは式と照合し、座標差と最小値の積が`T`を越えないことを確認する。
一点評価はheapを複製するため、DPループ内で多数回呼ぶ用途には向かない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `slopetrick`

```cpp
template<class T> struct slopetrick
```

区分線形凸関数を傾き変化点で管理する: slopetrick<long long> f;

### `min`

```cpp
T min() const
```

O(1)。f.min(): 関数の最小値を返す。

### `add_constant`

```cpp
void add_constant(T value)
```

O(1)。f.add_constant(value): 関数全体へvalueを加える。

### `add_left`

```cpp
void add_left(T a)
```

O(log n)。f.add_left(a): max(a-x, 0)を加える。

### `add_right`

```cpp
void add_right(T a)
```

O(log n)。f.add_right(a): max(x-a, 0)を加える。

### `add_abs`

```cpp
void add_abs(T a)
```

O(log n)。f.add_abs(a): |x-a|を加える。

### `shift`

```cpp
void shift(T a, T b)
```

O(1)。f.shift(a, b): g(x)=min_{x-b<=y<=x-a} f(y)へ更新する。a<=bであること。

### `argmin`

```cpp
std::pair<std::optional<T>, std::optional<T>> argmin() const
```

O(1)。f.argmin(): 最小点区間の左右端を返す。無限端はnullopt。

### `operator[]`

```cpp
T operator[](T x) const
```

O(n log n)。f[x]: 点xでの関数値を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC102 C - Linear Approximation](https://atcoder.jp/contests/abc102/tasks/arc100_a)では
`A_i-i`ごとに`add_abs`し、`min()`を出力すればよい。
`verify/atcoder_abc102_c.cpp`は公式サンプル確認済み。
