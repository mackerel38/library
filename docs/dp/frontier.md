---
title: "疎なfrontier DP"
documentation_of: //cp/dp/frontier.hpp
---

# 疎なfrontier DP


## 概要

盤面を一列ずつ走査するDP、辞書・オートマトン上のDPなど、各段階で到達する状態が全状態空間より
十分少ない問題に使う。`frontierdp`は状態から値への`map`と次層への交換だけを担当し、状態の意味や
遷移は利用側に残す。同じ遷移先への値は`add`で自動的に加算される。

## 厳密な定義

- `frontierdp`: frontierdp<State, Value> dp(initial): 同じ状態の値を+=でまとめる疎なfrontier DP。

密な整数状態をすべて走査できる場合は`vector`の方が速い。最小値DPのように集約演算が加算でない場合も、
この型をそのまま使わず、値型または別の状態管理を選ぶ。

## Include

```cpp
#include "dp/frontier.hpp"

poe::frontierdp<State, mint> dp(initial_state);
dp.step([&](const State& state, const mint& ways, auto& next) {
    for (State to : transitions(state)) next.add(std::move(to), ways);
});
mint answer = dp.sum([](const State& state) { return is_goal(state); });
```

公開されている`states`を直接走査できるが、次層の構築には`step`を使う。同じ層を参照しながら更新して
意図せず遷移を繰り返す事故を防げる。

## 使い方

## 前提と計算量

- `State`は`std::map`のキーとして比較可能であること。
- `Value{}`が加算単位元で、`Value`が`+=`を持つこと。
- 現在の状態数を`S`、生成する遷移総数を`T`とすると、`step`は`O(T log S')`時間、次層の状態数を
  `S'`として`O(S')`領域を使う。
- `sum`は`O(S)`時間、`add`は`O(log S)`時間。

## よくある誤り

- 同値な状態を正規化せず、ラベル名だけ異なる状態を大量に作る。
- 状態数が指数的に増える問題へ、枝刈り条件を入れずに使う。
- 最小値を求めたいのに`add`で値を足す。`frontierdp`の集約演算は加算固定である。

連結成分ラベルの正規化には`dp/profile.hpp`を使う。各列の全bitmask列挙まで固定した盤面連結用の
shortcutは、適用範囲が狭いため`recipe/connectivityfrontier.hpp`へ分離している。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `frontierdp`

```cpp
template<class State, class Value> struct frontierdp
```

frontierdp<State, Value> dp(initial): 同じ状態の値を+=でまとめる疎なfrontier DP。

### `frontierdp`

```cpp
frontierdp() = default
```

O(1)。frontierdp(): 状態を持たないDPを作る。

### `frontierdp`

```cpp
explicit frontierdp(State initial, Value value = Value
```

O(log states)。frontierdp(initial, value): 初期状態を一つ持つDPを作る。

### `add`

```cpp
void add(const State& state, const Value& value)
```

O(log states)。dp.add(state, value): stateの値へvalueを加える。

### `add`

```cpp
void add(State&& state, const Value& value)
```

O(log states)。dp.add(move(state), value): stateを移動して値へvalueを加える。

### `step`

```cpp
template<class Transition> void step(Transition transition)
```

遷移回数をTとしてO(T log T)。dp.step(f): f(state, value, next)で次の層を作る。

### `sum`

```cpp
template<class Predicate> Value sum(Predicate pred) const
```

O(states)。dp.sum(pred): pred(state)を満たす値の総和を返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。dp.size(): 現在の状態数を返す。

### `empty`

```cpp
bool empty() const noexcept
```

O(1)。dp.empty(): 状態がなければtrueを返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[TDPC S - マス目](https://atcoder.jp/contests/tdpc/tasks/tdpc_grid)では、この状態集約を
`connectivityprofile`と組み合わせられる。直接提出する短い完成形はrecipe側の文書を参照する。
汎用`frontierdp`単体を直接指定する公開問題は未選定。API testで状態統合と層更新を確認済み。
