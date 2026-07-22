# ライブラリ選択ガイド

問題文に現れる対象や操作から、最初に確認するheaderを選ぶための索引です。
各リンク先にはinclude、適用条件、API、計算量、使用例があります。用途が完全に一致しない場合は、
完成形の`recipe`より、`structure`や`graph`などの汎用部品を優先してください。

## 区間・列・集合

| 問題文の語 | 最初に見るheader | 主な用途 |
| --- | --- | --- |
| 一点加算、区間和 | [`structure/fenwicktree.hpp`](structure/fenwicktree.md) | Fenwick Tree |
| 二次元点更新、左下長方形の和・最大 | [`structure/fenwicktree.hpp`](structure/fenwicktree.md) | 疎な二次元Fenwick Tree |
| 巨大座標、区間代入、上書き前の区間片 | [`structure/intervalmap.hpp`](structure/intervalmap.md) | 区間定数な写像を境界だけで管理 |
| 区間積、一点更新 | [`structure/segtree.hpp`](structure/segtree.md) | monoidのSegment Tree |
| 区間更新、区間取得 | [`structure/lazysegtree.hpp`](structure/lazysegtree.md) | 遅延伝播Segment Tree |
| 区間切り捨て除算、区間代入、区間和 | [`utility/presetsegtree.hpp`](utility/presetsegtree.md) | 非負整数列の除算更新を償却高速化 |
| kth、順位、頻度 | [`structure/waveletmatrix.hpp`](structure/waveletmatrix.md) | 静的列の順序統計 |
| 動的な順位、k番目 | [`structure/orderstatistic.hpp`](structure/orderstatistic.md) | 順序付き集合・多重集合 |
| sliding windowの集約 | [`structure/swag.hpp`](structure/swag.md) | queue全体のmonoid積 |
| 直線の最小・最大 | [`structure/linecontainer.hpp`](structure/linecontainer.md) | Convex Hull Trick / Li Chao Tree |
| 隣接差、傾き上限 | [`algorithm/slopeconstraint.hpp`](algorithm/slopeconstraint.md) | 傾き制約を満たす最大の下包絡 |
| 値へ法の倍数を加減、幅最小 | [`algorithm/modularrange.hpp`](algorithm/modularrange.md) | 剰余類を円周上で覆う最短幅 |
| 二列の全組の絶対差和 | [`algorithm/absolutedifference.hpp`](algorithm/absolutedifference.md) | $\sum_{i,j}|A_i-B_j|$ |
| 区間端点が交互に現れる対、交差する弦 | [`algorithm/intervalcrossing.hpp`](algorithm/intervalcrossing.md) | Fenwick Treeによる交互区間対計数 |
| 区間部分集合で全域を覆う、ランダム区間で被覆完了 | [`algorithm/intervalcover.hpp`](algorithm/intervalcover.md) | 本数別部分集合数と期待時間 |
| 区間最小値と非負区間和が上限以下 | [`algorithm/subarrayminsum.hpp`](algorithm/subarrayminsum.md) | Cartesian Treeによる部分配列計数 |
| 相異なる整数の部分配列、max-minと長さの差が小さい | [`algorithm/subarrayrange.hpp`](algorithm/subarrayrange.md) | 単調stackと小さい値だけ持つSegment Tree |
| 三つの座標が全て非減少な最大重み列 | [`algorithm/dominance.hpp`](algorithm/dominance.md) | 三次元dominance DPと復元 |
| 多重集合を2倍・2で切り捨て除算して一致 | [`algorithm/multisettransform.hpp`](algorithm/multisettransform.md) | 二進prefix木上の最小移動matching |
| word多重集合、末尾削除、prefixまで移動 | [`algorithm/multisettransform.hpp`](algorithm/multisettransform.md) | 左右別の辺制約を持つprefix matching |
| 各グループから一つ選ぶXOR、k番目 | [`algorithm/groupxor.hpp`](algorithm/groupxor.md) | 積を均衡化する半分全列挙とXOR順位統計 |
| 二列から一つずつ選ぶXOR、k番目 | [`structure/xortrie.hpp`](structure/xortrie.md) | 二つのbinary trieを同時に降りる順位統計 |
| ちょうどk個を選択、総和の二乗を含む最大化 | [`algorithm/quadraticselection.hpp`](algorithm/quadraticselection.md) | $(\sum x)^2+\sum y$の固定個数選択 |

## グラフ・木

