# EDPC・TDPC・NDPC 分類とライブラリ化判断

対象はEDPC 26問、TDPC 20問、NDPC 20問の計66問である。
問題そのものを関数に閉じ込めるのではなく、別問題でも同じ形で使える部分だけを抽出する。
DPの主要部品は現在十分に揃っているため、この表の`recipe`や`固有`を問題別wrapperとして
埋めることはしない。新しい抽象構造が見つかった場合だけcoreへ追加する。

状態は次の意味で使う。

- **実装**: 今回追加または既存の公開ヘッダで直接再利用できる。
- **recipe**: 狭い問題族で共通する定型遷移。再利用可能な完成形だけ`cp/recipe`へ隔離し、
  それ以外は利用側へ書く。
- **固有**: 解法の中心が問題固有の観察であり、無理にライブラリ化しない。
- **不足**: 再利用価値はあるが、今回の実装範囲では安全なAPI・検証まで完了していない。

## Educational DP Contest

| 問題 | ジャンル | 判断 | 対応 |
| --- | --- | --- | --- |
| A Frog 1 | 線形DP・最短路 | recipe | 小さい遷移幅の基本形 |
| B Frog 2 | 線形DP・区間最小 | recipe | 遷移幅が入力依存 |
| C Vacation | 有限状態DP | recipe | 状態と禁止遷移が問題固有 |
| D Knapsack 1 | 0/1ナップサック・容量基準 | 実装 | `dp/knapsack.hpp` |
| E Knapsack 2 | 0/1ナップサック・価値基準 | 実装 | `dp/knapsack.hpp` |
| F LCS | 列DP・復元 | 実装 | `dp/sequence.hpp` |
| G Longest Path | DAG DP | 実装 | `graph/topologicalsort.hpp`と利用側DP |
| H Grid 1 | グリッドDP | recipe | 障害物と遷移方向が問題側 |
| I Coins | 確率DP | recipe | Bernoulli和の基本形 |
| J Sushi | 期待値DP・自己遷移除去 | recipe | 状態圧縮が問題固有 |
| K Stones | impartial game | recipe | 勝敗DPの基本形 |
| L Deque | 区間DP・零和ゲーム | recipe | 評価式が問題固有 |
| M Candies | 累積和最適化 | recipe | sliding prefix sum |
| N Slimes | 区間DP・Knuth最適化 | 実装 | `dp/optimization.hpp` |
| O Matching | bit DP・割当 | recipe | 標準bit DPだが入力関係を直接使う |
| P Independent Set | 木DP | 実装 | `tree/dp.hpp` |
| Q Flowers | LIS型DP・区間最大 | 実装 | `utility/presetsegtree.hpp` |
| R Walk | 行列累乗 | 実装 | `math/matrix.hpp` |
| S Digit Sum | 桁DP | 実装 | `dp/digit.hpp` |
| T Permutation | 挿入DP・累積和 | recipe | 不等号列固有の遷移 |
| U Grouping | 部分集合DP・分割 | recipe | 部分集合スコアが問題入力そのもの |
| V Subtree | 全方位木DP | 実装 | `tree/rerooting.hpp` |
| W Intervals | DP＋遅延Segment Tree | 実装 | `utility/presetsegtree.hpp` |
| X Tower | ソート＋0/1ナップサック | 実装 | ソートは問題側、DPは`dp/knapsack.hpp`を参考 |
| Y Grid 2 | 組合せ・障害点DP | 実装＋recipe | `math/combination.hpp`、障害点DPは問題側 |
| Z Frog 3 | Convex Hull Trick | 実装 | `structure/linecontainer.hpp` |

## Typical DP Contest

| 問題 | ジャンル | 判断 | 対応 |
| --- | --- | --- | --- |
| A コンテスト | 0/1部分和・bitset | 実装 | `dp/subsetsum.hpp` |
| B ゲーム | 二山ゲームDP | recipe | 手番を消費個数の偶奇で表す |
| C トーナメント | 確率DP・完全二分木 | recipe | 対戦確率が問題固有 |
| D サイコロ | 確率DP・素因数指数 | recipe | 2,3,5の指数を状態にする |
| E 数 | 桁DP | recipe | tight・剰余・出現フラグ |
| F 準急 | 累積和最適化 | recipe | 固定幅のDP和 |
| G 辞書順 | distinct subsequence・k番目復元 | 実装 | `string/subsequence.hpp` |
| H ナップザック | 色数制限ナップサック | recipe | 色ごとのtoggle状態が問題固有 |
| I イウィ | 区間DP | recipe | 消去可能判定が文字列固有 |
| J ボール | 期待値bit DP・自己遷移除去 | recipe | 命中集合が問題固有 |
| K ターゲット | LIS | 実装 | `dp/sequence.hpp` |
| L 猫 | prefix max・prefix sum | recipe | 漸化式が問題固有 |
| M 家 | TSP bit DP・行列累乗 | 実装 | `graph/travelingsalesman.hpp`と`math/matrix.hpp` |
| N 木 | 木の線形拡張数・組合せ | recipe | 辺を縮約した根付き木DP |
| O 文字列 | 組合せDP | 固有 | 頻度ブロックの組合せ遷移 |
| P うなぎ | 木上tree-knapsack | recipe | open/closed状態が問題固有 |
| Q 連結 | automaton型bit DP | recipe | 最大語長に依存するsuffix状態 |
| R グラフ | SCC圧縮・bitonic DP | 実装＋recipe | `graph/scc.hpp`、圧縮後DPは問題固有 |
| S マス目 | frontier/connection profile DP | recipe | 汎用部品は`dp/frontier.hpp`と`dp/profile.hpp`、完成形は`recipe/connectivityfrontier.hpp` |
| T フィボナッチ | 線形漸化式 | 実装 | `math/linearecurrence.hpp` |

