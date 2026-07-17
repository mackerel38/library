---
title: "P再帰列"
documentation_of: //cp/fps/holonomic.hpp
---

# P再帰列

- Header: `cp/fps/holonomic.hpp`
- Symbol: `poe::holonomic_sequence`
- Status: experimental

```cpp
#include "fps/holonomic.hpp"
```

## どんな問題に使えるか

添字の多項式を係数に持つ線形漸化式
`c(i,0)a[i]+c(i,1)a[i-1]+...+c(i,r)a[i-r]=0`から列を順に復元する。
有理型・代数型・D-finiteな母関数から得た漸化式の実装を、特異点処理と分離できる。

先頭係数が0になる添字や初期値は`singular(index,values)`から`optional<T>`を返す。
通常の添字では`nullopt`を返す。計算量`O(nr)`、領域`O(n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `holonomic_sequence`

```cpp
template<class T, class Coefficient, class Singular> std::vector<T> holonomic_sequence( int size, int order, Coefficient coefficient, Singular singular )
```

O(nr)。sum_{d=0}^r coefficient(i,d)*a[i-d]=0を満たすP再帰列を列挙する。
singular(i,a)がoptionalの値を返した添字では、その値を漸化式より優先する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[FPS 24題 U - 録画機](https://atcoder.jp/contests/fps-24/tasks/fps_24_u)で、低次の二変数有理式から
得られる5項P再帰を列挙する。verifyコードを用意し、サンプル確認済み・judge ACは未確認。