| 問題文の語 | 最初に見るheader | 主な用途 |
| --- | --- | --- |
| 有向・無向グラフ、入力 | [`graph/graph.hpp`](graph/graph.md) | adjacency listと`read` |
| グリッドの指定可能マスを塞ぐ最小数と最適集合数 | [`graph/gridseparator.hpp`](graph/gridseparator.md) | 4近傍の最小頂点separator |
| 頂点訪問回数の偶奇を指定するwalk | [`graph/paritywalk.hpp`](graph/paritywalk.md) | 長さ$4n$未満の構成 |
| 隣接行列、全点対変換 | [`graph/matrixgraph.hpp`](graph/matrixgraph.md) | matrix形式とlist形式の相互変換 |
| 同じ凸多角形を複数回平行移動、全ての共通部分 | [`geometry/polygon.hpp`](geometry/polygon.md) | 辺ごとの半平面下限を前計算 |
| 最短路・ラベル別の最寄り始点 | [`graph/shortestpath.hpp`](graph/shortestpath.md) | BFS、0-1 BFS、Dijkstra、Bellman-Ford |
| 指定地点でのみ休憩、一回の移動上限、到達可能性 | [`graph/bottleneck.hpp`](graph/bottleneck.md) | 多点Dijkstraと閾値順DSU |
| 全点対最短路 | [`graph/warshallfloyd.hpp`](graph/warshallfloyd.md) | Warshall-Floyd |
| path上の最大頂点costを最小化 | [`graph/bottleneck.hpp`](graph/bottleneck.md) | bitset一括bottleneck query |
| 強連結成分 | [`graph/scc.hpp`](graph/scc.md) | SCC分解と縮約 |
| 有界整数、二変数の和の上下限 | [`graph/integerconstraints.hpp`](graph/integerconstraints.md) | $L\leq x_i+x_j\leq R$を満たす整数割当て |
| 橋、関節点、二重連結成分 | [`graph/lowlink.hpp`](graph/lowlink.md) | LowLink |
| 最小全域木 | [`graph/minimumspanningtree.hpp`](graph/minimumspanningtree.md) | Kruskalなど |
| 頂点数が小さい、森を辺数別に数える | [`graph/spanningforest.hpp`](graph/spanningforest.md) | 行列木定理と集合分割DP |
| 子をlabel昇順に辿るDFS、行きがけ順から木を数える | [`tree/preorder.hpp`](tree/preorder.md) | preorder区間DP |
| 最大流、最小費用流 | [`flow/maxflow.hpp`](flow/maxflow.md)、[`flow/mincostflow.hpp`](flow/mincostflow.md) | flowgraph上の探索 |
| 重み付き二部matching | [`flow/matching.hpp`](flow/matching.md) | matchingサイズ別の最小・最大重み |
| LCA、パス分解 | [`tree/lca.hpp`](tree/lca.md)、[`tree/heavylightdecomposition.hpp`](tree/heavylightdecomposition.md) | 木上queryの基礎 |
| 全方位木DP | [`tree/rerooting.hpp`](tree/rerooting.md) | rerooting DP |
| 部分木更新、パス和 | [`tree/eulertour.hpp`](tree/eulertour.md)、[`tree/pathsum.hpp`](tree/pathsum.md) | 木を列・差分へ変換 |
| パス上の値による頂点対集計 | [`tree/pathfold.hpp`](tree/pathfold.md) | gcd・min・bitwise ANDなどを値別に集約 |
| 子部分木から上がる需要と頂点処理量 | [`tree/demand.hpp`](tree/demand.md) | 残余DPと頂点値の閾値探索 |
| 仮想木 | [`tree/virtualtree.hpp`](tree/virtualtree.md) | 指定頂点だけを含む圧縮木 |
| 全部分列の根パス和集合 | [`tree/subarrayunion.hpp`](tree/subarrayunion.md) | 各部分木の出現位置をsmall-to-large集約 |
| 互いに祖先・子孫でない頂点集合 | [`tree/antichain.hpp`](tree/antichain.md) | antichainを大きさ別に高速計数 |
| 頂点prefixごとに完全二分木・完全多分木を数える | [`tree/perfectsubtree.hpp`](tree/perfectsubtree.md) | 基本対称式の祖先差分DP |
| 凸多角形を避ける最短路 | [`geometry/convexpath.hpp`](geometry/convexpath.md) | 凸障害物の左右を回る最短距離 |
| 点追加と内積最大化 | [`geometry/incrementalhull.hpp`](geometry/incrementalhull.md) | incremental convex hullのsupport query |
| 整数格子、二種類までの移動ベクトル | [`geometry/latticemove.hpp`](geometry/latticemove.md) | 非負整数結合で目的地へ着く最小手数 |
| 直線交点、円内の交点数、k番目の交点距離 | [`geometry/line.hpp`](geometry/line.md) | 直線配置を弦の交差へ変換 |
| 同型を除いた単純グラフ数 | [`graph/unlabeledcount.hpp`](graph/unlabeledcount.md) | 全色使用の頂点彩色付き無ラベルグラフ |

