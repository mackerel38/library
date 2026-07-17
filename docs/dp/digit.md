---
title: "桁DP"
documentation_of: //cp/dp/digit.hpp
---

# 桁DP

- Header: `cp/dp/digit.hpp`
- Symbol: `poe::digitdp`, `poe::digitdpresult`, `poe::leadingzero`
- Status: experimental

## どんな問題に使えるか

`0`以上、指定した上限以下の整数を桁ごとに走査し、剰余、桁和、出現状態、オートマトン状態などを
集計する問題に使う。上限への一致状態`tight`はライブラリが管理し、利用側は一桁の状態遷移だけを書く。

```cpp
#include "dp/digit.hpp"

auto result = poe::digitdp<mint>(
    upper, 0,
    [&](int state, int digit, int position) {
        return (state + digit) % divisor;
    },
    poe::leadingzero::use
);
mint answer = result.sum([](int state, bool started) { return state == 0; });
```

遷移は次状態、または不正遷移を落とす`optional<状態>`を返す。`leadingzero::use`は上位の0も通常の
桁として遷移し、`ignore`は最初の非0桁まで状態を変えない。`ignore`では数0だけ`started=false`になる。
十進文字列のほか、任意基数の桁列と基数も指定できる。

状態数を`S`、桁数を`L`、基数を`B`として`O(LBS log S)`時間、`O(S)`領域。

## 使う前の確認と典型的な誤り

- 上限は両端を含む`0 <= x <= upper`である。区間`[L,R]`は`count(R)-count(L-1)`へ直す。
- `leadingzero::ignore`では数0だけ`started=false`になる。0を答えに含めるか明示する。
- 「0という桁」と「まだ数が始まっていない」を同一視しない。
- 不正遷移を捨てる場合は`optional<State>`を返す。
- `State`は`map`のキーとして比較可能であること。密な整数状態なら専用`vector` DPの方が速い。

状態が文字列オートマトンなら`string`カテゴリの部品と組み合わせる。単に固定遷移の長さ`L`列を
数えるだけなら、行列累乗の方が適する場合もある。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `leadingzero`

```cpp
enum class leadingzero
```

桁DPで先頭の0を通常の桁として使うか、数が始まるまで無視するかを指定する。

### `digitdpstate`

```cpp
template<class State> struct digitdpstate
```

digitdpの終端状態。started=falseはleadingzero::ignoreで数0だけを表す。

### `digitdpresult`

```cpp
template<class State, class Value> struct digitdpresult
```

digitdpの状態別集計結果。

### `sum`

```cpp
template<class Predicate> Value sum(Predicate pred) const
```

O(states)。result.sum(pred): pred(state, started)を満たす値の総和を返す。

### `digitdp`

```cpp
template<class Value, class State, class Transition> digitdpresult<State, Value> digitdp( const std::vector<int>& digits, int base, State initial, Transition transition, leadingzero zeros = leadingzero::ignore, Value one = Value
```

O(length * base * states * log states)。digitdp(digits, base, initial, transition): 0以上upper以下を集計する。
transition(state, digit, position)は次状態またはoptional<状態>を返す。

### `digitdp`

```cpp
template<class Value, class State, class Transition> digitdpresult<State, Value> digitdp( std::string_view upper, State initial, Transition transition, leadingzero zeros = leadingzero::ignore, Value one = Value
```

O(length * 10 * states * log states)。digitdp(upper, initial, transition): 十進数の0以上upper以下を集計する。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[EDPC S - Digit Sum](https://atcoder.jp/contests/dp/tasks/dp_s)へ直接使える。
`verify/atcoder_dp_s.cpp`は公式サンプル確認済み。
