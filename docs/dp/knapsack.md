---
title: "ナップサック"
documentation_of: //cp/dp/knapsack.hpp
---

# ナップサック

- Header: `cp/dp/knapsack.hpp`
- Symbol: `poe::knapsack01`, `poe::classify_knapsack01_items`, `poe::knapsack01_value`, `poe::knapsack01_mitm`, `poe::knapsack01_auto`, `poe::bounded_knapsack`, `poe::bounded_knapsack_tiny_weights`, `poe::unbounded_knapsack`, `poe::minimum_unbounded_knapsack_cost`
- Status: experimental

## どんな問題に使えるか

重さと価値が加算され、重さの総和が容量以下となる選び方の最大価値を求める問題に使う。
同じ0/1ナップサックでも、容量、価値総和、品物数のどれが小さいかによって解法を選ぶ。
個数が無制限または上限付きの場合も、同じ入力模型のまま専用の遷移を利用できる。

| 制約 | 関数 | 計算量 |
| --- | --- | --- |
| 容量`W`が小さい0/1 | `knapsack01` | `O(nW)`時間、`O(W)`領域 |
| 各品物が全最適解で必要か知りたい0/1 | `classify_knapsack01_items` | `O(nW)`時間・領域 |
| 価値総和`V`が小さい0/1 | `knapsack01_value` | `O(nV)`時間、`O(V)`領域 |
| 品物数が小さい0/1 | `knapsack01_mitm` | `O(2^(n/2))`時間・領域 |
| 上のいずれかが小さい0/1 | `knapsack01_auto` | 三解法から推定計算量最小を選択 |
| 各種類を何個でも選べる | `unbounded_knapsack` | `O(nW)`時間、`O(W)`領域 |
| コストが小さく目標価値が巨大 | `minimum_unbounded_knapsack_cost` | 最大コスト`C`として`O(nC^2)`時間 |
| 各種類に個数上限がある | `bounded_knapsack` | `O(nW)`時間、`O(W)`領域 |
| 個数・容量は巨大だが重みが1〜3 | `bounded_knapsack_tiny_weights` | `O(n log n)`時間、`O(n)`領域 |

すべて「重さが容量以下」の最大価値を返し、何も選ばない場合の価値`0`を許す。
価値DPと自動選択では価値を非負整数に限る。

## 使い方

制約から解法が明らかな場合は、計算量が名前から分かる関数を直接選ぶ。

```cpp
#include "dp/knapsack.hpp"

long long answer1 = poe::knapsack01<long long>(weight, value, capacity);
long long answer2 = poe::bounded_knapsack(weight, value, count, capacity);
long long answer3 = poe::bounded_knapsack_tiny_weights(weight, value, count, huge_capacity);
long long answer4 = poe::minimum_unbounded_knapsack_cost(cost, value, huge_target);
```

全最適解を比べて各品物を分類するときは、戻り値を入力順に調べる。
`mandatory`はすべての最適解、`optional`は一部の最適解だけで使われ、
`excluded`はどの最適解でも使われない。

```cpp
auto status = poe::classify_knapsack01_items(weight, value, capacity);
```

0/1で容量・価値総和・品物数のどれを軸にすべきか入力制約から一意に決まらない場合は、
`knapsack01_auto`が推定計算量の最小なものを選ぶ。

```cpp
long long answer = poe::knapsack01_auto(weight, value, capacity);
```

どの解法でも状態数が巨大になる入力は扱えない。重さが特殊な基数の冪、異なる重さの種類数が少ない、
最大重さ・最大価値だけが小さい、といった追加構造には別のアルゴリズムが必要である。

## よくある誤り

- 0/1問題で同じ品物を複数回使える向きに更新する。
- 重さ0・正価値の品物を個数無制限問題へ渡す。答えが有限でなくなる。
- `knapsack01_auto`ならあらゆる巨大制約を解けると考える。三つの標準解法から選ぶだけである。
- 価値DPへ負の価値を渡す。価値を配列添字に使うため非負が必要である。
- 「容量以下」と「重さがちょうど容量」を混同する。このAPIは前者の最大価値を返す。

選んだ品物の復元、選択数制約、辞書順最小解が必要な場合は、親情報を持つ問題側DPを書く。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `knapsack01`

```cpp
template <class T> T knapsack01( const std::vector<int>& weights, const std::vector<T>& values, int capacity )
```

O(nW)。knapsack01(weight, value, W): 重さW以下の0/1ナップサック最大価値を返す。

### `knapsackitemstatus`

```cpp
enum class knapsackitemstatus
```

最適な0/1ナップサック解における品物の使われ方。

### `classify_knapsack01_items`

```cpp
template <class T> std::vector<knapsackitemstatus> classify_knapsack01_items( const std::vector<int>& weights, const std::vector<T>& values, int capacity )
```

O(nW)時間・O(nW)領域。全最適解で各品物が必須・任意・不使用かを返す。

### `knapsack01_by_value`

```cpp
inline std::vector<long long> knapsack01_by_value( const std::vector<long long>& weights, const std::vector<int>& values )
```

O(nV)。knapsack01_by_value(weight, value): 各価値を作る最小重量を返す。価値は非負整数。

### `knapsack01_value`

```cpp
inline long long knapsack01_value( const std::vector<long long>& weights, const std::vector<int>& values, long long capacity )
```

O(nV)。knapsack01_value(weight, value, W): 価値DPで重さW以下の最大価値を返す。

