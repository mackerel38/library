---
title: "軸方向単位格子walk"
documentation_of: //cp/math/latticewalk.hpp
---

# 軸方向単位格子walk

## 概要

整数格子の原点から出発し、毎回どれか一つの座標を $1$ 増減するwalkについて、
指定回数後に指定座標へ着く経路数を数えます。一次元・二次元・三次元を同じ前計算で扱います。

## 厳密な定義

$d\in\{1,2,3\}$ とします。長さ $N$ のwalk

$$
p_0=(0,\ldots,0),p_1,\ldots,p_N=X
$$

で、各 $i$ について $p_i-p_{i-1}$ が標準基底ベクトル $e_j$ または $-e_j$ の
いずれかであるものの個数を返します。

一次元の経路数を $f_1(N,X)$ とすると

$$
f_1(N,X)=
\begin{cases}
\displaystyle\binom{N}{(N+|X|)/2} & (N\ge|X|,\ N\equiv X\pmod2),\\
0 & \text{otherwise}
\end{cases}
$$

です。二次元では座標変換 $u=x+y,v=x-y$ により

$$
f_2(N,X,Y)=f_1(N,X+Y)f_1(N,X-Y)
$$

となります。三次元では $z$ 軸へ使う手数 $k$ を列挙して

$$
f_3(N,X,Y,Z)=\sum_{k=0}^{N}\binom Nk
f_1(k,Z)f_2(N-k,X,Y)
$$

を計算します。

構築は $O(N)$ 時間・空間です。一次元・二次元queryは $O(1)$、三次元queryは
$O(N)$ です。

## Include

```cpp
#include "math/latticewalk.hpp"
```

## 使い方

```cpp
axiswalkcounter<998244353> counter(max_steps);
auto line = counter.count(steps, x);
auto plane = counter.count(steps, x, y);
auto space = counter.count(steps, x, y, z);
```

### `axiswalkcounter<mod>`

```cpp
explicit axiswalkcounter(int max_steps)
```

`0 <= max_steps < mod` が必要です。階乗と逆階乗を前計算するため、`mod` は素数で
なければなりません。

### `count`

```cpp
mint count(int steps, long long x) const;
mint count(int steps, long long x, long long y) const;
mint count(int steps, long long x, long long y, long long z) const;
```

`0 <= steps <= max_steps` が必要です。到達不能な座標では $0$ を返します。

## 頻出事故

- 「高々」ではなく、ちょうど `steps` 回移動します。
- 一回の移動では一軸だけが変わります。斜め移動や静止は含みません。
- 二次元の式で $x+y$ と $x-y$ の両方を使います。
- 法以上まで階乗を前計算できません。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `axiswalkcounter`

```cpp
template<int mod> struct axiswalkcounter
```

axiswalkcounter(max_steps): O(max_steps)時間・空間で軸方向単位walkを前計算する。

### `axiswalkcounter`

```cpp
explicit axiswalkcounter(int max_steps) : max_steps_(max_steps), combination_(max_steps)
```

O(max_steps)。0以上max_steps以下の歩数に必要な二項係数を前計算する。

### `count`

```cpp
mint count(int steps, long long x) const
```

O(1)。一次元で原点からxへちょうどsteps回の+-1移動で着く経路数を返す。

### `count`

```cpp
mint count(int steps, long long x, long long y) const
```

O(1)。二次元で原点から(x,y)へちょうどsteps回の軸方向単位移動で着く経路数を返す。

### `count`

```cpp
mint count(int steps, long long x, long long y, long long z) const
```

O(steps)。三次元で原点から(x,y,z)へちょうどsteps回の軸方向単位移動で着く経路数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC240 G - Teleporting Takahashi](https://atcoder.jp/contests/abc240/tasks/abc240_g)では、
三次元の目的座標をそのまま渡します。

```cpp
axiswalkcounter<998244353> counter(n);
cout << counter.count(n, x, y, z).val() << '\n';
```
