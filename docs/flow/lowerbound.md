---
title: "lowerboundflow"
documentation_of: //cp/flow/lowerbound.hpp
---

# `lowerboundflow`

## Include

```cpp
#include "flow/lowerbound.hpp"
```

各辺について`lower <= flow <= upper`を課した循環、非負s-t最小流、最大流を求める。
`circulation/feasible`は最大流一回、`minflow/maxflow`は最大流二回を使用する。
解がなければ`std::nullopt`を返し、解には追加順の辺流量が入る。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `lowerboundflowresult`

```cpp
template<class Cap> struct lowerboundflowresult
```

下限制約付きs-tフローの解。flowは流量値、edge_flowは辺追加順の流量。

### `lowerboundflow`

```cpp
template<class Cap> struct lowerboundflow
```

各辺lower<=flow<=upperのフローを解く: lowerboundflow<long long> problem(n)。

### `lowerboundflow`

```cpp
explicit lowerboundflow(int n) : size_(n)
```

O(n)。n頂点の下限制約付きフロー問題を作る。

### `add_edge`

```cpp
int add_edge(int from, int to, Cap lower, Cap upper)
```

償却O(1)。lower<=flow<=upperの辺を追加し、辺番号を返す。

### `circulation`

```cpp
std::optional<std::vector<Cap>> circulation() const
```

最大流計算1回。流量保存を満たす循環があれば辺流量を返す。

### `maxflow`

```cpp
std::optional<lowerboundflowresult<Cap>> maxflow(int source, int sink) const
```

最大流計算2回。下限制約を満たすsourceからsinkへの最大流を返す。

### `minflow`

```cpp
std::optional<lowerboundflowresult<Cap>> minflow(int source, int sink) const
```

最大流計算2回。非負のs-t流のうち下限制約を満たす最小流を返す。

### `feasible`

```cpp
bool feasible() const
```

最大流計算1回。流量保存を満たす循環が存在するか返す。

### `size`

```cpp
int size() const noexcept
```

O(1)。頂点数を返す。

### `edge_count`

```cpp
int edge_count() const noexcept
```

O(1)。追加した辺数を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[AtCoder ABC285 G - Tatami](https://atcoder.jp/contests/abc285/tasks/abc285_g)では、
必ずマッチさせるマスに接続する辺の下限を1、任意のマスの下限を0として二部マッチングを作る。

```cpp
poe::lowerboundflow<int> problem(vertex_count);
problem.add_edge(source, left_vertex, must_use_left ? 1 : 0, 1);
problem.add_edge(left_vertex, right_vertex, 0, 1);
problem.add_edge(right_vertex, sink, must_use_right ? 1 : 0, 1);
bool feasible = problem.maxflow(source, sink).has_value();
```

`verify/atcoder_abc285_g.cpp`で2026-07-15にjudge AC確認済み。