## 文字列

| 問題文の語 | 最初に見るheader | 主な用途 |
| --- | --- | --- |
| pattern検索 | [`string/matching.hpp`](string/matching.md) | KMPなどの文字列照合 |
| 接頭辞一致、Z配列 | [`string/zalgorithm.hpp`](string/zalgorithm.md) | Z algorithm |
| suffix array、部分文字列比較 | [`string/suffixarray.hpp`](string/suffixarray.md)、[`string/lcp.hpp`](string/lcp.md) | suffix順とLCP |
| 全部分文字列を重複込みで辞書順に並べる | [`string/substringorder.hpp`](string/substringorder.md) | 順位から元文字列上の出現位置を取得 |
| 異なる部分文字列、オンライン追加 | [`string/suffixautomaton.hpp`](string/suffixautomaton.md) | suffix automaton |
| palindrome | [`string/manacher.hpp`](string/manacher.md)、[`string/palindromictree.hpp`](string/palindromictree.md) | 回文半径・回文木 |
| 辞書、prefix | [`string/trie.hpp`](string/trie.md) | Trie |
| hashで部分文字列比較 | [`string/rollinghash.hpp`](string/rollinghash.md) | Rolling Hash |
| 二部分列の要素ごとのXOR、LCP | [`string/xorrollinghash.hpp`](string/xorrollinghash.md) | nimber Rolling Hash |
| 禁止patternを壊す最小文字置換 | [`string/avoid.hpp`](string/avoid.md) | Aho–Corasickと区間greedy |

## 数学・FPS・DP

