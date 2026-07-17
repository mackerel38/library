---
title: "Mo's algorithm"
documentation_of: //cp/structure/mo.hpp
---

# Mo's algorithm

- Status: experimental
- Header: `cp/structure/mo.hpp`
- Symbol: `poe::Mo`, `poe::Mo2d`

## Include

```cpp
#include "structure/mo.hpp"
```

## 使い方

更新のない列に対する多数の区間queryを並べ替え、現在区間の左右端を一要素ずつ動かす。
型名`Mo`は競技プログラミングで定着した短い固有名なので、例外的に大文字を使う。

```cpp
Mo mo(n);
for (int i = 0; i < q; ++i) answer_id[i] = mo.add(left[i], right[i]);
mo.run(
    [&](int index) { add(values[index]); },
    [&](int index) { remove(values[index]); },
    [&](int id) { answers[id] = current; }
);
```

左右で処理が異なる場合は、5 callback版の
`run(add_left, add_right, remove_left, remove_right, answer)`を使う。
どちらも`answer(id)`の`id`は`add(left, right)`が返した登録順の番号である。

## 適用条件

- queryの順番を入れ替えてよいoffline問題である。
- 区間へ一要素を追加・削除する処理が十分軽い。
- 区間更新がある場合はこの基本版では扱わない。

端点移動回数は標準的に`O((n+q)sqrt(q))`程度、callbackが`O(f)`なら全体へ`f`を掛ける。
queryと並べ替え用領域は`O(q)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `Mo`

```cpp
struct Mo
```

静的区間queryを端点一つの更新へ分解する: Mo mo(n); mo.add(l, r); mo.run(...);

### `Mo`

```cpp
explicit Mo(int n) : n_(n)
```

O(1)。長さnの列に対するquery集合を作る。

### `add`

```cpp
int add(int left, int right)
```

O(1)。半開区間[left, right)を登録し、answerの添字を返す。

### `run`

```cpp
template <class AddLeft, class AddRight, class RemoveLeft, class RemoveRight, class Answer> void run(AddLeft add_left, AddRight add_right, RemoveLeft remove_left, RemoveRight remove_right, Answer answer) const
```

O((n+q)sqrt(q))回程度の端点更新。左右別の追加・削除とanswer(id)を呼ぶ。

### `run`

```cpp
template <class Add, class Remove, class Answer> void run(Add add, Remove remove, Answer answer) const
```

O((n+q)sqrt(q))回程度の端点更新。左右共通のadd(index), remove(index)を使う。

### `size`

```cpp
int size() const noexcept
```

O(1)。登録済みquery数を返す。

### `Mo2d`

```cpp
struct Mo2d
```

offlineの二次元点queryを座標一つの更新へ分解する: Mo2d mo(x_limit, y_limit);。

### `Mo2d`

```cpp
Mo2d(int x_limit, int y_limit) : x_limit_(x_limit), y_limit_(y_limit)
```

O(1)。0<=x<=x_limit, 0<=y<=y_limitのquery集合を作る。

### `add`

```cpp
int add(int x, int y)
```

O(1)。点(x,y)を登録し、answerの添字を返す。

### `run`

```cpp
template<class IncreaseX, class DecreaseX, class IncreaseY, class DecreaseY, class Answer> void run(IncreaseX increase_x, DecreaseX decrease_x, IncreaseY increase_y, DecreaseY decrease_y, Answer answer) const
```

O((max(x)+q)sqrt(q))回程度の座標更新。(0,0)から各query点を巡回する。

### `size`

```cpp
int size() const noexcept
```

O(1)。登録済みquery数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC242 G - Range Pairing Query](https://atcoder.jp/contests/abc242/tasks/abc242_g)用の
`verify/atcoder_abc242_g.cpp`を収録した。

値`x`の現在個数を`count[x]`とすると、追加前が奇数ならpair数を1増やし、
削除前が偶数ならpair数を1減らすだけで各queryを処理できる。

## 二次元点版

`Mo2d(x_limit, y_limit)`は、区間ではないoffline二次元点queryを並べ、状態を`(0,0)`から
上下左右へ一座標ずつ動かす。`run(increase_x, decrease_x, increase_y, decrease_y, answer)`の
各移動callbackには移動前の座標が渡る。例えば`increase_x(old_x)`は`x=old_x`から`old_x+1`への移動である。

```cpp
Mo2d mo(max_x, max_y);
for (auto [x, y] : queries) mo.add(x, y);
mo.run(increase_x, decrease_x, increase_y, decrease_y,
       [&](int id) { answers[id] = current; });
```

二変数の値を四方向すべて`O(1)`で更新でき、query順を変えてよい場合に使う。
[ABC463 G - Random Walk Distance](https://atcoder.jp/contests/abc463/tasks/abc463_g)では、
二項係数のprefix和`f(N,M)`と重み付きprefix和`g(N,M)`を二次元Moでまとめて求められる。
`verify/atcoder_abc463_g.cpp`は公式サンプルとの一致を確認済み。

## 検証

- `tests/api/structure/mo.cpp`: 共通callback版と回答順
- `tests/property/structure/mo.cpp`: ランダム区間和を愚直計算と比較