## Next DP Contest

テーマは[公式テーマ一覧](https://atcoder.jp/contests/ndpc/editorial/19121)を基準にした。

### 閲覧状況

- [全問題一覧](https://atcoder.jp/contests/ndpc/tasks)と[印刷用全問題文](https://atcoder.jp/contests/ndpc/tasks_print)は閲覧できる。
- 個別問題URLは`ndpc_a`ではなく`ndpc2026_a`形式である。旧形式を使っていた文書は修正した。
- 公式解説ページは閲覧できるが、全体テーマ一覧と後半の一部略解が中心で、各問の正式解説は未公開のものが多い。

| 問題 | ジャンル | 判断 | 対応 |
| --- | --- | --- | --- |
| A ポリオミノ | 状態設計DP | 固有 | 何を状態にするかが問題の中心 |
| B DAG | メモ化再帰・トポロジカルDP | 実装＋recipe | `graph/topologicalsort.hpp` |
| C 文字列 | DP上のDP・automaton DP | recipe | オートマトン自体が問題固有 |
| D 紙幣 | 状態数削減 | 固有 | 遷移の観察が問題の中心 |
| E 夏休み | ダブリング | 実装 | `structure/doubling.hpp` |
| F 集合 | Cartesian Tree・二乗木DP | 実装＋recipe | `tree/cartesiantree.hpp` |
| G 口 | automaton DP・動的DP | 不足 | monoid化後のSegment Treeは問題依存が強い |
| H コイン拾い | bool DP次元落とし | 固有 | 次元落としの観察が中心 |
| I 更新点 | 挿入DP | recipe | 挿入位置による状態設計 |
| J 個数と総和 | 繰り上がり付き桁DP | recipe | 基数・carry状態を利用側で定義 |
| K 加算と減算 | 部分和への帰着 | 実装 | `dp/subsetsum.hpp` |
| L 最小公倍数 | 約数ゼータ変換 | 実装 | `math/divisortransform.hpp` |
| M 数字 | 高速ゼータ変換・SOS DP | 実装 | `math/subsettransform.hpp` |
| N ナップサック | unbounded knapsack | 実装＋固有高速化 | `dp/knapsack.hpp`、満点高速化は未実装 |
| O ゲーム | max-plus畳み込み・Aliens trick | 実装＋recipe | `math/tropicalconvolution.hpp`と`dp/aliens.hpp` |
| P LIS | 固有DP・LIS | 実装＋固有 | `dp/sequence.hpp`は構成要素のみ |
| Q 区間の和集合 | DP・多項式補間・省メモリ | 実装＋recipe | `math/interpolation.hpp`、DP本体は問題側 |
| R 3つ組 | ternary wildcard更新・包除・乱択 | 固有 | 問題特化データ構造となるため作らない |
| S 2枚の扉 | 標数2上の特殊DP・指定点経由最短path | 固有 | 一般用途が狭く、現時点では作らない |
| T 独立集合 | static top tree DP | 実装＋recipe | `tree/statictoptree.hpp`、5演算の設計は問題側 |

## なお問題固有として残すもの

基礎部品の不足は今回解消した。一方、次は問題文固有の状態・観察が解法の中心なので完成解法を
公開ヘッダにはしない。

1. NDPC A/D/H/R/Sの状態設計・次元落とし・特殊な標数2 DP
2. NDPC Gの盤面固有automaton DPとその動的更新モノイド
3. NDPC Nの巨大重量query向け周期・高速化部分
4. NDPC Pの問題固有LIS遷移
5. TDPC O/P/Q/Rの入力固有の組合せ・木DP・automaton・SCC後DP

これらには利用できる下位部品を対応表へ記載し、問題そのものを名前だけ変えた専用関数は作らない。

## 現在の追加方針

今後は次の順で改善する。

1. 既存ヘッダの適用条件・計算量・典型WA・関連部品を文書化する。
2. 公開問題のverifyを追加し、judge ACとローカル差分検査を区別して記録する。
3. 複数問題に共通する新しい状態表現または遷移高速化が見つかった場合だけ実装を増やす。
4. 問題固有wrapperは追加せず、高水準だが再利用できるものだけrecipeへ置く。
