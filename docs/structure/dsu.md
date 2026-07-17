---
title: "DSU"
documentation_of: //cp/structure/dsu.hpp
---

# DSU

## 概要

通常・重み付き・parity・rollback・成分辺数集約付きの素集合データ構造を扱う。

## 厳密な定義

- `dsu`: 素集合を併合して管理する: dsu uf(n); 併合・判定はならしO(alpha(n))。
- `paritydsu`: XOR差を保つDSU: paritydsu uf(n); uf.merge(a,b,1)で異なる二色を要求する。
- `weighted_dsu`: 頂点間の加法的な差を保つDSU: weighted_dsu<long long> uf(n); 各操作ならしO(alpha(n))。
- `rollbackdsu`: 併合を巻き戻せるDSU: rollbackdsu uf(n); 併合・判定O(log n)、undo O(1)。
- `rollbackgraphdsu`: 成分の(頂点数,辺数)スコア総和を保って辺追加を巻き戻す: rollbackgraphdsu uf(n, score)。


## Include

```cpp
#include "structure/dsu.hpp"
```

## 選び方

| 型 | 用途 | `merge` | `leader` / `same` |
| --- | --- | --- | --- |
| `dsu` | 通常の集合併合 | ならし`O(alpha(n))` | ならし`O(alpha(n))` |
| `weighted_dsu<T>` | 頂点間の加法的な差も保持 | ならし`O(alpha(n))` | ならし`O(alpha(n))` |
| `paritydsu` | 頂点間の0/1 XOR差と二色の人数を保持 | ならし`O(alpha(n))` | ならし`O(alpha(n))` |
| `rollbackdsu` | 追加した辺を後から取り消す | `O(log n)` | `O(log n)` |
| `rollbackgraphdsu` | 成分の頂点数・辺数によるスコアを保持して取り消す | `O(log n)` | `O(log n)` |

全型が`same`、`leader`、`size`、`count`、代表元を返す`operator[]`を持つ。
通常の`dsu`だけは`groups()`で全連結成分も列挙する。

### 重み付きDSU

```cpp
weighted_dsu<long long> uf(n);
bool ok = uf.merge(a, b, difference);  // potential(b)-potential(a)=difference
long long answer = uf.diff(a, b);
```

`merge`は新しい式が既存の式と矛盾するときだけ`false`を返す。同じ式を再度追加した場合は`true`。
`diff(a, b)`は同じ集合に属するときだけ使える。`T`には加減算、`+=`、等値比較が必要。

### rollback DSU

```cpp
rollbackdsu uf(n);
int time = uf.snapshot();
uf.merge(a, b);
uf.rollback(time);
```

`merge`一回につき履歴も一件増えるため、併合済み頂点への`merge`も`undo()`一回で対応する。
経路圧縮を使わずunion by sizeだけを使うので、探索は`O(log n)`となる。

### 成分スコア付きrollback DSU

```cpp
rollbackgraphdsu uf(n, [](int vertices, int edges) {
    return min(vertices, edges);
});
int time = uf.snapshot();
uf.add_edge(a, b);
cout << uf.value() << '\n';
uf.rollback(time);
```

`rollbackgraphdsu`は自己辺・多重辺を含む無向辺を追加し、各成分の
`score(頂点数, 辺数)`の総和を`value()`で返す。`size(v)`、`edges(v)`、`same`、
`leader`、`count`も取得できる。`add_edge`は同一成分内の辺でも必ず履歴を一件追加する。
`score`は同じ引数へ常に同じ値を返し、戻り値型は加減算とデフォルト構築が可能であること。

### parity DSU

```cpp
paritydsu uf(n);
uf.merge(u, v, 1);              // uとvを異なる色にする
cout << uf.minimum_color_size(); // 各成分を反転できるときの少ない側の総数、矛盾時-1
```

`merge(a,b,d)`は`parity(a) xor parity(b)=d`を追加する。`diff`、`color_sizes`、
`consistent`も使える。オンライン辺追加に対する二部グラフ判定と、各成分の二色人数集約を同時に行える。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `dsu`

```cpp
struct dsu
```

素集合を併合して管理する: dsu uf(n); 併合・判定はならしO(alpha(n))。

### `dsu`

```cpp
explicit dsu(int n) : component_count_(n), parent_or_size_(n, -1)
```

