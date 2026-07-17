# ライブラリ拡張予定

この文書は、ABCで現れた再利用可能な部分問題を中心に、未収録分野と実装順を管理する。
「ABC全問題を問題別関数にする」のではなく、各問題の重い部分を他の問題でも使える抽象へ分解して網羅する。
問題固有の入出力や一度しか現れない式変形は`verify`へ置く。

## 優先順位

| 優先 | カテゴリ | 実装する核 | 重い典型・recipe候補 | 状態 |
| --- | --- | --- | --- | --- |
| 0 | `algorithm` | 整数・実数の単調述語二分探索、尺取り、整数isotonic balancing | 頻出の汎用制御部品を追加 | 基本実装済み |
| 1 | `geometry` | 点、外積、交差、凸包、包含、アフィン変換、円、最近点対 | 最小包含円も実装済み。半平面交差は未実装 | 基本実装済み |
| 2 | `math` | 素数篩、素因数分解、約数列挙、XOR線形基底、一般の体とGF(2)上の連立一次方程式は実装済み | Miller–Rabin、Pollard–Rho、離散対数も実装済み | 基本実装済み |
| 3 | `structure` | 累積和、Sparse Table、SWAG、永続Segment Tree、DSU派生、Mo、Wavelet Matrix、動的中央値は実装済み | offline dynamic connectivityも実装済み。成分集約付き版はrecipe候補 | 基本実装済み |
| 4 | `graph` | 0-1 BFS、Bellman–Ford、functional graph、Euler路、二部matching、最大clique・最大独立集合は実装済み | 一般matching、全方位木距離集約 | 基本実装済み |
| 5 | `string` | rolling hash、trie、Aho–Corasick、禁止pattern回避語、巡回シフト、suffix automaton、回文木は実装済み | 回文分割のseries link高速化は将来候補 | 基本実装済み |
| 6 | `tree` | virtual tree、重心分解、DSU on Tree、頂点・辺pathと部分木の木上差分、完全Euler tourは実装済み | 最寄り有効頂点recipeは実装済み | 基本実装済み |
| 7 | `dp` | bitset部分和、集合分割、区間併合、状態圧縮、concave列、高々K個の非交差区間は実装済み | divide-and-conquer DP最適化は実装済み。convolution DPは未実装 | 基本実装済み |
| 8 | `utility` | 23種の用途別区間構造。区間加算・和・二乗和とSegment Tree Beatsも実装済み | 新しい頻出操作の組だけ随時追加 | 基本実装済み |

## ABC出題との対応方針

過去問監査の正本は`coverage/problems.tsv`とする。最初のreleaseは2026-07-12 cutoffの
直近10回（ABC457〜466）C〜G、計50問で、`python3 tools/coverage.py summary`が
確認済み・未確認・不足ありを分母付きで集計する。

