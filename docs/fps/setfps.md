---
title: "集合冪級数"
documentation_of: //cp/fps/setfps.hpp
---

# 集合冪級数

- Header: `cp/fps/setfps.hpp`
- Symbol: `poe::setfps_exp`, `poe::setfps_log`, `poe::setfps_articulation_transform`
- Status: experimental

```cpp
#include "fps/setfps.hpp"
```

## どんな問題に使えるか

`f[mask]`を頂点集合`mask`上の構造数とみなし、互いに素な集合への分割を積とする集合冪級数を扱う。
`setfps_exp`は連結構造から任意個の連結成分、`setfps_log`は任意の構造から連結構造を取り出す。

`setfps_articulation_transform(f,v,false)`は、頂点`v`を含む係数を偏微分SPSの`log`で置換し、
関節点を禁止する連結・二重連結変換に使える。`true`で逆の`exp`変換を行う。

配列長は`2^n`。現在の実装はpivotを固定した集合分割漸化式で、`exp`,`log`が`O(3^n)`、
頂点変換が`O(3^(n-1))`、領域`O(2^n)`。`n<=16`程度を想定する。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `setfps_exp`

```cpp
template<int mod> std::vector<staticmodint<mod>> setfps_exp(const std::vector<staticmodint<mod>>& f)
```

O(3^n)。集合冪級数f（f[0]=0）のexpを返す。配列長は2冪。

### `setfps_log`

```cpp
template<int mod> std::vector<staticmodint<mod>> setfps_log(const std::vector<staticmodint<mod>>& f)
```

O(3^n)。集合冪級数f（f[0]=1）のlogを返す。配列長は2冪。

### `setfps_articulation_transform`

```cpp
template<int mod> void setfps_articulation_transform( std::vector<staticmodint<mod>>& f, int vertex, bool exponential )
```

O(3^(n-1))。vertexを含む係数だけを、その偏微分SPSのexpまたはlogで置き換える。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[FPS 24題 W - 閉路](https://atcoder.jp/contests/fps-24/tasks/fps_24_w)で、全グラフから連結グラフ、
二重頂点連結成分へ変換して、指定2頂点を含む閉路を数える。verifyコードを用意し、judge ACは未確認。
