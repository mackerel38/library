# poe competitive library

GNU++23 / AtCoder向けの競技プログラミングライブラリです。

## 使い方

`cp`ディレクトリをinclude pathへ追加し、利用したいheaderを`cp/`なしでincludeします。

```cpp
#include "structure/fenwicktree.hpp"
#include "graph/graph.hpp"

using namespace poe;
```

```sh
g++ -std=gnu++23 -O2 -Icp main.cpp
```

各headerページには、次の情報を掲載します。

- 何を解決するライブラリか
- publicな型・関数・コンストラクタ・メソッドの宣言と動作
- 引数、返り値、添字・区間、使用条件
- 時間計算量と追加領域
- よくある誤用
- 実在問題での使用例とjudge確認状況

## カテゴリ

- `algorithm`: 二分探索、列操作、offline queryなどの汎用アルゴリズム
- `structure`: Fenwick Tree、Segment Tree、DSUなどのデータ構造
- `graph`, `tree`: グラフ表現とグラフ・木アルゴリズム
- `flow`: 最大流、最小費用流、流量制約、劣modular最小化
- `math`, `fps`: 数学アルゴリズム、畳み込み、形式的冪級数
- `string`: 文字列検索、suffix系、automaton
- `dp`: 再利用できるDPの部分問題・遷移
- `geometry`: 二次元幾何
- `utility`: 問題文の操作名で直ちに使える完成済みデータ構造
- `recipe`: 適用範囲が狭い高水準shortcut。明示的にincludeします

下の「Library Files」からカテゴリ・headerを選んでください。
