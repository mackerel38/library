---
title: "平方根遷移列の数え上げ"
documentation_of: //cp/math/floorsqrtchain.hpp
---

# 平方根遷移列の数え上げ

## 概要

現在値を平方根以下の正整数へ置き換える操作を繰り返すとき、できる列の個数を数えます。
最大値を一度渡して前計算すれば、各クエリには $O(1)$ で答えられます。

## 厳密な定義

$f(x)$ を、$a_1=x$ であり、すべての $i\ge 1$ について

$$
1\le a_{i+1}\le\lfloor\sqrt{a_i}\rfloor
$$

を満たす無限整数列 $(a_1,a_2,\ldots)$ の個数とします。値は有限回で $1$ になり、
その後は $1$ だけが続くため、$f(x)$ は有限です。

このライブラリは

$$
f(x)=\sum_{i=1}^{\lfloor\sqrt{x}\rfloor}f(i)
$$

をさらに二段分まとめた

$$
f(x)=\sum_{i=1}^{\lfloor x^{1/4}\rfloor}
(\lfloor\sqrt{x}\rfloor-i^2+1)f(i)
$$

を利用します。最大クエリを $X$ とすると、前計算は $O(X^{1/4})$ 時間・空間、
各 `count` は $O(1)$ です。

## Include

```cpp
#include "math/floorsqrtchain.hpp"
```

## 使い方

### `floorsqrtchaincounter`

```cpp
explicit floorsqrtchaincounter(unsigned long long max_value)
```

$1\le x\le\text{max_value}$ のすべてのクエリへ答えるための前計算を行います。
計算量は $O(\text{max_value}^{1/4})$ 時間・空間です。

### `count`

```cpp
long long count(unsigned long long x) const
```

$f(x)$ を返します。`1 <= x && x <= max_value` が必要です。計算量は $O(1)$ です。
制約内の答えが `long long` に収まる必要があります。

### `operator()`

```cpp
long long operator()(unsigned long long x) const
```

`count(x)` と同じです。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `floorsqrtchaincounter`

```cpp
struct floorsqrtchaincounter
```

floorsqrtchaincounter(max_value): O(max_value^(1/4))時間・空間で前計算する。

### `count`

```cpp
long long count(unsigned long long x) const
```

O(1)。xから始め、次を[1,floor(sqrt(現在値))]から選ぶ無限列の個数を返す。

### `operator()`

```cpp
long long operator()(unsigned long long x) const
```

O(1)。count(x)と同じ。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

```cpp
vector<unsigned long long> x(t);
for (auto& value : x) cin >> value;

floorsqrtchaincounter counter(*max_element(x.begin(), x.end()));
for (const auto value : x) cout << counter(value) << '\n';
```

[AtCoder ABC243 G - Sqrt](https://atcoder.jp/contests/abc243/tasks/abc243_g)では、
初項 $X$ から始めて平方根以下の正整数を追加し続けたときの列の種類数を求めます。
全クエリの最大値で `floorsqrtchaincounter` を構築し、各 $X$ をそのまま問い合わせられます。
