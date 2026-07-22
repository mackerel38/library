# カテゴリ別実装計画

この文書は、未実装の有名アルゴリズムを無条件に増やす一覧ではない。
ABCのG・H・Exを監査し、重い部分問題が現れたときに、どの汎用部品へ切り出すかを決めるための計画である。

## 監査順

1. ABC288 ExからABC212 Exまでを降順に確認する。
2. ABC406 GからABC212 Gまでを降順に確認する。
3. ABC423〜407のC〜Fで残っている行を埋める。
4. 10回分ごとに、既存API再利用・新規core・問題固有の比率を確認する。

各問題は、既存APIで十分、新しい汎用coreが必要、強い条件を持つ`recipe`が必要、問題固有なので追加しない、
のいずれかへ分類する。headerを追加する場合はhpp・md・metadata・API test・property test・verifyを同時に揃える。

## カテゴリ別候補

| カテゴリ | 次に不足を確認する核 | 重い典型として切り出せる条件 | 追加判断 |
| --- | --- | --- | --- |
| `algorithm` | selection、offline parallel binary search、整数凸最適化 | 同じ判定を多数queryへ並列に進める | 二分探索本体より状態更新の共通化が大きい場合 |
| `structure` | implicit sequence tree、dynamic forest、offline 2D query | cut・paste・reverseを伴う列、辺cut/linkを伴う木 | 自前実装が長く、APIの操作集合を問題名なしで定義できる場合 |
| `graph` | dominator、directed MST、k-shortest walk/path | 有向到達の必須点、根付き有向全域木、複数経路列挙 | graph型の有向・重み性をconceptで制約できる場合 |
| `tree` | tree isomorphism、動的木、部分木同型集約 | canonical form、link/cut、同型部分木の頻度 | HLD・top tree・centroid treeで足りないことを先に確認する |
| `flow` | cost付き下限制約、より高速な最大流、割当て復元 | circulationと最小費用を同時に要求する問題族 | `flowgraph`のcost有無を型で表す現在の設計を維持する |
| `string` | Lyndon分解、runs、series-link回文分割 | 全反復構造、最小回文分割を線形・準線形で求める | suffix系・回文木との選び分けを文書化できる場合 |
| `geometry` | 半平面交差、Minkowski和、rotating calipers | 多角形の共通部分、凸多角形和、対蹠点query | 退化ケースと整数・実数predicateをproperty testできる場合 |
| `math` | 乗法的関数のprefix和、任意mod畳み込みの拡張、組合せ変換 | $n$が巨大な素数和、法がNTT不適、集合・multiset間変換 | 数式の入出力を問題固有語なしで一意に定義できる場合 |
| `fps` | polynomial gcd・factorization、multivariate係数抽出 | 多項式の共通因子、低変数の巨大次数係数 | 既存FPS24・Bostan--Mori・set FPSで代用できない場合 |
| `dp` | quota付き割当て、Monge畳み込み、木knapsackの共有 | 少数種類count DP、凸列遷移、子併合の反復 | callbackが状態遷移を自然に表し、利用側の多重loopを消せる場合 |
| `game` | drawを含む勝敗game、確率game | 勝敗・引分と最短/最長手数、確率最適化 | 重み付き交互min/max後退解析で表せない場合 |
| `recipe` | frontier、動的集合、区間構造の複合操作 | 強い前提まで複数問題で一致する完成形 | `all.hpp`へ入れず、使える条件と使えない条件を冒頭に書く |

## 現在の直近設計

