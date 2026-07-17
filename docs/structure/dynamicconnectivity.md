---
title: "Offline Dynamic Connectivity"
documentation_of: //cp/structure/dynamicconnectivity.hpp
---

# Offline Dynamic Connectivity


## 概要

無向辺が途中で追加・削除される時系列を先に登録し、各時刻の連結判定、連結成分サイズ、
連結成分数をまとめて求める。時間segment treeとrollback DSUを内部で処理するため、
利用側は辺の生存区間やDFSを書かなくてよい。

## 厳密な定義

- `dynamicconnectivity`: 辺の追加・削除がある連結性をofflineで解く: dynamicconnectivity dc(n, q); solveはO((q+k)log q log n)。

```cpp
dynamicconnectivity dc(vertex_count, operation_count);
dc.add(time, a, b);
int answer_id = dc.same(time, x, y);
dc.remove(time, a, b);
vector<int> answers = dc.solve();
cout << answers[answer_id] << '\n';
```

`add`した辺はその時刻から有効で、`remove`した時刻には無効となる。
同じ辺を複数回追加した場合は多重辺として扱い、同じ回数だけ削除するまで連結性へ寄与する。
`remove`には、それ以前の対応する`add`が必要である。

`same(time, a, b)`、`size(time, a)`、`count(time)`は質問を登録し、結果配列の添字を返す。
同じ時刻へ複数の質問を登録してよい。構築後に`solve()`を呼び、返された`vector<int>`を
登録時の添字で読む。

## Include

```cpp
#include "structure/dynamicconnectivity.hpp"
```

## 計算量

頂点数を`n`、時刻数を`q`、辺の有効区間と質問の総数を`k`とする。

- 登録: `add`・`remove`は`O(log q)`、質問は`O(1)`
- `solve`: `O((q + k) log q log n)`
- 領域: `O((q + k) log q + n)`

単純な辺追加だけなら通常の`dsu`、削除順が逆から処理できるだけならreverse queryを使う方が軽い。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `dynamicconnectivity`

```cpp
struct dynamicconnectivity
```

辺の追加・削除がある連結性をofflineで解く: dynamicconnectivity dc(n, q); solveはO((q+k)log q log n)。

### `dynamicconnectivity`

```cpp
dynamicconnectivity(int n, int q) : n_(n), q_(q), queries_(q)
```

O(q)。n頂点、時刻[0, q)の処理列を作る。

### `add`

```cpp
void add(int time, int a, int b)
```

O(log q)。時刻timeから無向辺(a,b)を一つ追加する。

### `remove`

```cpp
void remove(int time, int a, int b)
```

O(log q)。時刻timeから無向辺(a,b)を一つ削除する。対応するaddが必要。

### `same`

```cpp
int same(int time, int a, int b)
```

O(1)。時刻timeでaとbが連結かを尋ね、solve結果の添字を返す。

### `size`

```cpp
int size(int time, int a)
```

O(1)。時刻timeでaの連結成分サイズを尋ね、solve結果の添字を返す。

### `count`

```cpp
int count(int time)
```

O(1)。時刻timeで連結成分数を尋ね、solve結果の添字を返す。

### `solve`

```cpp
std::vector<int> solve() const
```

O((q+k)log q log n)。登録した質問への答えを、各メソッドが返した添字順で返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[Library Checker - Dynamic Graph Vertex Add Component Sum](https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum)
には同じoffline dynamic connectivityが必要だが、さらに頂点加算と成分和をrollback可能な状態へ
載せる必要がある。本ヘッダは連結性・サイズ・成分数までを直接扱い、成分和版は今後のrecipe候補とする。

## 検証

- `tests/api/structure/dynamicconnectivity.cpp`: 多重辺、削除、3種類の質問
- `tests/property/structure/dynamicconnectivity.cpp`: 各時刻の愚直BFSとの固定seed比較