O(n)。n頂点を互いに異なる集合として作る。

### `merge`

```cpp
int merge(int a, int b)
```

ならしO(alpha(n))。aとbを併合し、新しい代表元を返す。

### `same`

```cpp
bool same(int a, int b)
```

ならしO(alpha(n))。aとbが同じ集合ならtrueを返す。

### `leader`

```cpp
int leader(int a)
```

ならしO(alpha(n))。aが属する集合の代表元を返す。

### `size`

```cpp
int size(int a)
```

ならしO(alpha(n))。aが属する集合の要素数を返す。

### `operator[]`

```cpp
int operator[](int a)
```

ならしO(alpha(n))。uf[a]はaの代表元を返す。

### `count`

```cpp
int count() const noexcept
```

O(1)。現在の連結成分数を返す。

### `groups`

```cpp
std::vector<std::vector<int>> groups()
```

O(n)。全連結成分を頂点番号の昇順で返す。

### `paritydsu`

```cpp
struct paritydsu
```

XOR差を保つDSU: paritydsu uf(n); uf.merge(a,b,1)で異なる二色を要求する。

### `paritydsu`

```cpp
explicit paritydsu(int n) : parent_or_size_(n, -1), xor_to_parent_(n), counts_(n,
```

O(n)。全頂点のparityを0とした独立成分を作る。

### `merge`

```cpp
bool merge(int a, int b, int difference)
```

ならしO(alpha(n))。parity(a) xor parity(b)=differenceを追加し、矛盾しなければtrue。

### `same`

```cpp
bool same(int a, int b)
```

ならしO(alpha(n))。aとbが同じ成分ならtrueを返す。

### `leader`

```cpp
int leader(int a)
```

ならしO(alpha(n))。aの代表元を返す。

### `parity`

```cpp
int parity(int a)
```

ならしO(alpha(n))。代表元に対するaのparityを返す。

### `diff`

```cpp
int diff(int a, int b)
```

ならしO(alpha(n))。parity(a) xor parity(b)を返す。同じ成分であること。

### `color_sizes`

```cpp
std::array<int, 2> color_sizes(int a)
```

ならしO(alpha(n))。aの成分の二色の頂点数を返す。

### `consistent`

```cpp
bool consistent() const noexcept
```

O(1)。追加した全制約が矛盾しないならtrueを返す。

### `minimum_color_size`

```cpp
int minimum_color_size() const noexcept
```

O(1)。各成分を反転できるときの色1の最小総数。矛盾時は-1。

### `weighted_dsu`

```cpp
template <class T> struct weighted_dsu
```

頂点間の加法的な差を保つDSU: weighted_dsu<long long> uf(n); 各操作ならしO(alpha(n))。

### `weighted_dsu`

```cpp
explicit weighted_dsu(int n) : component_count_(n), parent_or_size_(n, -1), weight_(n)
```

O(n)。各頂点のpotentialを0として、互いに異なる集合を作る。

### `merge`

```cpp
bool merge(int a, int b, const T& difference)
```

ならしO(alpha(n))。potential(b)-potential(a)=differenceを追加する。矛盾時だけfalse。

### `same`

```cpp
bool same(int a, int b)
```

ならしO(alpha(n))。aとbが同じ集合ならtrueを返す。

### `leader`

```cpp
int leader(int a)
```

ならしO(alpha(n))。aの代表元を返す。

### `potential`

```cpp
T potential(int a)
```

ならしO(alpha(n))。代表元を基準としたaのpotentialを返す。

### `diff`

```cpp
T diff(int a, int b)
```

ならしO(alpha(n))。potential(b)-potential(a)を返す。同じ集合であること。

### `size`

```cpp
int size(int a)
```

ならしO(alpha(n))。aが属する集合の要素数を返す。

### `operator[]`

```cpp
int operator[](int a)
```

ならしO(alpha(n))。uf[a]でaの代表元を返す。

### `count`

```cpp
int count() const noexcept
```

O(1)。現在の連結成分数を返す。

### `rollbackdsu`

```cpp
struct rollbackdsu
```

併合を巻き戻せるDSU: rollbackdsu uf(n); 併合・判定O(log n)、undo O(1)。

### `rollbackdsu`

```cpp
explicit rollbackdsu(int n) : component_count_(n), parent_or_size_(n, -1)
```