- ABC288 Ex: bounded XOR tuple、distinct XOR tupleへの変換、XOR multisetへの変換として実装済み。
- ABC287 Ex: bitset Warshallによる頂点bottleneck path queryとして実装済み。
- ABC286 Ex: 凸多角形障害物を避ける二点間最短路として実装済み。
- ABC285 Ex: 列和固定で全成分偶数の行を禁止する非負整数行列計数として実装済み。
- ABC284 Ex: 対称群Burnsideと頂点彩色付き無ラベル単純グラフ計数として実装済み。
- ABC283 Ex: floor sumによる等差数列のpopcount総和として実装済み。
- ABC282 Ex: 区間最小値と非負区間和の上限制約を満たす部分配列計数として実装済み。
- ABC281 Ex: 確定係数から一次因子を逐次追加するonline FPS積として実装済み。
- ABC280 Ex: 複数文字列の全部分文字列を出現ごとに数える辞書順順位索引として実装済み。
- ABC279 Ex: Lucas二項係数と一般五角数定理によるEuler積係数として実装済み。
- ABC278 Ex: Gaussian二項係数・有限体行空間計数・Stirling数変換として実装済み。
- ABC277 Ex: 有界整数変数に対する二変数和制約を、閾値命題による2-SATへ変換するAPIとして実装済み。
- ABC276 Ex: 長方形XOR制約を四隅の累積変数だけで解き、0/1行列を復元するAPIとして実装済み。
- ABC275 Ex: merge可能な単調減少離散凸関数とsuffix min-plus遷移として実装済み。
- ABC274 Ex: 64-bit Nim積と、要素ごとのXORに線形なnimber Rolling Hashとして実装済み。
- ABC273 Ex: 圧縮Stern–Brocot仮想木と、全部分列の根パス和集合重み集計として実装済み。
- ABC272 Ex: 順列の位置別下限制約を満たす個数分布を、多点評価で求めるHakone Ekiden DPとして実装済み。
- ABC271 Ex: 二種類までの整数移動ベクトルによる最小手数と、許可8方向の完成形recipeとして実装済み。
- ABC270 Ex: 最大不足量への状態圧縮後はmodint累乗を含む短い問題固有漸化式だけなので、新API・verifyは追加しない。
- ABC269 Ex: Static Top Treeと多項式積で、根付き木のantichainを大きさ別に数えるAPIとして実装済み。
- ABC268 Ex: Aho–Corasickの最短suffix一致と、禁止pattern全出現を壊す最小文字置換として実装済み。
- ABC267 Ex: 部分集合和を作る方法数を選択個数の偶奇別に一括計算するAPIとして実装済み。
- ABC266 Ex: 疎な二次元Fenwick Treeと、最大重み三次元dominance chain・復元として実装済み。
- ABC265 Ex: Walsh--Hadamard変換、XOR畳み込み、和×XOR二次元畳み込みの高速べき乗として実装済み。
- ABC264 Ex: 頂点prefixごとの完全r分誘導部分木計数として実装済み。
- ABC263 Ex: 交互区間対計数、直線基本型、円内交点数、k番目の直線交点距離として実装済み。
- ABC262 Ex: 複数の区間最大値制約を満たす非負整数列の数え上げとして実装済み。
- ABC261 Ex: 非負重み付き有向グラフの終了優先交互min/maxゲームとして実装済み。
- ABC260 Ex: ラベル付き色順列の隣接色変化数分布と重み付き冪和として実装済み。
- ABC259 Ex: group別の右下単調grid path総数を疎密切替で数えるAPIとして実装済み。
- ABC258 Ex: 通常行列遷移の間に疎な例外時刻を持つ巨大時刻DPとして実装済み。
- ABC257 Ex: 固定個数の点を選ぶ$(\sum x)^2+\sum y$最大化を、支持方向の全順位交換を走査するAPIとして実装済み。
- ABC256 Ex: 非負整数列の区間切り捨て除算・区間代入・区間和を用途別区間構造へ追加済み。
- ABC255 Ex: 巨大座標上の区間定数写像と、代入時に上書き前の区間片を返すAPIとして実装済み。
- ABC254 Ex: 左右別の辺移動制約を持つprefix多重集合matchingと、2倍・2除算shortcutとして実装済み。
- ABC253 Ex: 重み付き無向グラフの全域森を辺数別に数えるAPIとして実装済み。
- ABC252 Ex: 各groupから一つ選ぶXORと、二列間XORのk番目を求めるAPIとして実装済み。
- ABC252 G: 子をlabel昇順に辿るDFSの行きがけ順から根付き木を数える区間DPとして実装済み。
- ABC251 Ex: 小さい素数法でRLE列の隣接和縮約を素数冪skipへ分解するAPIとして実装済み。
- ABC251 G: 平行移動した同一凸多角形群の共通部分への点包含APIとして実装済み。
- ABC250 Ex: terminalでのみ区切れる一回上限付き移動queryとして実装済み。
- ABC250 G: 8行程度のpriority queue greedyが最短のため、独立APIを作らず監査記録のみ追加済み。
- ABC249 Ex: 上方向へ高々1進むMarkov連鎖のポテンシャル方程式として実装済み。
- ABC249 G: 第一成分XORに上限を課す二成分XOR基底最適化として実装済み。
- ABC248 Ex: 相異なる整数列の部分配列について値域幅と長さの余剰を小さいK以下で数えるAPIとして実装済み。
- ABC248 G: 可換演算で畳み込んだ木上パスを値別に本数・長さ総和へ集約するAPIとして実装済み。
- ABC247 Ex: block内置換を総cycle数別に数える上昇階乗積APIとして実装済み。
- ABC247 G: 重み付き二部matchingのサイズ別最大値APIとして実装済み。
- ABC246 Ex: 許容文字集合を一点更新できる動的distinct subsequence構造として実装済み。
- ABC246 G: 需要と頂点処理可能量による根付き木の残余DP・閾値探索として実装済み。
- ABC245 G: ラベル別multi-source Dijkstraと異なるラベルへの最短距離APIとして実装済み。
- ABC245 Ex: 積の剰余を指定した巨大長列の数え上げAPIとして実装済み。
- ABC244 Ex以降: H/Exを一問ずつ監査し、G問題も同じ回で続けて確認する。
- ABC251 Ex以降: H/Exを一問ずつ監査し、上表の候補より問題から抽出した抽象を優先する。
- ABC406 G以降: H/Exの一巡後にG問題へ戻り、既存部品の再利用verifyもcoverageへ明記する。

## 実装開始条件

次のうち二つ以上を満たす候補を優先する。

- 実装が概ね20行以上、または境界条件が多く、コンテスト中の再実装リスクが高い。
- 二つ以上の公開問題、あるいは明確に異なる二つ以上の制約模型へ適用できる。
- 問題文に現れる語から30秒程度でheaderを発見できる名前と選択ガイドを用意できる。
- property test用の愚直解を独立に書ける。
- 既存のcoreを組み合わせるだけでは利用側に同じ状態管理が繰り返し残る。

数行の式変形、短いgreedy、問題の登場人物だけを置き換えた関数は追加しない。
