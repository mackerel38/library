---
title: "OGF・EGFによるグループ分配"
documentation_of: //cp/fps/combinatorial.hpp
---

# OGF・EGFによるグループ分配

- Header: `cp/fps/combinatorial.hpp`
- Symbol: `poe::groupdistributioncounts`, `poe::divisible_group_counts`,
  `poe::labeled_tree_degree_weight_sum`, `poe::count_labeled_trees_with_degrees`,
  `poe::adjacentdistinctwordcounts`, `poe::adjacent_distinct_words`
- Status: experimental

## どんな問題に使えるか

`n`個の要素を複数の区別される箱・色・グループへ分配し、各groupの要素数を`period[i]`の倍数に
制限する問題を扱う。要素を区別しない通常型母関数と、各要素を区別する指数型母関数を同時に構築する。

```cpp
#include "fps/combinatorial.hpp"

auto answer = poe::divisible_group_counts<998244353>(n, {a, b, c});
cout << answer.ordinary << '\n';
cout << answer.labeled << '\n';
```

`ordinary`はgroupごとの個数だけで方法を区別する。`labeled`は各要素がどのgroupへ入ったかで区別する。
group自体は入力順で区別される。各periodは正、`0<=n<mod`が必要。

group数を`k`として、疎な因子構築に`O(kn)`、多項式積に概ね`O(M(n) log k)`時間を使う。
領域は積木を含め`O(kn)`が上限となる。

### ラベル付き木の次数制約

```cpp
auto count = poe::count_labeled_trees_with_degrees<998244353>(n, allowed_degrees);
```

全頂点の次数が`allowed_degrees`のいずれかであるラベル付き木を
`O(n+M(n) log n)`で数える。より一般に、

```cpp
auto sum = poe::labeled_tree_degree_weight_sum<998244353>(n, degree_weight);
```

とすると、各木`T`へ`product(degree_weight[deg_T(v)])`を掛けた重み総和を返す。
Prüfer列で頂点の出現回数が次数より1小さいことを使い、次数別重みのEGFの`n`乗へ帰着する。
`2<=n<mod`、`degree_weight.size()>=n`が必要。

### 同じ記号が隣接しない重み付きword

```cpp
auto words = poe::adjacent_distinct_words<998244353>(maximum_sum, symbol_weights);
cout << words.count[target] << '\n';
cout << words.length_sum[target] << '\n';
```

各記号に正の整数重みを与え、隣接する記号が異なるwordを重み和ごとに数える。
`count[s]`はword数、`length_sum[s]`は全wordの長さ総和。空wordは重み0の個数1・長さ0。
同じ重みを持つ別記号は入力中の別要素として区別される。

一記号を同じ値で連続させたrunへ包除を適用し、
`g=sum z/(1+z)`、`g'=sum z/(1+z)^2`から
個数`1/(1-g)`、長さ総和`g'/(1-g)^2`を構築する。
計算量は`O(sum(maximum_sum/weight)+M(n) log n)`。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `groupdistributioncounts`

```cpp
template<class T> struct groupdistributioncounts
```

divisible_group_countsの結果。ordinaryは要素を区別せず、labeledは各要素を区別する。

### `adjacentdistinctwordcounts`

```cpp
template<int mod> struct adjacentdistinctwordcounts
```

adjacent_distinct_wordsの結果。countとlength_sumを重み和ごとのFPSで持つ。

### `divisible_group_counts`

```cpp
template<int mod> groupdistributioncounts<staticmodint<mod>> divisible_group_counts( int element_count, const std::vector<int>& periods )
```

O(kn+M(n)log k)。n要素を各groupの大きさがperiodの倍数となるよう分配する通り数を返す。

### `labeled_tree_degree_weight_sum`

```cpp
template<int mod> staticmodint<mod> labeled_tree_degree_weight_sum( int vertex_count, const std::vector<staticmodint<mod>>& degree_weight )
```

O(M(n)log n)。各頂点vにdegree_weight[deg(v)]を掛けたn頂点ラベル付き木の重み総和を返す。

### `count_labeled_trees_with_degrees`

```cpp
template<int mod> staticmodint<mod> count_labeled_trees_with_degrees( int vertex_count, const std::vector<int>& allowed_degrees )
```

O(n+M(n)log n)。全頂点の次数がallowed_degreesのいずれかであるn頂点ラベル付き木を数える。

### `adjacent_distinct_words`

```cpp
template<int mod> adjacentdistinctwordcounts<mod> adjacent_distinct_words( int maximum_sum, const std::vector<int>& symbol_weights )
```

O(sum(max_sum/weight)+M(n)log n)。同じ記号が隣接しないwordの個数と長さ総和を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC422 G - Balls and Boxes](https://atcoder.jp/contests/abc422/tasks/abc422_g)で、
三つの箱の個数周期`A,B,C`を渡し、区別なし・区別ありの答えを同時に得る。
`verify/atcoder_abc422_g.cpp`で公式sampleを確認済み。

[AtCoder ABC303 Ex - Constrained Tree Degree](https://atcoder.jp/contests/abc303/tasks/abc303_h)
では、許される次数集合を`count_labeled_trees_with_degrees`へ直接渡す。
`verify/atcoder_abc303_h.cpp`で公式sample 3件を確認済み。

[AtCoder ABC297 Ex - Diff Adjacent](https://atcoder.jp/contests/abc297/tasks/abc297_h)
では重み1から`N`の記号を渡し、`length_sum[N]`を使う。
`verify/atcoder_abc297_h.cpp`で公式sample 3件を確認済み。

## 検証

- `tests/api/fps/combinatorial.cpp`: 基本例と空group
- `tests/property/fps/combinatorial.cpp`: 小さい全個数tupleの列挙と多項係数による愚直値との比較
- `tests/api/fps/labeledtree.cpp`: 次数集合、空解、次数別重みの基本例
- `tests/property/fps/labeledtree.cpp`: 小さい全Prüfer列との重み総和比較
- `tests/api/fps/adjacentdistinct.cpp`: 同重み別記号と正整数compositionの基本例
- `tests/property/fps/adjacentdistinct.cpp`: 最後の記号を持つ愚直DPとの全係数比較
