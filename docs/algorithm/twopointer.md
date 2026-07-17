---
title: "尺取り法"
documentation_of: //cp/algorithm/twopointer.hpp
---

# 尺取り法

- Header: `cp/algorithm/twopointer.hpp`
- Symbol: `poe::count_subarrays_inversions_at_most`, `poe::count_subarrays_inversions`, `poe::count_subarrays_sum_nonnegative`, `poe::count_subrectangles_sum_nonnegative`, `poe::count_subarrays_frequency_constraints`, `poe::circularthresholdwindow`, `poe::circular_threshold_windows`
- Status: experimental

## どんな問題に使えるか

要素が非負整数である列・行列について、和が指定値と一致する空でない連続区間・長方形を数える。
負の要素がある場合は単調性が壊れるので使えない。

値を座標圧縮し、Fenwick treeで区間の転倒数を管理する版もある。値の重複を許し、
転倒数が上限以下または指定値と等しい部分配列を数えられる。

```cpp
#include "algorithm/twopointer.hpp"

long long intervals = count_subarrays_sum_nonnegative(values, target);
long long rectangles = count_subrectangles_sum_nonnegative(matrix, target);
long long at_most = count_subarrays_inversions_at_most(values, limit);
long long exact = count_subarrays_inversions(values, target);
```

一次元版は「和がtarget以下」から「和がtarget-1以下」を引く尺取り法で、0が連続する場合や
`target=0`も扱える。二次元版は短い辺の上下端を固定して列和へ圧縮し、必要なら行列を転置する。

## 計算量と前提

- 一次元: `O(n)`時間、`O(1)`追加領域。
- 二次元: `O(min(h,w)^2 max(h,w))`時間、`O(max(h,w))`追加領域。
- 転倒数: `O(n log n)`時間、`O(n)`追加領域。個数は`long long`で返す。
- 値と区間和は`long long`へ変換でき、総和が`long long`に収まること。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `count_subarrays_inversions_at_most`

```cpp
template<class T> long long count_subarrays_inversions_at_most( const std::vector<T>& values, long long limit )
```

O(n log n)。転倒数がlimit以下である空でない部分配列の個数を返す。

### `count_subarrays_inversions`

```cpp
template<class T> long long count_subarrays_inversions( const std::vector<T>& values, long long target )
```

O(n log n)。転倒数がtargetと等しい空でない部分配列の個数を返す。

### `count_subarrays_sum_nonnegative`

```cpp
template<class T> long long count_subarrays_sum_nonnegative( const std::vector<T>& values, long long target )
```

O(n)。非負整数列で和がtargetとなる空でない部分配列の個数を返す。

### `count_subrectangles_sum_nonnegative`

```cpp
template<class T> long long count_subrectangles_sum_nonnegative( const std::vector<std::vector<T>>& matrix, long long target )
```

O(min(h,w)^2 max(h,w))。非負整数行列で和がtargetとなる空でない長方形の個数を返す。

### `count_subarrays_frequency_constraints`

```cpp
template<std::ranges::input_range Range, class FirstPredicate, class SecondPredicate> long long count_subarrays_frequency_constraints( const Range& values, int minimum_first, int maximum_second, FirstPredicate first_predicate, SecondPredicate second_predicate )
```

O(n)。Pをminimum以上、Qをmaximum未満含む空でない部分配列数を返す。minimum,maximum>=1。

### `circularthresholdwindow`

```cpp
template <class Sum> struct circularthresholdwindow
```

円環列の始点から和が閾値以上になる最短区間の長さと和。

### `circular_threshold_windows`

```cpp
template <class T, class U> auto circular_threshold_windows(const std::vector<T>& values, U target)
```

O(n)。非負円環列の各始点から和がtarget以上になる最短prefixを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AtCoder ABC461 D - Count Subgrid Sum = K](https://atcoder.jp/contests/abc461/tasks/abc461_d):
  0/1行列を渡すだけで長方形数を求められる。

`verify/atcoder_abc461_d.cpp`に提出用コードを用意し、公式sampleを確認済み。
API testと小さい非負行列の全長方形列挙との固定seed比較を実施する。

- [AtCoder ABC452 F - Interval Inversion Count](https://atcoder.jp/contests/abc452/tasks/abc452_f):
  `count_subarrays_inversions(P, K)`を直接使える。

`verify/atcoder_abc452_f.cpp`に提出用コードを用意し、公式sampleを確認済み。

## 二種類の出現数制約

`count_subarrays_frequency_constraints(values, minimum_first, maximum_second, P, Q)`は、
`P(value)`を満たす要素を`minimum_first`個以上、`Q(value)`を満たす要素を`maximum_second`個未満含む
空でない部分配列を数える。二つのpredicateは重なってもよい。

各左端について両条件が初めて閾値へ達する右端を独立した尺取りで求めるため、`O(n)`時間・`O(n)`領域。
両閾値は1以上とする。

[AtCoder ABC430 C - Truck Driver](https://atcoder.jp/contests/abc430/tasks/abc430_c)では、
`P(c)=c=='a'`、`Q(c)=c=='b'`として直接使える。
`verify/atcoder_abc430_c.cpp`で公式2 sampleを確認済み。

## 円環上の閾値prefix

`circular_threshold_windows(values, target)`は、非負円環列の各始点について、和が`target`以上に
初めてなるprefixの`length`と`sum`を返す。`target>0`かつ一周の総和以上でないことを前提とする。

```cpp
auto windows = circular_threshold_windows(frequency, required);
cout << windows[start].length << ' ' << windows[start].sum << '\n';
```

右端が始点とともに単調に進むため、全始点を`O(n)`時間・`O(n)`領域で処理する。

[AtCoder ABC429 D - On AtCoder Conference](https://atcoder.jp/contests/abc429/tasks/abc429_d)では、
人がいる地点だけへ圧縮した円環頻度列に使い、同じ結果になる開始位置の個数を掛ける。
`verify/atcoder_abc429_d.cpp`で公式sample 2件を確認済み。
