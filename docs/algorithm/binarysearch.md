---
title: "二分探索"
documentation_of: //cp/algorithm/binarysearch.hpp
---

# 二分探索

## Include

```cpp
#include "algorithm/binarysearch.hpp"
```

単調な述語の境界を、overflowしにくい`std::midpoint`で探索する。

- `first_true(left, right, predicate)`: `[left,right)`でfalseからtrueへ変わる最初
- `last_true(left, right, predicate)`: `[left,right)`でtrueからfalseへ変わる最後
- `binary_search_real(false_value, true_value, predicate, iterations)`: 実数のfalse/true境界
- `minimize_convex_real(left, right, function, iterations)`: 実数凸関数の最小点と最小値

整数版は候補がなければ`nullopt`を返すため、`left-1`や`right`を番兵として値域外へ置く必要がない。
答えの存在が問題で保証される場合は`.value()`で短く取得できる。計算量は整数版が
`O(log(right-left))`、実数版が`O(iterations)`。

述語の単調方向を逆にしない。`first_true`はfalse→true、`last_true`はtrue→falseである。
実数版では両端がそれぞれfalse・trueであることを利用側で保証する。
凸関数最小化は最小点が指定閉区間に存在することを保証し、平坦な最小区間も許す。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `first_true`

```cpp
template<std::integral T, class Predicate> std::optional<T> first_true(T left, T right, Predicate predicate)
```

O(log(right-left))。falseからtrueへ変わる[left,right)内の最初を返し、なければnullopt。

### `last_true`

```cpp
template<std::integral T, class Predicate> std::optional<T> last_true(T left, T right, Predicate predicate)
```

O(log(right-left))。trueからfalseへ変わる[left,right)内の最後を返し、なければnullopt。

### `binary_search_real`

```cpp
template<std::floating_point T, class Predicate> T binary_search_real(T false_value, T true_value, Predicate predicate, int iterations = 100)
```

O(iterations)。falseからtrueへ変わる実数述語を二分探索し、true側の近似境界を返す。

### `realminimum`

```cpp
template<std::floating_point T> struct realminimum
```

minimize_convex_realの近似結果。argumentでvalueを取る。

### `minimize_convex_real`

```cpp
template<std::floating_point T, class Function> realminimum<T> minimize_convex_real( T left, T right, Function function, int iterations = 100 )
```

O(iterations)。[left,right]上の凸関数を三分探索し、最小点と最小値の近似を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder Beginner Contest 463 D - Maximize the Gap](https://atcoder.jp/contests/abc463/tasks/abc463_d)。
布を右へ`extra`延長してもinterval schedulingで`K`枚選べるかは、`extra`に対してtrue→falseになる。

```cpp
auto extra = last_true(0LL, 1'000'000'001LL, feasible);
cout << (extra ? *extra + 1 : -1) << '\n';
```

`verify/atcoder_abc463_d.cpp`に提出用コードを用意し、選択可能・不可能の小ケースを確認済み。
公式サンプルと。

[AtCoder ABC314 Ex - Disk and Segments](https://atcoder.jp/contests/abc314/tasks/abc314_h)では、
全線分への距離の最大値が二変数凸関数になるため、`x`固定の`y`最小化を内側、
その結果の`x`最小化を外側の`minimize_convex_real`で行う。
`verify/atcoder_abc314_h.cpp`で公式sampleを確認済み。