| 問題文の語 | 最初に見るheader | 主な用途 |
| --- | --- | --- |
| mod整数 | [`math/modint.hpp`](math/modint.md) | 静的・動的modint |
| 小さい素数法の巨大二項係数、Lucas | [`math/lucas.hpp`](math/lucas.md) | $n,k$が巨大な$\binom nk\bmod p$ |
| RLE列を隣接和で大量に縮約、小さい素数法 | [`math/binomialsequence.hpp`](math/binomialsequence.md) | 素数冪距離のskip |
| 状態が上へ高々1進むMarkov連鎖、ポテンシャル | [`math/randomwalk.hpp`](math/randomwalk.md) | 状態順に期待値方程式を解く |
| Gaussian二項係数、有限体の部分空間 | [`math/qanalog.hpp`](math/qanalog.md) | $q$階乗と行空間の制約付き計数 |
| 等差数列のpopcount和 | [`math/bitcountsum.hpp`](math/bitcountsum.md) | 各bitをfloor sumで数える |
| 平方根以下へ値を移す列の数え上げ | [`math/floorsqrtchain.hpp`](math/floorsqrtchain.md) | $O(X^{1/4})$前計算・$O(1)$クエリ |
| 番号付け替えを同一視、Burnside | [`math/burnside.hpp`](math/burnside.md) | 対称群のcycle typeによる軌道計数 |
| 素数、素因数分解 | [`math/prime.hpp`](math/prime.md) | primality / factorization |
| XORを固定したtuple・multiset計数 | [`math/xorcount.hpp`](math/xorcount.md) | bounded tuple、distinct tuple、非減少列の変換 |
| 二成分の部分集合XOR、第一成分に上限、第二成分を最大化 | [`math/xorbasis.hpp`](math/xorbasis.md) | prefix制約付きGF(2)解空間最適化 |
| Nim積、XORを加法とする体 | [`math/nimber.hpp`](math/nimber.md) | 64-bit nimber演算 |
| Stern–Brocot木、既約分数の仮想木 | [`math/sternbrocot.hpp`](math/sternbrocot.md) | 巨大深さをrun-length圧縮 |
| 列和固定、各行に奇数要素 | [`math/paritymatrix.hpp`](math/paritymatrix.md) | 全成分偶数の行を包除する行列計数 |
| 畳み込み | [`math/convolution.hpp`](math/convolution.md) | NTTなど |
| maskのXOR畳み込み | [`math/subsettransform.hpp`](math/subsettransform.md) | Walsh--Hadamard変換 |
| 総和とXORを同時に畳み込み、べき乗 | [`math/multiconvolution.hpp`](math/multiconvolution.md) | 二次元分布DPの高速化 |
| min-plus / max-plus畳み込み | [`math/tropicalconvolution.hpp`](math/tropicalconvolution.md) | 凸性を使う場合を含む |
| 行列、巨大時刻の線形遷移、疎な例外時刻 | [`math/matrix.hpp`](math/matrix.md) | 行列累乗と疎イベント付き線形DP |
| 0/1行列、長方形XOR制約 | [`math/rectangleparity.hpp`](math/rectangleparity.md) | 長方形の四隅だけを使うGF(2)連立方程式 |
| ランダムウォークの合流・初到達 | [`math/randomwalk.hpp`](math/randomwalk.md) | 合流確率列・周期剰余への初到達期待値 |
| 多項式、形式的冪級数 | [`fps/fps.hpp`](fps/fps.md)、[`fps/polynomial.hpp`](fps/polynomial.md) | inv、log、exp、補間など |
| 上限付き等比級数積の巨大次数係数 | [`fps/boundedgeometric.hpp`](fps/boundedgeometric.md) | 小種類数の多重集合型母関数 |
| 係数確定ごとに一次因子を追加 | [`fps/online.hpp`](fps/online.md) | 動的な$\prod(1+a_ix)$の係数再帰 |
| 整数分割、一般五角数、Euler積 | [`fps/partition.hpp`](fps/partition.md) | 分割数列と巨大次数の疎な係数抽出 |
| Stirling数、集合分割、cycle数 | [`fps/stirling.hpp`](fps/stirling.md) | 第一種・第二種とblock内置換のcycle数分布 |
| 小alphabetの動的な異なる部分列数 | [`string/dynamicsubsequence.hpp`](string/dynamicsubsequence.md) | 許容文字集合の一点更新 |
| 線形漸化式の第N項 | [`fps/bostanmori.hpp`](fps/bostanmori.md)、[`math/linearecurrence.hpp`](math/linearecurrence.md) | Bostan-Moriなど |
| 値ごとの重み付き冪和を全次数について求める | [`fps/rational.hpp`](fps/rational.md) | $\sum_i w_i v_i^k$を有理型母関数で一括計算 |
| 積の剰余を指定した巨大長列 | [`math/modularproduct.hpp`](math/modularproduct.md) | 素数冪と$p$進付値による数え上げ |
| 色別個数固定の順列、隣接色変化数 | [`fps/combinatorial.hpp`](fps/combinatorial.md) | ラベル付き順列を変化回数別に数える |
| knapsack | [`dp/knapsack.hpp`](dp/knapsack.md)、[`dp/rangeknapsack.hpp`](dp/rangeknapsack.md) | 制約別knapsack |
| 部分集合和を選択個数の偶奇別に数える | [`dp/subsetsumcount.hpp`](dp/subsetsumcount.md) | 同値な値をまとめた多項式積 |
| 桁DP | [`dp/digit.hpp`](dp/digit.md) | automatonと桁制約 |
| 種類別人数を指定して二群へ割当て | [`dp/groupassignment.hpp`](dp/groupassignment.md) | 順番固定・quota付きの配置DP |
| 複数区間の最大値を指定して列を数える | [`dp/intervalmaximum.hpp`](dp/intervalmaximum.md) | 区間上限と最大値の存在条件をまとめるDP |
| 同じgroupの二点を結ぶ右下grid pathの総数 | [`dp/gridpath.hpp`](dp/gridpath.md) | 頻度別に全点対と盤面DPを自動切替 |
| subset DP、TSP | [`dp/subset.hpp`](dp/subset.md)、[`graph/travelingsalesman.hpp`](graph/travelingsalesman.md) | bit DP |
| Alien DP、傾き最適化 | [`dp/aliens.hpp`](dp/aliens.md)、[`dp/slopetrick.hpp`](dp/slopetrick.md) | Lagrangian relaxation / slope trick |
| 木DP関数の加算、suffix min-plus | [`dp/decreasingconvex.hpp`](dp/decreasingconvex.md) | merge可能な単調減少離散凸関数 |
| 順列の位置別下限制約、成立個数分布 | [`dp/permutationthreshold.hpp`](dp/permutationthreshold.md) | 高速Hakone Ekiden DP |

## ゲーム

| 問題文の語 | 最初に見るheader | 主な用途 |
| --- | --- | --- |
| DAG上の通常プレイ、Grundy数 | [`game/dag.hpp`](game/dag.md) | impartial gameの勝敗と直和 |
| 手数が固定されたgraph game | [`game/finite.hpp`](game/finite.md) | 手番ごとのOR・ANDによる有限手数minimax |
| 循環、終了優先、交互min/max、非負辺費用 | [`game/alternating.hpp`](game/alternating.md) | 後退解析による有限性と最適総費用 |

## 強い条件がある完成形

固定の操作集合や強い制約まで一致する場合だけ、[Recipe選択ガイド](recipe/guide.md)から選びます。
条件が少しでも異なる場合は、各recipeの依存先にある汎用部品を使って組み立ててください。
