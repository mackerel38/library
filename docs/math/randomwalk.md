---
title: "ランダムウォークの合流確率・初到達期待値"
documentation_of: //cp/math/randomwalk.hpp
---

# ランダムウォークの合流確率・初到達期待値


## 概要

独立な一次元ランダムウォークが同一点へ集まる確率列と、正のstepを加えるwalkが周期剰余へ
初到達するまでの期待step数を扱う。状態$i$から上方向へは$i+1$までしか進まないMarkov連鎖の
ポテンシャル方程式も、状態番号順に解ける。

## 厳密な定義

### `symmetric_walk_meeting_probabilities`

初期位置$x_0,\ldots,x_{p-1}$から各walkが毎時刻独立に$-1,+1$を確率$1/2$で選ぶ。
返り値の$t$番目は、時刻$t$に全walkが同じ整数地点にいる確率である。
初めて集まる確率ではなく、過去に集まったかを問わない確率を返す。

### `cyclic_hitting_expectation`

正の整数stepを独立に選んで累積和へ加え、累積和が`remaining (mod period)`へ初めて到達するまでの
step回数の期待値を返す。

### `upward_skipfree_potential`

状態$0,1,\ldots,n$を持ち、状態$i<n$からの遷移先が$0,1,\ldots,i+1$に限られるMarkov連鎖で、
$h_0=0$かつ

$$
h_i=r_i+\sum_{j=0}^{i+1}P_{i,j}h_j
$$

を満たすポテンシャル$h_0,\ldots,h_n$を返す。

## Include

```cpp
#include "math/randomwalk.hpp"

using mint = poe::modint998244353;
auto meeting = poe::symmetric_walk_meeting_probabilities<998244353>(
    std::vector<int>{0, 0, 2}, maximum_time
);
mint answer = poe::cyclic_hitting_expectation(
    period, remaining,
    std::vector<mint>{mint{1} / 6, mint{1} / 6, mint{1} / 6,
                      mint{1} / 6, mint{1} / 6, mint{1} / 6}
);
auto potential = poe::upward_skipfree_potential(transition, reward);
```

## 合流確率列

全初期位置は同じparityでなければならない。人数を$p$、最大時刻を$n$とすると、時間計算量は
$O(pn+n\log p+M(n))$、領域は$O(n)$である。ここで$M(n)$は長さ$n$程度の畳み込み時間を表す。
階乗を法上で割るため、初期位置を平行移動して2で割った最大値と$n$の和が`mod`未満であることを要求する。

初期位置を平行移動して$x_i$を偶数にし、$d_i=x_i/2$とする。数列
$$
q_s=\prod_i\frac{1}{(s-d_i)!},\qquad
r_s=\prod_i\frac{1}{(s+d_i)!}
$$
を作ると、時刻$t$の合流確率は
$$
\frac{(t!)^p}{2^{pt}}[x^t]Q(x)R(x)
$$
となるため、一回の畳み込みで全時刻を求める。

## 周期剰余への初到達期待値

確率列の`i`番目はstep `i+1`の確率で、総和は1であること。末尾の確率0は自動で除く。
最大stepを`d`として`period>d`、`1<=remaining<period`を要求する。

期待値の局所漸化式を`d+1`次のaffine行列で進め、周期境界で生じる`d-1`個の未知値だけを
Gauss消去で決定する。計算量は`O(d^3 log period)`、領域は`O(d^2)`。
法上の期待値では境界方程式が一意解を持つ必要がある。

## 上方向skip-free Markov連鎖

`transition[i]`は長さ`i+2`で、`transition[i][j]`が$P_{i,j}$を表す。
$P_{i,i+1}\ne0$が必要である。各行の総和が1であることは関数内では検査しないため、通常の
確率遷移として使う場合は呼び出し側で保証する。`T`は四則演算と等値比較を持つ体の要素とする。

既知の$h_0,\ldots,h_i$を式へ代入し、唯一の未知数$h_{i+1}$を順に解く。
時間計算量は$O(n^2)$、返り値を除く追加領域は$O(n)$である。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `upward_skipfree_potential`

```cpp
template<class T> std::vector<T> upward_skipfree_potential( const std::vector<std::vector<T>>& transition, const std::vector<T>& reward )
```

O(n^2)時間・O(n)追加領域。上方向へ高々1だけ進むMarkov連鎖のh[i]=reward[i]+E[h[next]]をh[0]=0から解く。

### `symmetric_walk_meeting_probabilities`

```cpp
template<int mod> std::vector<staticmodint<mod>> symmetric_walk_meeting_probabilities( const std::vector<int>& positions, int maximum_time )
```

O(pn+n log p+M(n))。同parityの各位置から独立な±1 walkを始め、時刻0..nに全員が同一点にいる確率を返す。

### `cyclic_hitting_expectation`

```cpp
template<class T> T cyclic_hitting_expectation( long long period, long long remaining, std::vector<T> step_probability )
```

O(d^3 log period)。正のstep和がremaining mod periodへ初到達するまでの期待step数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC289 Ex - Trio](https://atcoder.jp/contests/abc289/tasks/abc289_h)では、3人の初期位置から
合流確率列$G$、全員が原点にいる状態からの再合流確率列$H$を作る。初回合流確率列$F$は
$F(x)H(x)=G(x)$を満たすので、既存`fps::div`で求める。
`verify/atcoder_abc289_h.cpp`に提出用コードを収録している。

[AtCoder ABC299 Ex - Dice Sum Infinity](https://atcoder.jp/contests/abc299/tasks/abc299_h)では、
`period=10^9`、`remaining=R`、step 1から6を各確率`1/6`として渡す。
`verify/atcoder_abc299_h.cpp`で公式sample 2件を確認済み。

[AtCoder ABC249 Ex - Dye Color](https://atcoder.jp/contests/abc249/tasks/abc249_h)では、一色の個数だけを
状態とする上方向skip-free連鎖を作り、全色のポテンシャル和から吸収までの期待操作回数を求める。
`verify/atcoder_abc249_h.cpp`に提出用コードを用意し、公式sampleを確認している。

## 検証

- `tests/api/math/randomwalk.cpp`: 一歩walkと公平サイコロの基本例
- `tests/property/math/randomwalk.cpp`: 少人数walkの直接分布DP、小周期の全剰余連立方程式、skip-free方程式のGauss消去との比較
