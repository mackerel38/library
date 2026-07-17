---
title: "離散対数"
documentation_of: //cp/math/discretelog.hpp
---

# 離散対数

- Header: `cp/math/discretelog.hpp`
- Symbol: `poe::discrete_log`
- Status: experimental

```cpp
#include "math/discretelog.hpp"
```

`discrete_log(a,b,mod)`は`a^x=b (mod mod)`を満たす最小の`x>=0`を返し、存在しなければ`-1`。
法と底が互いに素でない場合も、共通因子を順に除いてからbaby-step giant-stepへ帰着する。

計算量・領域は`O(sqrt(mod))`。hash tableを使うため、法が64bit上限に近い問題には向かない。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `discrete_log`

```cpp
inline long long discrete_log(long long base, long long target, long long mod)
```

O(sqrt(mod))時間・領域。discrete_log(a,b,mod): a^x=b(mod mod)となる最小x>=0。なければ-1。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[ABC270 G - Sequence in mod P](https://atcoder.jp/contests/abc270/tasks/abc270_g)の一般ケースは、
一次変換で数列を等比数列へ移して離散対数を求める。Library Checkerのverifyコードを用意し、
小さい法の全探索照合済み・。