O(n)。n頂点を互いに異なる集合として作る。

### `merge`

```cpp
bool merge(int a, int b)
```

O(log n)。aとbを併合する。実際に併合したならtrueを返す。

### `same`

```cpp
bool same(int a, int b) const
```

O(log n)。aとbが同じ集合ならtrueを返す。

### `leader`

```cpp
int leader(int a) const
```

O(log n)。aの代表元を返す。

### `size`

```cpp
int size(int a) const
```

O(log n)。aが属する集合の要素数を返す。

### `operator[]`

```cpp
int operator[](int a) const
```

O(log n)。uf[a]でaの代表元を返す。

### `count`

```cpp
int count() const noexcept
```

O(1)。現在の連結成分数を返す。

### `snapshot`

```cpp
int snapshot() const noexcept
```

O(1)。現在位置をrollback用の値として返す。

### `undo`

```cpp
void undo()
```

O(1)。直前のmerge一回分を取り消す。履歴があること。

### `rollback`

```cpp
void rollback(int snapshot)
```

合計O(取り消すmerge数)。snapshot()で得た時点まで戻す。

### `rollbackgraphdsu`

```cpp
template<class Score> struct rollbackgraphdsu
```

成分の(頂点数,辺数)スコア総和を保って辺追加を巻き戻す: rollbackgraphdsu uf(n, score)。

### `rollbackgraphdsu`

```cpp
rollbackgraphdsu(int n, Score score) : component_count_(n), parent_or_size_(n, -1), edge_count_(n), score_(std::move(score))
```

O(n)。各孤立頂点のscore(1,0)を初期値として作る。

### `add_edge`

```cpp
bool add_edge(int a, int b)
```

O(log n)。無向辺(a,b)を追加し、異なる成分を併合したならtrueを返す。

### `same`

```cpp
bool same(int a, int b) const
```

O(log n)。aとbが同じ成分ならtrueを返す。

### `leader`

```cpp
int leader(int a) const
```

O(log n)。aの代表元を返す。

### `size`

```cpp
int size(int a) const
```

O(log n)。aの成分の頂点数を返す。

### `edges`

```cpp
int edges(int a) const
```

O(log n)。aの成分の辺数を返す。自己辺と多重辺も一辺ずつ数える。

### `operator[]`

```cpp
int operator[](int a) const
```

O(log n)。uf[a]でaの代表元を返す。

### `count`

```cpp
int count() const noexcept
```

O(1)。現在の連結成分数を返す。

### `value`

```cpp
const value_type& value() const noexcept
```

O(1)。全成分のscore(頂点数,辺数)の総和を返す。

### `snapshot`

```cpp
int snapshot() const noexcept
```

O(1)。現在位置をrollback用の値として返す。

### `undo`

```cpp
void undo()
```

O(1)。直前のadd_edge一回分を取り消す。履歴があること。

### `rollback`

```cpp
void rollback(int snapshot)
```

合計O(取り消す辺数)。snapshot()で得た時点まで戻す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Unionfind](https://judge.yosupo.jp/problem/unionfind)。

```cpp
poe::dsu sets(n);
if (type == 0) sets.merge(left, right);
else std::cout << sets.same(left, right) << '\n';
```

`verify/dsu.test.cpp`で2026-07-15に全ケースAC確認済み。

[AOJ DSL_1_B - Weighted Union Find Trees](https://onlinejudge.u-aizu.ac.jp/problems/DSL_1_B)用の
`verify/aoj_dsl_1_b.cpp`も収録した。

`rollbackdsu`はoffline dynamic connectivityなどの時間分割と組み合わせて使う。
[Library Checker - Dynamic Graph Vertex Add Component Sum](https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum)


[AtCoder ABC302 Ex - Ball Collector](https://atcoder.jp/contests/abc302/tasks/abc302_h)では、
`score(vertices, edges)=min(vertices, edges)`を使い、根から各頂点へのpathに対応する辺集合を
`root_path_rollback`で追加・取消する。`verify/atcoder_abc302_h.cpp`で公式sample 2件を確認済み。


[AtCoder ABC451 F - Make Bipartite 3](https://atcoder.jp/contests/abc451/tasks/abc451_f)は
各辺を`merge(u,v,1)`するだけで解ける。`verify/atcoder_abc451_f.cpp`で公式sampleを確認済み。