- 幾何: [ABC151 F](https://atcoder.jp/contests/abc151/tasks/abc151_f)、
  [ABC189 E](https://atcoder.jp/contests/abc189/tasks/abc189_e)、
  [ABC248 E](https://atcoder.jp/contests/abc248/tasks/abc248_e)、
  [ABC266 C](https://atcoder.jp/contests/abc266/tasks/abc266_c)を最初の基準問題にする。
- 数論: 素因数分解、約数、合同式、巨大整数の素数判定を分け、入力上限から選べる文書を用意する。
- データ構造: オンライン・オフライン、更新可否、可換性の違いを型と選択表で明示する。
- グラフ・木: `directed_graph`と`undirected_graph`を維持し、共通アルゴリズムはconcept付きtemplate一つで書く。
- 文字列: 文字種を固定せず、必要な場合だけ小文字英字向けの短いshortcutを別に用意する。
- 重い典型は、複数問題で同じ状態管理を繰り返す場合に`recipe`へ置く。単なる解答全文は置かない。

## 網羅判定

各ABCを定期的に確認し、未対応問題を次の三つに分類する。

1. 既存ライブラリの組合せで主要実装が短くなる。
2. 再利用可能な新しいcoreまたはrecipeを追加できる。
3. 問題固有の考察が本体で、ライブラリ化しない。

「網羅」は1または2へ分類でき、2にはhpp・md・verifyコード・テストが揃った状態を指す。
分類表は実装の進行に合わせて追加し、judge ACとサンプル確認のみを区別する。

## 第一回ABC監査の結果

ABC457〜466のC〜G、計50問は2026-07-15に全行を確認済みにした。
この監査から、問題別shortcutではなく次の再利用可能な核を追加した。

- `algorithm`: 二分探索、尺取り、右移動による整数isotonic balancing
- `structure`: 任意次元累積変換、差分配列、二次元Mo、動的中央値
- `graph/tree`: grid BFS、動的頂点集合直径、二色木の同色連結成分集約
- `string`: 自己重なりを除くpattern DP、禁止pattern回避語のautomaton行列累乗
- `dp`: 要素数別部分集合和統計、concave max-DP列、高々K個の非交差部分配列
- `math`: 巨大な`n`・小さい`k`の二項係数

短いgreedy、式変形、偶奇別の格子凸最適化などは、再利用性が低いと判断して
`verify`と`coverage/problems.tsv`へ解法を残し、public APIにはしていない。

## 次の監査単位

ABC447〜456のC〜Gを同じ50問単位で監査し、50問すべてを確認済み。
ABC455 Fから`range_add_range_sum_square`を追加し、ABC456 E/Fでは既存の
有向閉路検出とSWAGを重い問題へ再利用できることを確認した。ABC454 Gからは
再帰を使わない`dsuontree`を追加した。ABC453 Gからは整数version handle型の
`persistentsegtree`を追加した。ABC452 D/F/Gからは部分列制約付きsubstring数え上げ、
転倒数を管理する尺取り、禁止値を含まない相異substring数え上げを追加した。ABC451 E/F/Gからは
木距離行列復元、parity DSU、XOR Trieを追加した。ABC450 C/D/Eからはgrid成分列挙、
剰余円周の最小幅、巨大Fibonacci word queryを追加した。ABC449 Fからは軸平行長方形の
和集合面積を追加し、ABC449 Gでは既存FPSを再利用した。ABC448 E/Gからは巨大十進RLE演算と
永続動的Li Chao Treeを追加した。ABC447 F/Gからは条件付き木上最長pathと
キー重複を除く固定長top-K monoidを追加した。続くABC437〜446の50問を監査中で、
ABC446 Fから頂点有効化順に到達集合を保つ`incremental_reachability`を追加した。
ABC445 Fから任意半環の行列積・累乗と短い`minplus_matrix_power`を追加した。ABC445 Gでは
既存の二部matchingから最大独立集合を復元でき、重い構成問題にも再利用できることを確認した。
ABC444 Fから均等二分後のmultisetを生成個数に依存せず圧縮する`balanced_split_leaves`を追加した。
ABC444 Gからガウス整数、有限monoid冪周期、二平方和解の剰余分布を分離して追加した。
ABC443 D/Fから離散Lipschitz下包絡と暗黙automatonの最短辞書順受理語を追加した。
ABC442 E/Gから整数偏角比較と、重み1〜3・巨大容量の個数制限knapsackを追加した。
ABC441 F/Gから全最適ナップサック解における品物分類と、active要素だけへの区間加算・
全値clear後の状態反転・区間最大値を組み合わせた用途別遅延Segment Treeを追加した。
ABC440 D〜Gから固定除外整数集合、固定個数multiset和の上位列挙、順位範囲和付き動的multiset、
layerごとのgrid連結成分圧縮と隣接layer接続を追加した。ABC440 Gの一度だけ梯子を使うDPは
問題固有部分としてverifyへ残し、再利用可能な入力graph構築だけをpublic APIにした。
ABC439 Gから`[x^n] f(x)^i g(x)`を全`i`について求めるpower projectionを追加した。
二変数Bostan–MoriをKronecker substitutionで一次元畳み込みへ落とし、定数項非0も
binomial transformで扱う。ABC439 E/Fでは既存のLISとFenwick Treeを再利用した。
ABC438 C〜Gから隣接同値群の縮約、非空連続フェーズ分割、経路集約付きダブリング、
LCA上のパス所属・切断側サイズ、固定列のオフライン区間上限制約和を追加した。
周期列の巨大長和は`gcd`で互いに素な部分へ分け、全周期と高々二つの通常区間へ帰着した。
ABC437 D〜Fから二列間の全組絶対差和、末尾追加で共有される列群の辞書順、
一点更新付き区間最遠Manhattan距離を追加した。ABC437 Gでは既存最大流を、木辺を削除するときの
状態割当てと実行順復元へ再利用した。これでABC437〜446の監査対象50問は全行確認済みとなった。
続くABC427〜436の50問も監査対象へ追加した。ABC436 D/Gから単位cost hyperedge付きBFSと、
巨大上限の非負整数線形不等式を二進縮約と畳み込みで数えるAPIを追加した。ABC435 D/E/Gからは、
始点追加型の単調到達可能集合、追加・削除・被覆長を持つ半開区間集合、全keyのvalueへaffine変換を
遅延するmapを追加した。ABC435 FではCartesian Treeを比較関数対応にしてmax木にも共通化した。
ABC434 D〜Gからは2次元差分配列、辺数・木判定付き連結成分、`XY`と`YX`をZ-arrayで高速比較する
連結順、push・pop操作列の一点更新・区間評価Segment Treeを追加した。特に最後の構造は、定数サイズへ
圧縮すると積が閉じない情報をSegment Treeの分割履歴から復元する重い典型として独立させた。
ABC433 D/Gからbase進表記で二数を連結した合同条件を満たす順序付きpair数と、DAG上の
impartial gameのGrundy数・勝敗判定を追加した。GではSuffix AutomatonとDAGゲームを合成する。
ABC433 C/Fは既存のrun-length encoding・組合せを再利用し、Eの制約付き置換行列構成は
問題固有性が高いためcoverageへ整理だけ記録した。
ABC432 D〜Fから半開長方形の境界分割・平行移動・辺接触判定、動的multisetのclamp後総和、
零和部分集合への最大個数分割と復元を追加した。ABC432 Gは既存の二項変換を再利用した。
ABC432 Cの数式走査はライブラリを使わない問題固有解なので、coverageへ整理だけ記録しverifyは作らない。
ABC431 E〜Gから暗黙graphの0-1 BFS、隣接減少幅制約付きmultiset順列数、1回swap後の全列に対する
辞書順offline順位queryを追加した。最後はswapの左端・移動値・右端だけで辞書順を決め、Fenwick Treeで
指定順位を選ぶ重い典型として独立させた。ABC431 Dは既存0/1 knapsackを再利用し、Cのsort greedyは
ライブラリを使わないためcoverageへの記録だけにした。
ABC監査は以後G問題以降と各回の最終問題を優先する。ABC430 Gから、64要素以下の集合列に対する
区間追加・削除と最大要素数・達成位置数queryを扱う`setsegtree`を追加した。区間の和集合と積集合から
更新bitが一様かを判定し、失敗時だけ子へ降りるSegment Tree Beatsとして重い典型を独立させた。
ABC429 GからはFloor Sumの格子経路に沿う任意モノイド積`floor_product`と、合成数法でも使える
`sum_power_mod_linear`を追加した。後者は法をbaseと互いに素な部分と冪零部分へ分け、前者と
低指数の逆像列挙をCRTで統合する。
ABC428 Gからは添字の積に対するDirichlet畳み込み・逆元・形式対数と、重みの積ごとに有向necklaceを
数える`product_necklace_counts`を追加した。問題固有の美しさ入力から切り離し、約数DPや乗法的な
形式級数にも再利用できるcoreと、巡回列族に再利用できる高水準APIを同じ抽象の上へ分離した。
ABC427 Gからは、`x<=threshold`なら一定値を加え、それ以外なら一定値を引く写像列を末尾追加できる
`thresholdsequence`を追加した。写像列を等価な単調閾値列へ正規化し、二進カウンタ状のblockで保持して、
追加ならし`O(log n)`・任意の初期値への全操作適用`O(log^2 n)`を実現する。
次の監査範囲としてABC417〜426をcoverageへ追加し、最終問題優先でABC426 Gから着手した。
さらにABC407〜416のC〜Gをcoverageへ追加した。この範囲もC〜Fの穴埋めより先に、
ABC416 Gから降順で各回のG・最終問題を一巡し、再利用可能な重い典型を優先して抽出する。
ABC416 Gからは、同じ候補文字列集合から重複を許してちょうどK個選ぶ辞書順最小連結
`minimum_repeated_concatenation`を追加した。連結順比較、最小無限周期語、周期内位置DPを内部化する。
ABC415 Gからは任意半環Kitamasa `semiring_recurrence`と、そのmax-plus版
`maxplus_recurrence`を追加した。小状態DP以降が固定最大報酬漸化式になる巨大容量問題を飛ばせる。
ABC414 Gからは、交差しない二区間の全頂点対へ`base+座標距離`の有向辺を張る
`range_distance_shortest_paths`を追加した。東西四本のsegment tree graph構築とDijkstraを隠す。
ABC413 Gからは巨大盤面を確保せず、疎な障害物の8近傍連結と外周接触だけで左上から右下への
4近傍pathを判定する`sparse_grid_path_exists`を追加した。平面双対の障壁判定を内部化する。
ABC412 Gからは乱択Tutte行列とPfaffian多項式補間による
`minimum_binary_perfect_matching_weight`を追加した。一般グラフの重み0/1完全matchingを扱う。
ABC411 Gからは20頂点以下の多重無向グラフで、辺集合を区別した頂点単純cycle数を返す
`count_simple_cycles`を追加した。長さ2の多重辺cycleとsubset path DPを一つのAPIへまとめる。
ABC410 Gからは、区間を外側から内側へ狭義包含する最長列を復元する
`longest_nested_intervals`を追加した。円環上の弦は二つの直線区間へ展開して同じAPIへ落とす。
ABC409 Gからは、`sum_{n>=k} C(n,k)a[n]c^(n-k)`を全ての`k`について計算する
`upper_binomial_transform`を追加した。初登場時刻や選択数別に上側から集約する期待値計算へ使える。
ABC408 Gからは、有理数の開区間に入る分母最小の分数を返す
`simplest_fraction_between`を追加した。連分数の同方向遷移を商でまとめて対数時間で処理する。
ABC407 Gからは、空集合を含む任意サイズの重み付き二部matchingを最小化する
`minimum_weight_bipartite_matching`を追加した。負辺の平行移動とmatchingサイズ別費用の復元を隠す。
監査範囲をABC212まで拡張し、ABC212〜406に実在するG 194問とH/Ex 106問をcoverageへ追加した。
以後はH/ExをGより優先し、ABC318 Exから降順に重い典型を抽出する。
ABC318 Exは新APIを増やさず、既存`fps::exp`をcycle分解の指数型母関数へ適用するverifyを追加した。
ABC317 Exからは、低次多項式を要素に持つ小正方行列列を分割統治で掛ける
`polynomial_matrix_product`を追加した。局所walk漸化式をまとめ、有理型母関数の巨大係数へ接続する。
ABC315 Exからは、両方の入力列を左から確定しながら積の現在係数を返す
`relaxedconvolution`を追加した。自己畳み込みを含むonline DPを直接書ける。
ABC314 Exからは、実数凸関数の最小点と最小値を返す`minimize_convex_real`を追加した。
多変数でも座標ごとの凸性と周辺化後の凸性を示せる場合は入れ子で使える。
ABC313 Exからは、値を昇順に最終順列へ挿入した際の隣接済み成分数を状態にする
`insertion_component_counts`を追加した。新成分・片側接続・二成分結合の係数を共通化する。
ABC312 Exからは、文字列の最短原始周期`primitive_period`と、入力順に過去と重複しない
最小反復回数を割り当てる`minimum_unique_repetition_counts`を追加した。
ABC311 Exからは、少数branchの部分木変換をheavy childで共有する
`heavyrecursivedp`を追加した。木ナップサック固有の遷移はcallbackへ残し、
畳み込みを避ける重軽再帰と全部分木への初期状態再適用を共通化する。
ABC310 Exからは、小さい正コスト品を無制限に選んで巨大な目標価値へ達する
`minimum_unbounded_knapsack_cost`を追加した。最良効率品以外の使用を
最大コスト二乗以内の残差DPへ押し込む部分を共通化する。
ABC309 Exからは`cyclic_convolution`を追加し、`cyclic_power`を一般長へ拡張した。
2冪長の周波数直接累乗と、一般長の巡回畳み込み二分累乗をAPI内部で切り替える。
ABC308 Exからは、非負重み無向グラフで指定頂点を通る最小単純cycleを返す
`minimum_cycle_through_vertex`を追加した。根からの最短路木を最初の子で分類し、
異なる枝を結ぶ非木辺で閉じる処理と、辺一つの使用禁止を共通化する。
ABC307 Exからは、両側にwildcardを許す全alignmentを返す`wildcard_matches`を追加した。
文字値の二乗差和を3回の畳み込みで求め、係数上限に応じて単一modと整数CRTを切り替える。
ABC306 Exからは、各無向辺を二方向または同一視の三状態にし、縮約後がDAGとなる割当数
`count_acyclic_edge_states`を追加した。部分集合の誘導連結成分数を符号とする包除DPを内部化する。
ABC304 Exからは、各頂点の配置可能位置を閉区間で指定する
`bounded_topological_sort`を追加した。後続頂点の上限をDAGの逆順で伝播し、
解禁済み頂点のうち締切最小を選ぶ貪欲法を再利用可能なAPIへまとめる。
ABC303 Exからは、次数ごとの任意の重みの積を全ラベル付き木について合計する
`labeled_tree_degree_weight_sum`と、許可次数集合を直接渡す短縮版を追加した。
Prüfer列の出現回数と次数の対応をEGFへ隠し、禁止次数以外の次数別重み問題にも再利用できる。
ABC302 Exからは、成分の頂点数・辺数だけで決まる任意のスコア総和を保つ
`rollbackgraphdsu`と、rollback可能な状態を根から全頂点へのpathで評価する
`root_path_rollback`を追加した。問題固有の値選択を多重グラフの辺追加へ分離する。
ABC301 Exからは、指定辺の重み増加で二頂点間minimax距離が増えるかを判定する
`bottlenecksensitivity`を追加した。同重みKruskal層の橋、MST上の辺cut、
path最大重みを組み合わせ、queryごとの再計算を不要にする。
ABC300 Exからは、有理型母関数の係数をindexが指定maskのsubmaskである範囲で合計する
`bostan_mori_submask_sum`と`rationalfps::submask_sum`を追加した。
Bostan--Moriの偶奇抽出をbitごとの片方選択から和の選択へ一般化する。
ABC299 Exからは、正の確率stepの累積和が指定周期剰余へ初到達する期待step数を返す
`cyclic_hitting_expectation`を追加した。局所漸化式をaffine行列で進め、
周期境界に残る最大step未満の未知値だけをGauss消去で閉じる。
ABC298 Exからは、木の全頂点・固定根部分木への距離和と、二点の近い方への距離和を返す
`treedistancesum`を追加した。path中央でのVoronoi分割を部分木と補集合へ落とし、
深さ総和・祖先の部分木サイズprefix・reroot距離和で処理する。
ABC297 Exからは、重み付き記号wordで同じ記号の隣接を禁じた個数と長さ総和を返す
`adjacent_distinct_words`を追加した。同一記号runへの包除と長さmarkの微分を
FPSの逆元へまとめ、同重みの別記号も区別して扱う。
ABC296 Exからは、幅の小さい盤面で必須マスを含む最小連結supersetを返す
`minimum_connected_grid_completion`をrecipeへ追加した。必須maskのsuperset列挙と
frontier成分を消さない遷移で、複数成分の将来合流を追加flagなしに扱う。
ABC295 Exからは、行左prefixと列上prefixのORで作れる相異なるgridをpattern制約下で数える
`count_row_column_prefix_union`をrecipeへ追加した。列prefix継続maskと行prefix終端を
canonicalに処理し、複数の操作列が同じ完成gridを作る重複を除く。
ABC294 Exからは、ranked zetaによる`subset_convolution`と、空集合値1の列を巨大指数で冪乗する
`subset_convolution_unit_power`を追加した。さらに低次数除去・次数2の削除縮約と組み合わせた
`count_graph_colorings`で、頂点数・辺数30以下の疎グラフ彩色数を一つのAPIにまとめた。
ABC294 Gからは、木の辺番号をHLD位置へ隠して一点更新・取得・path和を行う
`edgepathsum`を追加した。提出側は辺更新と`sum(u, v)`だけを書けばよい。
ABC293 Gからは、静的列の各区間で同値な`k`添字組を数える
`equal_tuple_range_counts`を追加した。Moの頻度更新と二項係数差分を利用側から隠す。
ABC293 Exは目的関数が特殊なためrecipeへ分離し、path成分への頂点分割で任意path上の最大成分数を
最小化する`minimum_path_partition_width`を追加した。判定は各頂点で上位4子だけを保持する。
ABC292 Gからは、一般alphabetの同長wildcard列を辞書順狭義増加へ補完する
`count_strictly_increasing_wildcard_sequences`を追加した。各桁の同一記号groupを区間DPで処理する。
ABC292 Exからは、一点更新・prefix和・最大prefix和・指定値への最初の到達位置を扱う
`point_set_prefix_sum_max`を用途別区間構造へ追加した。
多数の部分配列0/1ナップサックを列の分割統治で処理する`range_knapsack01`を追加し、中央をまたぐ
queryへ左suffix容量DPと右prefix容量DPを共有する重い典型を独立させた。
ABC425 Gからは、静的整数集合に対する最近傍XOR距離を整数半開区間全体で合計する`sum_min_xor`を
追加した。Binary Trie各部分木の全域和を前計算し、任意区間では端に対応する一本の経路だけを辿る。
ABC424 Gからは、左次数上限と右要求次数だけを持つ完全二部グラフの実現可能性判定と、右頂点部分集合の
最大報酬`max_bipartite_degree_reward`を追加した。Gale--Ryser型prefix条件と選択DPを一つのAPIに隠す。
ABC423 Gからは、固定数字列を部分文字列として含む最小の十進倍数`smallest_multiple_containing`を追加した。
前後の追加桁を分割し、短い側の列挙と非互いに素な一次合同式を組み合わせる処理を隠蔽する。
ABC423 Fからは、条件集合をすべて満たす個数から一致条件数別の個数へ変換する
`exact_match_counts`と、LCMで倍数条件を構築する`divisibility_match_counts`を追加した。
問題固有の対象名ではなく、包除の入力形式と倍数条件族を再利用可能なAPIとして分離する。
ABC423 Eからは、静的列の指定区間内に含まれる全部分配列の和を返す`subarrayprefixsum`を追加した。
値と添字一次・二次momentの累積和を内部化し、同じ二次寄与を持つ区間queryへ再利用できるようにする。
ABC422 Gからは、各groupの大きさが指定周期の倍数となる分配を、要素の区別なし・ありの双方で数える
`divisible_group_counts`を追加した。OGFとEGFの因子構築・積・階乗復元を一つの制約APIへまとめる。
ABC421 Gからは、頂点ごとの供給上限・必須需要・容量付き有向辺を受け取る`mincost_transport`を追加した。
差分列と区間加算の変換はverify側に残し、超始点・超終点の構築と実行可能性判定を再利用可能にした。
ABC420 Gからは、非零整数の正負の約数を昇順列挙する`signed_divisors`を追加した。
二次式から積への問題固有変形はverify側に残し、符号付き因数候補の列挙をDiophantine方程式へ再利用できるようにした。
ABC419 Gからは、余剰辺数が小さい無向グラフの単純pathを長さ別に数える
`simple_path_length_counts`を追加した。葉刈り・次数2の道の縮約・小グラフDFSを一つの重い典型へまとめる。
ABC418 Gからは、任意括弧付けの二項演算到達集合`parenthesized_results`、membership oracleを
有界右文脈で分類する`right_context_dfa`、DFA受理部分文字列を集計する`accepted_substring_stats`を追加した。
問題固有の16場合分けを避け、区間DP・automaton構築・線形suffix集計の三つの再利用可能な段階へ分離する。
ABC417 Gからは、既存version同士の連結と巨大列の添字参照を行う`persistentconcatenation`を追加した。
長さ飽和・heavy childのbinary lifting・light遷移回数の対数上界を内部化し、文字列以外の列にも使える形にする。
G問題の現監査範囲を一巡したためC〜Fへ戻り、ABC430 Fから有向path posetの各value可能位置区間と
各位置の候補数を返す`path_order_positions`を追加した。L/R文字自体ではなく前後関係predicateを受け取る。
ABC430 Eからは二列間の最小左巡回シフト回数`rotation_distance`を追加し、既存のBooth法APIと
巡回列操作をまとめた。targetの最初の出現をKMPで求め、周期列の複数一致にも対応する。
ABC430 Dからは数直線上の動的点集合で各点の最寄り別点距離総和を保つ`nearestset`を追加した。
挿入・削除の影響を更新点と昇順隣接点だけへ限定する差分更新を、再利用可能な構造として隠蔽する。
ABC430 Cからは二つのpredicateの出現数に下限・上限を課す部分配列数
`count_subarrays_frequency_constraints`を追加した。文字種専用ではなく、重なりうる任意条件を受け取る。
ABC429 Fからは固定高さgridの各列を行間min-plus遷移として合成する
`monotonegridshortestpath`を追加した。壁の一点更新と任意の始終点列区間queryを扱い、
「左へ戻らない最短路」が保証される細い盤面問題へ再利用できる形にした。
ABC429 Eからは各頂点に近い異なる始点を固定個数求める`nearest_sources_bfs`を追加した。
通常の多点始点BFSを一番近い始点だけで終わらせず、始点labelごとの到達を各頂点でCount個まで保持する。
ABC429 Dからは非負円環列の各始点について、和が閾値以上となる最短prefixの長さと和を返す
`circular_threshold_windows`を追加した。巨大な円周座標への重み付けはverify側へ残した。
ABC429 Cは値ごとの頻度`f`について`C(f,2)(N-f)`を足すだけの問題固有な短い集計なので、
抽象化を増やさずcoverageへの記録だけにした。
ABC428 Fからは長さ昇順の入れ子区間列をprefix単位で左端・右端へ整列し、点被覆数を求める
`nestedintervals`を追加した。同じ整列状態の連続添字をblock stackへ圧縮して更新を償却する。
ABC428 Eからは任意の同距離tie-breakを受け取り、全頂点からの最遠頂点と距離を返す
`tree_farthest_vertices`を追加した。直径端点だけに依存する問題固有処理ではなく全方位木DPで実装した。
ABC428 Dからは非負64bit整数の厳密な`floor_sqrt`と`is_square`を数学基本APIへ追加した。
浮動小数点平方根は初期値だけに使い、128bit積で丸め誤差を補正する。
ABC428 Cからは末尾push・popに対応する`bracketsequence`を追加した。
各長さのbalanceとprefix最小値を履歴化し、現在の正当性を定数時間で判定する。
ABC427 Fからはpath上で隣接要素を同時に選ばない部分集合和の列挙
`nonadjacent_subset_sums`と、剰余target別の個数を半分全列挙で求める
`count_nonadjacent_subsets_mod`を追加した。
ABC427 Eからは長方形grid上の点集合を一斉平行移動し、禁止点を避けて盤外へ全消去する
`minimum_safe_translation_clear_steps`を追加した。残存元座標長方形と累積移動量へ状態を圧縮する。
ABC427 Dからは最終頂点の真偽と各手番の最大化・最小化を受け取る
`finite_horizon_game`を追加した。交互手番以外の有限手数graph gameにも使える。
ABC427 Cからは小頂点数の非負重み無向グラフで最大cutと二色分けを返す
`maximum_cut`を追加した。最小辺削除二部化は全辺重みから最大cutを引いて求める。
ABC426 Fからは非負列へ区間一様消費を行い、0を下限として実際の消費量和を返す
`range_subtract_clamp_zero_sum`を追加した。枯渇要素を無効化し、全操作で各要素を高々一度だけ探索する。
ABC426 Eからは点と線分の距離、および二点がそれぞれ線分を速さ1で進み到着後停止するときの
`minimum_distance_segment_walks`を追加した。相対位置を高々二本の線分へ分ける。
ABC426 Dは既存`runlength`で文字別総数と最長runを集計するverifyを追加した。
ABC426 Cは単調pointerと配列だけで全体`O(N+Q)`となり、専用APIより直接実装が短いためcoverage記録に留めた。
ABC425 Fからは、同じ残存列へ至る一要素削除を重複させず空列までの全削除順を数える
`count_distinct_deletion_orders`を追加した。文字列以外の同値比較可能な列にも使える。
ABC425 Eからは任意の正の法に対する`pascalcombination`と多項係数を追加した。
逆元を使わず、上限が小さい合成数法の多数queryへ使う。
ABC425 Dからは、各round開始時の条件判定後に頂点を一斉有効化する
`synchronous_activation`とgrid版を追加した。条件が有効近傍数について非単調でも同期性を保ち、
新規有効頂点の近傍だけを次候補にするため全体線形となる。
ABC425 Cからは静的円環列の論理的な左回転、区間和、一点取得を`O(1)`で扱う
`circularprefixsum`を追加した。二連結累積和とoffset管理を利用側から隠蔽する。
ABC424 Fからは端点が重複しない閉区間をonline追加し、既存laminar familyと交差する場合だけ拒否する
`laminarintervalset`を追加した。各点を含む最短区間をrange chmin・point getで管理する。
ABC424 Eからは最大要素を一つ選んで半分二つへ置換する操作を巨大回数行った後の順位を返す
`kth_largest_after_max_halving`を追加した。最終最大値の二分探索とdyadic分布復元を内部化する。
ABC424 Dからは各行の元maskからsubmaskを選び、隣接行predicateの下で削除bit数を最小化・復元する
`minimum_submask_profile`を追加した。細い盤面の局所禁止模様や編集DPへ使える。
ABC424 Cは既存`directed_graph`と`bfs`で仮想始点からの到達可能性をそのまま表せるため、
新規APIは増やさず再利用verifyだけを追加した。

各10回分が終わるたびに、
新規header数ではなく「既存APIで短くなった問題」「新しいcoreになった問題」
「問題固有として見送った問題」の比率を確認し、具体化しすぎたAPIを増やさない。