### `knapsack01_mitm`

```cpp
template<class T> T knapsack01_mitm( const std::vector<long long>& weights, const std::vector<T>& values, long long capacity )
```

O(2^(n/2))時間・領域。knapsack01_mitm(weight, value, W): 半分全列挙で最大価値を返す。

### `knapsack01_auto`

```cpp
inline long long knapsack01_auto( const std::vector<long long>& weights, const std::vector<long long>& values, long long capacity )
```

容量DP・価値DP・半分全列挙の最小推定計算量。knapsack01_auto(weight, value, W): 最大価値を返す。
価値は非負整数で、いずれかの解法の状態数がメモリに載る制約で使う。

### `unbounded_knapsack`

```cpp
template <class T> T unbounded_knapsack( const std::vector<int>& weights, const std::vector<T>& values, int capacity )
```

O(nW)。unbounded_knapsack(weight, value, W): 同じ品物を何個でも使える最大価値を返す。

### `minimum_unbounded_knapsack_cost`

```cpp
inline long long minimum_unbounded_knapsack_cost( const std::vector<int>& costs, const std::vector<long long>& values, long long target )
```

O(nC^2)時間・O(C^2)領域。小さい正コスト品を無制限に選び、価値target以上の最小コストを返す。
Cは最大コスト。valuesとtargetは正で、答えがlong longに収まること。

### `bounded_knapsack`

```cpp
template<class T> T bounded_knapsack( const std::vector<int>& weights, const std::vector<T>& values, const std::vector<long long>& counts, int capacity )
```

O(nW)。bounded_knapsack(weight, value, count, W): 各品物をcount個まで使える最大価値を返す。

### `bounded_knapsack_tiny_weights`

```cpp
template <class T> T bounded_knapsack_tiny_weights( const std::vector<int>& weights, const std::vector<T>& values, const std::vector<long long>& counts, long long capacity )
```

O(n log n)。重みが1,2,3だけの個数制限knapsackを巨大capacityでも解く。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

- [AOJ DPL_1_B - 0-1 Knapsack Problem](https://onlinejudge.u-aizu.ac.jp/problems/DPL_1_B): 容量DP
- [AOJ DPL_1_C - Knapsack Problem](https://onlinejudge.u-aizu.ac.jp/problems/DPL_1_C): 個数無制限
- [AOJ DPL_1_F - 0-1 Knapsack Problem II](https://onlinejudge.u-aizu.ac.jp/problems/DPL_1_F): 価値DP
- [AOJ DPL_1_G - Knapsack Problem with Limitations](https://onlinejudge.u-aizu.ac.jp/problems/DPL_1_G): 個数制限
- [AOJ DPL_1_H - Huge Knapsack Problem](https://onlinejudge.u-aizu.ac.jp/problems/DPL_1_H): 半分全列挙
- [EDPC D - Knapsack 1](https://atcoder.jp/contests/dp/tasks/dp_d)
- [EDPC E - Knapsack 2](https://atcoder.jp/contests/dp/tasks/dp_e)
- [ABC441 F - Must Buy](https://atcoder.jp/contests/abc441/tasks/abc441_f): 最適解における各品物の分類
- [ABC431 D - Robot Customize](https://atcoder.jp/contests/abc431/tasks/abc431_d):
  全品物を体へ付けた価値を基準に、頭へ移す品物の重さと価値差で0/1ナップサック

EDPC D/Eは2026-07-15にjudge AC確認済み。
AOJの5 verifierは2026-07-15にAOJ公式テストデータ全190ケースで自動verify成功。
0/1は500回、個数制限は300回、独立した全列挙との固定seed差分検査を行っている。

## 参考と今後の範囲

[競プロ的ナップサック問題11問](https://www.mathenachia.blog/knapsack/)の制約分類を参考にした。
今回は記事の(1)から(5)に対応する、一般的な容量・個数・価値・品物数制約を実装した。
(6)以降のうち、最大重さが3で容量と個数が巨大な場合は`bounded_knapsack_tiny_weights`として追加した。
選択個数を6との剰余で固定し、残りを同じ重さ6のgroupへ統合する。

[AtCoder ABC442 G - Lightweight Knapsack](https://atcoder.jp/contests/abc442/tasks/abc442_g)の
`verify/atcoder_abc442_g.cpp`で公式sampleを確認済み。judge ACは未確認。
基数の冪や、最大重さが4以上の一般化は前提と計算量を個別に検証してから追加する。

ABC441 Fはprefix DPを全時点分、suffix DPを走査中の1行だけ保持することで、
素直な前後DP二表の約半分となる`O(nW)`領域で分類する。
`verify/atcoder_abc441_f.cpp`で公式sampleを確認済み。judge ACは未確認。

ABC431 Dは`verify/atcoder_abc431_d.cpp`で公式sampleを確認済み。judge ACは未確認。

[AtCoder ABC310 Ex - Negative Cost](https://atcoder.jp/contests/abc310/tasks/abc310_h)では、
長さ`1..600`の基本行動列を品物へ圧縮した後、巨大な目標damageへ
`minimum_unbounded_knapsack_cost`を使う。最良の価値/コスト比を持つ品物以外は
最大コスト未満の個数だけ使う最適解が存在するため、残差コスト`O(C^2)`だけをDPすればよい。
`verify/atcoder_abc310_h.cpp`で公式sample 2件を確認済み。judge ACは未確認。
