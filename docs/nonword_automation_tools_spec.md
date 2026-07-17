# Non-word 愚直解高速化ツール群 仕様書

## 0. 位置付け

この文書は、`word-dp` 以外の「小さい入力なら正しい愚直解」から提出用の高速 C++ を作る機能を設計する。対象 command は次のとおりとする。

| command | 内部 plugin | 対象 | 優先度 |
| --- | --- | --- | --- |
| `poly-fit` | `sequence/poly_fit` | 一変数多項式・bounded 準多項式 | P0 |
| `gf-guess` | `sequence/gf_guess` | 有理母関数候補 | P1 |
| `gf-guess --families algebraic,d-finite` | `sequence/gf_guess_advanced` | 代数的・D-finite 候補 | P3 |
| `bag-dp` | `composition/bag_dp` | 順序を持たない個数 vector | P1 |
| `operator-id` | `operator/identify` | 遅い線形作用素・遷移 kernel | P1 |
| `grid-dp` | `frontends/grid_dp` | 固定幅 grid の列・行合成 | P1 |
| `property-scout` | `optimization/property_scout` | DP 性質と反例の report | P2 |
| `dp-opt` | `optimization/dp_opt` | 愚直 DP の高速化条件探索 | P2 |
| `tree-dp` | `composition/tree_dp` | 根付き木・ranked term | P2 experimental |
| `graph-dp` | `composition/graph_dp` | nice tree decomposition 上の DP | P3 research |

既存 command との境界は次のとおりである。

- `seq` は項の収集、Berlekamp--Massey、周期候補、共通 validation を担当する。
- `poly-fit` と `gf-guess` は `seq` の model plugin であり、同じ項 cache と holdout を使う。独立した CLI 名は、コンテスト中に探索 family を明示するための alias である。
- `fps` は既知または推定済みの式を計算 DAG へ落とす。`gf-guess` は式候補を発見し、採用後に `fps` へ渡す。
- `grid-dp` は新しい線形代数 engine ではない。局所規則から `state-dp` へ、scalar grid oracle から列 token を作って `word-dp` へ渡す frontend である。
- `operator-id` は作用素の family を同定する。既知の subset 式を直接変換する `subset-transform` より根拠が弱くなり得る。
- `tree-dp` と `bag-dp` は `word-dp` と context learner、有限体線形代数、CEGIS、codegen の基盤を共有する。

任意の C++ や任意の問題文から解法を発見することは非目標である。対象を組み立てる操作、oracle の意味、係数 domain、探索する model family は typed schema で与える。

## 1. 共通利用手順

コンテスト中の標準操作は全 command で統一する。

1. 小さい対象を正しく解く `oracle.cpp` または typed recurrence DSL を書く。
2. alphabet、constructor、変数 domain、mod、制約、target query を job schema に記す。
3. `tools/ac.py <command>` を `--risk checked` で実行する。
4. tool は training corpus を作り、複数候補を合成する。
5. 候補選択に使う validation と、選択後に一度だけ使う final holdout を分離する。
6. 不一致があれば反例を保存・縮小し、basis、model bound、候補 family を更新する。
7. compile、bundle、stress、benchmark を通した候補だけを `.ac-work/<job>/selected/` に出す。
8. ユーザーは生成された `.hpp` または `solution.cpp` を `oj-bundle` して提出する。

oracle はジャッジ上で実行しない。提出物に入るのは生成されたアルゴリズムと必要最小限の `cp/...` header だけである。

### 1.1 共通 oracle contract

- 同じ入力に同じ出力を返す。
- 空対象、最小対象、境界値の規約を明示する。
- signed overflow、未初期化値、unordered container の非決定順序へ依存しない。
- prime field、整数、exact rational、min-plus などの domain を明示する。
- parameter により model 自体が変わる場合、parameter を固定した job、symbolic parameter、補間対象のいずれかに分ける。
- batch 呼び出しを既定とし、optional に構築途中の snapshot を再利用できる。
- oracle executable、source、compiler flags、mod、job schema の hash を cache key に含める。

### 1.2 共通 evidence

plugin 固有の根拠は必ず次の二軸で表示し、共通 global evidence と同じ field へ混ぜない。

| 軸 | 例 |
| --- | --- |
| problem から model への根拠 | `PROVED_FROM_AST`、`USER_ASSERTED_BOUND`、`INFERRED_MODEL` |
| model から code への根拠 | `FINITE_COMPOSITION_CERTIFICATE`、`ALGEBRAIC_CERTIFICATE`、`HOLDOUT_CONSISTENT` |

共通 global evidence は master spec の `EXACT / EXHAUSTIVE_ON_DECLARED_DOMAIN / CONSISTENT` だけを使う。`EXACT` と表示できるのは、typed AST からの意味保存変換、または機械的に導いた model 上限の下で十分な代数 certificate を作れた場合である。有限宣言 domain の全検査は `EXHAUSTIVE_ON_DECLARED_DOMAIN` とする。ユーザー仮定に依存する certificate は plugin metadata に `THEOREM_CONDITIONAL` badge を残すが、global evidence は `CONSISTENT` とする。有限 sample から family 自体を推測した場合も、何件一致しても `CONSISTENT` のままとする。

### 1.3 risk mode

- `exact`: `EXACT` だけを生成する。ユーザー仮定に依存する `THEOREM_CONDITIONAL` は含めない。
- `checked`: `THEOREM_CONDITIONAL` badge 付き候補と、独立 holdout、構造的 testcase、長さ・サイズ外挿を通った `CONSISTENT` も生成する。既定値。
- `gamble`: deadline 内の最良候補を生成するが、未検査 domain、残存候補数、最初に不足した certificate を先頭へ書く。

既知の反例がある候補、未コンパイル候補、制約上 TLE/MLE が明白な候補は `gamble` でも選ばない。

## 2. `bag-dp`: 可換 composition の復元

### 2.1 model

種類数 `k` の個数 vector `c = (c_0, ..., c_{k-1})` に対し、次の有限次元表現を対象にする。

~~~~text
f(c) = u^T M[0]^c_0 M[1]^c_1 ... M[k-1]^c_{k-1} v
M[i] M[j] = M[j] M[i]
~~~~

順序に意味がない多重集合、種類ごとの要素追加、巨大な出現回数を持つ数え上げが対象である。入力順に意味があるなら `word-dp` を使う。

### 2.2 入力

~~~~cpp
Mint naive(std::span<const std::uint64_t> count);
~~~~

job には次を記す。

- 種類数と各 token 名
- 係数体と mod
- count の学習上限と target 上限
- hidden rank の既知上限、または自動探索上限
- 一件だけか、多数 query か
- optional な count 間の制約
- optional な word oracle。与えられた場合は同じ multiset の random permutation を比較し、順序非依存 contract の反例を探す。

proof 用 oracle は原則 `N^k` 全域で定義する。`sum(count)=N` のように加算で閉じない target 制約を Hankel context の枝刈りへ使ってはならない。制約は target / validation だけへ適用するか、domain 外にも値を与えることが数学的に正しい total extension を別途示す。

想定 CLI:

~~~~text
python3 tools/ac.py bag-dp oracle.cpp \
  --kinds red,blue,green \
  --mod 998244353 \
  --auto-dimension --max-dimension 32 \
  --target many-queries --risk checked
~~~~

### 2.3 復元

count vector を canonical word とみなし、`word-dp` の context signature と incremental basis を再利用する。ただし順列を列挙せず、全候補を個数 vector で cache する。

1. `0` vector を basis 候補にする。
2. 現在の独立 basis `b` に各単位 vector `e_i` を足した `b+e_i` だけを調べる。
3. 左右 context との値 `f(p+q)` から有限 Hankel table を作る。
4. 各 generator `M[i]` を復元する。
5. 全 `i,j` で commutator `M[i]M[j]-M[j]M[i]` が零か検査する。
6. 未使用の疎・密・極端な count vector と巨大 count の分割恒等式で検証する。

proof mode では total degree `D-1` 以下の全 count context を canonical に列挙する。context 数と query 数の目安は次である。

~~~~text
|U| = binom(D - 1 + k, k)
queries ~= |U| * (1 + k * r)
~~~~

既知の rank 上限 `D` と有限次元可換線形表現の存在を前提に、全 proof context と closure residual を検査できれば、rank `r<D` でも `THEOREM_CONDITIONAL` とする。問い合わせる count の total degree は最大 `2D-1` とする。rank `D` の full-rank minor が得られれば早期終了候補にできるが、adaptive 探索での rank `r<D` plateau や有限 box の一致だけなら `CONSISTENT` である。

### 2.4 codegen

- 小 count: sparse matvec の反復
- 巨大 count: 各 `M[i]` の二乗 table
- 多数 query: power table を共有
- `k` が大きく count が疎: 非零種類だけ処理
- 同一行列を持つ種類: token class を統合
- 小 rank: sparse / unrolled matrix-vector

復元 rank `r` のみを生成し、探索上限 `D` まで padding しない。

### 2.5 失敗条件と受け入れ試験

- token の並べ替えで oracle 値が変わる意味を schema が含む場合は `word-dp` へ誘導する。
- commutator が非零なら bag model を拒否する。
- count の加算 overflow、複合数 mod での field elimination、rank 上限違反を拒否する。
- proof mode で加法閉でない target domain を context 枝刈りに使うことを拒否する。
- `k=1` は `seq`、明示順序付き oracle は `word-dp` と相互比較する。
- 可換な非対角 2 行列、rank 落ち、zero function、巨大 count、多数 query を fixture にする。
- shallow な有限 box だけ rank plateau し後で新状態が現れる例と、`sum(count)=N` を不正に proof 枝刈りする例を fixture にする。

## 3. `tree-dp`: tree context からの復元

### 3.1 model

arity が既知の label 集合を持つ根付き木を ranked term とみなす。label `a` の arity を `m` とすると、状態は multilinear map で合成される。

~~~~text
state(node_a(t_1, ..., t_m))
  = T[a](state(t_1), ..., state(t_m))
f(t) = answer(state(t))
~~~~

`m=0` は leaf vector、`m=1` は行列、`m=2` は三階 tensor に対応する。tree Hankel table は、一つだけ穴を持つ context `C` と subtree `t` に対して `H[C,t]=f(C[t])` とする。

### 3.2 入力と対象表現

~~~~cpp
Mint naive(const RankedTreeView& tree);
~~~~

tooling 用 `RankedTreeView` は node label、children の連続範囲、root を持つ read-only view とする。提出 library の公開 API には入れず、`tools/helpers/` の adapter として提供する。

想定 CLI:

~~~~text
python3 tools/ac.py tree-dp oracle.cpp \
  --oracle-fn naive \
  --alphabet tree_symbols.json \
  --mod 998244353 \
  --dimension-bound 12 \
  --max-arity 2 --risk checked
~~~~

job には次を記す。

- label、arity、root 規約
- label ごとの追加 parameter domain
- mod / coefficient domain
- node 数、深さ、次数の oracle 上限
- hidden rank の上限または探索上限
- target が一木、複数木、rerooting、dynamic tree のどれか

oracle は ranked schema から生成できるすべての well-formed finite term に値を返す。label の追加 parameter が無限集合なら有限 alphabet ではないため、有限 token class へ落とす、degree bound 付き補間へ渡す、または対象外として拒否する。

### 3.3 復元 algorithm

1. leaf と小さい term から subtree basis `Q` を作る。
2. 一穴 context basis `P` を作り、signature `h(t)=(f(C[t]))_{C in P}` を計算する。
3. rank を増やした subtree だけを basis に残す。
4. label `a` と basis child の全 tuple に対し `node_a(q_1,...,q_m)` の座標を解く。
5. 得た tensor から候補 tree DP を作る。
6. 学習に使わない木、path、star、balanced、skew、同型 relabel、局所 mutation で検証する。
7. 反例木を root-to-leaf path で分解し、区別に必要な subtree と context を CEGIS へ追加する。

rank `r`、arity `m` の symbol 集合を `Sigma_m`、dual context 数を `|P|` とすると、tensor 復元の membership query 主項は次である。

~~~~text
sum_m |Sigma_m| * |P| * r^m
~~~~

`|P|=r` の binary symbol 一つでも `Theta(r^3)` query になる。codegen の dense contraction も概ね一頂点 `O(r^3)` なので、実行前に oracle query と提出時演算の両方を見積もり、tensor の sparse 化、対称性、低 rank 分解を portfolio に含める。

v1 の証明付き終了は保守的に限定する。機械的に導いた上限 `D` またはユーザー仮定の上限 `D` に対し、rank `D` の非特異 Hankel minor を得て、subtree basis、dual context basis、全 constructor tuple の closure を検査できた場合だけ、上限の由来に応じて `EXACT` または `THEOREM_CONDITIONAL` とする。rank `r<D` で深さを増やして plateau しただけの候補は、membership query だけでは後から現れる状態を排除できないため v1 では `CONSISTENT` とする。

### 3.4 codegen backend

- rooted postorder DP
- sparse tensor contraction
- child 順序が無意味な label の symmetric tensor
- 二分木化した多子 merge
- 同じ子を多数持つ場合の repeated bilinear action
- P2: rerooting 用 context action
- P3: Static Top Tree 上の dynamic composition

初期版は arity `0..2`、prime field、rooted static tree に限定する。一般次数の木は associative な binary merge が schema で与えられる場合だけ二分木化する。

次数無制限の通常の木は、将来 `empty_children`、`append_child(accumulator, subtree)`、`finish_node(label, accumulator)` という many-sorted child-list algebra が与えられる場合に扱う。子順序を無視する場合は、oracle の子 permutation 検査と symmetric tensor 検査を必須にする。

### 3.5 失敗条件と受け入れ試験

- child 順序の有無が schema と oracle で矛盾する。
- nullary constructor がなく有限 tree を一つも構成できない。
- 同型木で値が変わるのに ordered / labeled 情報が不足する。
- arity、rank、query 数、tensor 非零数が deadline を超える。
- merge が非線形、min-plus、入力値依存なのに prime-field multilinear engine を選んでいる。
- leaf-only、unary path、ordered binary、unordered binary、rank 落ち、zero functionを fixture にする。
- 復元 tree DP を node 数の小さい全木と random larger tree で oracle と比較する。
- 浅い木だけ plateau して深い木で新状態が現れる例、左右 tensor を反転した例、query budget 超過を生成前に拒否する例を含める。

## 4. `grid-dp`: 固定幅 frontend

### 4.1 二つの入力 mode

`rule` mode は cell と近傍の局所規則を typed DSL で与え、frontier state と遷移を全列挙する。この経路は初期状態、全 cell / slice transition、invalid 遷移、終端判定、answer 射影まで列挙 domain が閉じていることを検査できた場合だけ `EXACT` にできる。

`oracle` mode は固定した slice 幅 `W` の小さい grid に対する scalar oracle を与える。各 slice の pattern を token として `word-dp` へ渡す。

~~~~cpp
Mint naive(const SmallGridView& grid);
~~~~

想定 CLI:

~~~~text
python3 tools/ac.py grid-dp oracle.cpp \
  --oracle-fn naive \
  --slice-width 5 --scan-axis columns \
  --cell-alphabet empty,blocked \
  --slice-encoding bitmask \
  --mod 998244353 --risk checked
~~~~

schema 名は `slice_width`、`slice_count`、`scan_axis`、`bit_order` に固定する。`scan_axis=columns` なら一 token は高さ `slice_width` の一列で、`slice_count` は列数である。slice 幅は job ごとに固定し、slice count 0 の grid、境界条件、bit `r` と slice 内位置 `r` の対応を oracle と生成 code で共有する。runtime slice 幅が学習値と違う、未知 cell がある、legal slice の列挙が入力 domain を覆わない場合は拒否する。

### 4.2 pipeline

1. `slice_width W`、`scan_axis`、cell alphabet を確定する。
2. slice pattern を整数 token へ encode する。
3. obstacle、weight、label が同じ遷移を持つ token class を探索する。
4. rule mode は frontier state graph を `state-dp` へ渡す。
5. oracle mode は列列を `word-dp` へ渡す。
6. explicit width、巨大 width、列 update のいずれかに合わせて scan、power、segment tree を選ぶ。

cell 種類数を `c` とすると alphabet `A` は最悪 `c^W` になる。proof mode の word context は次の規模になる。

~~~~text
|U| = 1 + A + ... + A^(D-1)
queries ~= |U| * (1 + A * r)
~~~~

全 slice pattern を先に作らず、target と basis 探索で必要になった token を lazy に問い合わせる mode は `checked` 用である。`exact` では judge 入力に現れ得る全 legal slice を certificate 対象に含める。未問い合わせ token が judge 入力へ現れ得るなら、codegen 前に全 token を被覆するか、対応外として拒否する。

### 4.3 初期対応範囲

- domino / tile の局所配置
- 独立集合、被覆、隣接禁止
- 一列または少数列だけを跨ぐ制約
- 列 obstacle pattern
- P2: connectivity partition を持つ plug DP

grid の両辺が入力とともに大きくなり、固定幅 representation が存在しない場合は対象外である。

acceptance test は、幅 1、長さ 0、左右非対称遷移、obstacle 付き独立集合、domino tiling、transpose、同一列の長い run、periodic columns、point update、unknown cell、width mismatch を含める。未学習 slice が target に現れる例、`scan_axis / slice_width / bit_order` を一つずつ壊す例、rule mode の終端判定だけを壊す例を fail-closed で検出する。

## 5. `operator-id`: 線形作用素の構造同定

### 5.1 入力 mode

安全性の高い順に三つ用意する。

1. `kernel-ast`: `K(n,i,j)` の typed AST を与える。
2. `kernel-oracle`: `K(n,i,j)` を直接評価する。
3. `apply-oracle`: vector `x` に対する `T_n(x)` だけを評価する。

~~~~cpp
void apply(int n, std::span<const Mint> x, std::span<Mint> y);
Mint kernel(int n, int i, int j);
~~~~

想定 CLI:

~~~~text
python3 tools/ac.py operator-id operator.json \
  --adapter kernel.cpp \
  --risk checked --emit report,cpp,hpp,replay
~~~~

反復遷移なら `--preset kernel-dp --steps T` を追加する。job には `y[i] += K(i,j)x[j]` という向き、square / rectangular、`n` と parameter の domain、一回適用か反復か、in-place 可否を記す。

同定候補は次の family とする。

- diagonal / permutation / banded
- prefix / suffix / difference
- Toeplitz、Hankel、circulant、通常 convolution
- XOR / AND / OR convolution、subset / superset zeta
- divisor / multiple zeta
- block diagonal / block circulant
- Kronecker product
- bounded rank `K(i,j)=sum_t a_t(i)b_t(j)`
- polynomial kernel と有限個の moment
- sparse irregular matrix

### 5.2 probe と候補選択

1. 零 vector、basis vector、少数の deterministic dense vector を評価する。
2. 加法性・斉次性を検査し、反例があれば linear family を拒否する。
3. 小 `n` では全 basis を評価して行列を復元する。
4. index shift、bit operation、block、rank の fingerprint を計算する。
5. 一致した template の parameter を解く。
6. training に使っていない `n`、basis、random vector、境界 index で比較する。
7. naive、同定した高速候補、既知 transform 候補を compile・benchmark する。

固定された有限 `n` の全 basis を調べれば、線形性を contract としたときのその `n` の作用素は厳密に復元できる。しかし `apply-oracle` の有限 probe は線形性自体を証明せず、任意の `n` への pattern 外挿も証明にならない。`kernel-ast` が登録済み恒等変形に一致した場合だけ任意 `n` へ `EXACT` とし、black-box の複数サイズ一致は `CONSISTENT` とする。

### 5.3 codegen

- naive threshold 付き NTT / arbitrary-mod convolution
- FWT、subset transform
- prefix / difference の in-place 安全な loop
- low-rank `O(rn)` apply
- sparse CSR apply
- Kronecker の軸別 apply
- 同じ作用素の反復なら `state-dp`、`seq`、matrix power へ接続

transform の正規化、inverse 係数、padding 長、cyclic / linear convolution、index 反転、in-place 更新順を model JSON と fixture で固定する。

反復では family が積に閉じるかを別に検査する。diagonal は scalar power、permutation は cycle decomposition、`K=UV^T` は小行列 `V^TU` へ縮約できる。circulant の pointwise power は係数体に必要な長さ `n` の root of unity が存在し、`n` が可逆で、その長さの変換で実際に対角化できる場合だけ使う。条件を満たさなければ kernel polynomial を `x^n-1` で剰余しながら convolution 付き二乗法を使うか候補を拒否する。XOR convolution も inverse normalization の可逆性を検査する。一般 Toeplitz 行列は積で Toeplitz に閉じないため、一回の高速 convolution をそのまま巨大反復へ外挿しない。

### 5.4 失敗条件

- 線形性 probe に反例がある。
- 複数 family が holdout 後も区別不能で、計算量または出力が異なる。
- target `n` が学習 domain と異なるのに外挿根拠がない。
- NTT 長、mod、inverse、overflow の前提を満たさない。
- 高速候補が naive より遅い、または memory 制約を超える。

affine 作用素は schema に `affine: true` があり、定数座標を一つ追加する場合だけ扱う。未宣言の定数項は線形性違反として拒否する。

acceptance test は zero、identity、scaled permutation、banded、prefix / suffix、linear convolution と circulant の区別、Hankel の index 反転、XOR inverse 係数、subset / superset の向き、rank 1 / rank `r`、Kronecker、奇数 `n` だけ壊れる偽候補、非線形 apply、複合数 mod を含める。反復 backend は同じ作用素の愚直反復とも比較する。

## 6. `poly-fit` / `quasi-fit`

### 6.1 model family

次をこの順に試す。

1. P0: 定数、一変数 polynomial
2. P0: period 上限付きの residue class ごとの quasipolynomial
3. P1: `n0` 上限をユーザーが与える eventual polynomial / quasipolynomial
4. P2: 少数変数の total-degree bounded polynomial

polynomial-exponential `sum_j P_j(n) lambda_j^n` は補間ではなく recurrence / Prony 型なので、`gf-guess` または `seq` へ送る。

### 6.2 入力

~~~~cpp
Value naive(std::span<const std::int64_t> parameter);
~~~~

job には domain、次数上限、周期上限、eventual offset 上限、整数か prime field か、target point を記す。次数・周期が不明な場合は budget 内で小さいものから探索する。

想定 CLI:

~~~~text
python3 tools/ac.py poly-fit \
  --oracle ./brute \
  --mod 998244353 \
  --degree auto --period auto \
  --max-degree 12 --max-period 60 \
  --target 1000000000000000000 \
  --risk checked
~~~~

### 6.3 合成と検証

- consecutive point は有限差分を優先する。
- arbitrary point は Lagrange / Newton interpolation を使う。
- quasipolynomial は各 residue class を独立補間する。
- `n0`、period、degree を小さい順に探索し、記述長で tie-break する。
- candidate を選ぶ validation point と、選択後の final holdout を分ける。
- 整数列は可能なら exact integer と複数 prime で比較する。
- multivariate は simplex と box の両方から holdout を取る。

prime field mode では `degree < mod` と、補間点が field 上で相異なることを要求する。quasipolynomial は residue `r=n mod q` を整数上で選び、各 class では `t=(n-r)/q` を変数にする。これにより係数と評価点を不必要に大きくしない。

次数・周期・開始位置の上限と family membership が問題構造またはユーザー仮定として与えられ、その範囲で一意に補間した場合は `THEOREM_CONDITIONAL` とする。自動探索で見つけただけなら `CONSISTENT` である。

### 6.4 codegen と失敗条件

- 連続点 Lagrange の `O(d)` 評価
- Newton basis / falling factorial basis
- residue class dispatch
- 多数 query 用 multipoint evaluation
- target が全区間なら差分から順次生成

点の差が係数環で可逆でない、必要 factorial が mod で零、次数が sample 数に近すぎる、period が増え続ける、holdout が不一致なら拒否またはより一般の `seq` 候補へ戻す。自動 eventual offset は末尾だけを補間する過適合を起こしやすいため、v1 ではユーザー上限を要求し、offset を増やすほど model penalty を大きくする。

acceptance test は定数、一次、高次、integer-valued polynomial、負係数、period `2/3/6`、真の period が候補の約数になる例、residue ごとに次数が違う例、`floor(n/3)`、prefix だけ一致して壊れる列、eventual 列、random 列、field 上の点衝突を含める。Horner、binomial basis、difference backend を相互比較する。

## 7. `gf-guess` / `fps-synth`

### 7.1 責務分割

`seq` が oracle から項を収集し、`gf-guess` が方程式候補を発見し、`fps` が既知方程式を高速計算する。推測と意味保存 codegen を同じ evidence にしない。

想定 CLI:

~~~~text
python3 tools/ac.py gf-guess \
  --oracle ./brute \
  --mod 998244353 \
  --target 1000000000000000000 \
  --families rational,algebraic,d-finite \
  --max-x-degree 8 --max-y-degree 4 \
  --risk checked
~~~~

### 7.2 candidate family

OGF / EGF、全係数か第 `K` 項か、prime field 列か整数列を job で区別する。軽い順に次を探索する。

1. P1: hypergeometric ratio `p(n)a[n+1]=q(n)a[n]`
2. P1: constant-coefficient recurrence / rational GF `Q(x)F(x)=P(x)`
3. P3: algebraic GF `A(x,F(x))=0`
4. P3: P-recursive recurrence
5. P3: D-finite differential equation `sum_i P_i(x)F^(i)(x)=0`

各 family は order、`x` degree、`F` degree、coefficient size の明示上限を持つ。全係数が必要な target と、一つの巨大 index だけ必要な target を分け、対応 backend がない候補を採用しない。

### 7.3 復元

1. training prefix を作る。
2. 小さい model bound から homogeneous linear system を解く。
3. scale ambiguity を canonical normalization する。
4. monomial order と model complexity で最小 relation を選び、既知 relation の多項式倍を同一候補として除去する。primitive factor と branch を検証しても異なる最小 relation が残る場合だけ `UNDERDETERMINED` とする。
5. 未使用係数で式の residual を検査する。
6. prefix を延ばして同じ最小 model が安定するか調べる。
7. 整数 oracle は複数 prime でも degree / order を比較する。
8. 計算可能な候補だけ `fps` cost planner へ渡す。

algebraic candidate では `x^i F(x)^j` の係数列から nullspace を求める。真の relation `A` があれば `xA` や `FA` も nullspace に残るため、nullity が 1 になるまで追加項を要求する方法は使わない。monomial order、content 除去、既知 relation の多項式倍除去、factor ごとの初期値・branch 検査で primitive candidate を選ぶ。未知係数数に対して十分な surplus coefficient を必須とし、`x` degree を training 長近くまで上げて切り詰め列そのものを埋め込む候補を禁止する。hypergeometric candidate は零項を除算せず、積形式の関係のまま検証する。

有限係数から未知の無限級数がその family に属することは証明できない。family と次数上限が仮定され、十分な独立方程式で候補が一意になった場合だけ `THEOREM_CONDITIONAL`、それ以外は `CONSISTENT` とする。typed recurrence AST へ候補方程式を代入して恒等的に検査できれば `PROVED_FROM_AST` へ昇格できる。

### 7.4 backend

- rational GF: Berlekamp--Massey、Kitamasa、Bostan--Mori
- algebraic GF の先頭 `N` 項: Newton iteration
- P-recursive: 係数順次生成、可能な場合の polynomial matrix product
- D-finite: 係数 recurrence へ変換し、対応範囲だけ生成

「方程式を推測できた」と「巨大第 `K` 項を制約内で計算できる」を分離する。後者の backend がなければ report だけを出し、提出候補を生成しない。

algebraic GF は `A(0,F(0))=0` と `partial A / partial F (0,F(0))` の可逆性を検査し、branch が一意な場合だけ汎用 Newton code を生成する。初期版は先頭 `N` 項の生成に限定し、一般の algebraic GF の巨大第 `K` 項を自動的に高速化しない。

有限体上の P-recursive / D-finite 推定は P3 でも保守的に扱う。v1 は training、holdout、必要な微分次数・factorial が characteristic 未満で、導出した係数 recurrence の leading coefficient が生成対象 index で可逆な場合に限定する。target が singular residue を跨ぐなら、そこを跨げる別 relation と十分な初期値がなければ codegen を拒否する。`D(F^p)=0` など Frobenius 固有の偽関係を避けるため、exact integer / rational 復元または複数 prime の比較を優先するが、複数 prime 一致だけを証明とはしない。

### 7.5 失敗条件

- candidate space が非一意のまま deadline に達する。
- equation の初期条件、分枝、`F(0)` が特定できない。
- Newton step に必要な微分が非可逆。
- holdout residual が非零。
- 式は正しそうでも target complexity を満たす backend がない。
- characteristic 固有の relation、または leading coefficient の singular residue を排除できない。

P1 acceptance は Fibonacci 型 rational GF、分子分母に共通因子がある列、零項を含む hypergeometric 列、prefix だけ一致する偽列、random 列、mod ごとに rank が落ちる例を含める。P3 acceptance で Catalan 型 algebraic GF、複数 branch、特異な導関数、factorial / binomial 型 P-recursive、Frobenius 固有関係を追加する。巨大一係数へ非対応の algebraic / D-finite backend を選ばないことも検査する。

## 8. `dp-opt`: 愚直 DP 最適化候補の生成

`property-scout` は性質と反例を報告する read-only command、`dp-opt` はその証拠から高速候補を生成して差分検査する command とする。opaque な `solve_brute()` しかない場合は内部遷移を取得できないため、`property-scout` の限定 report までとし、提出コードを捏造しない。

~~~~text
python3 tools/ac.py property-scout dp_job.json \
  --deadline-ms 30000 --emit report,counterexamples
python3 tools/ac.py dp-opt dp_job.json \
  --risk checked --emit report,cpp,hpp,replay
~~~~

### 8.1 入力 contract

arbitrary C++ の loop を解析せず、次のいずれかを要求する。

- typed recurrence DSL
- `cost(i,j)`、遷移可能範囲、`min/max` を分離した oracle
- 小 `n` の DP table と argmin / argmax trace
- 入力 testcase generator と愚直 solver

順序付き最適化の初期版は整数 score に限定し、中間比較に原則 `__int128` を使う。modular value、NaN を含む floating value、比較誤差規約のない実数 oracle は拒否する。`cost` の計算量、`INF` の表現、min / max、同点を最左・最右のどちらにするかを job に記す。

初期 DSL は次の形に限定する。

~~~~text
next[i] = min_{j in [L(i), R(i))} (prev[j] + cost(j, i))
dp[l][r] = min_{k in (l, r)} (dp[l][k] + dp[k][r] + cost(l, r))
~~~~

### 8.2 property family

- predicate monotonicity / unimodality
- argmin monotonicity
- Monge / anti-Monge、quadrangle inequality
- total monotonicity
- Knuth の opt 区間条件
- `cost(j,i)=a[j]x[i]+b[j]` 型の affine separation
- convex / concave slope order
- translation invariance、Toeplitz cost
- low rank / separability
- prefix、convolution、sliding-window への rewrite

ここで reduction の演算を混同しない。sum-product 遷移の Toeplitz は通常 convolution として `operator-id` へ渡せるが、`min_j(prev[j]+g(i-j))` は min-plus convolution であり NTT へ変換できない。min-plus では convex kernel、Monge、distance transform 等の専用条件が別に証明できた場合だけ高速候補を作り、通常体上の low rank fingerprint も min-plus 最適化の根拠にしない。

### 8.3 pipeline

1. schema の全小 domainまたは生成 testcase で cost と argmin table を集める。
2. 各 property の最小反例を探索する。
3. 通った property から D&C、Knuth、SMAWK、CHT、Li Chao、deque、convolution 候補を生成する。
4. 同点 argmin は最左・最右の両規約を検査する。
5. 高速候補を愚直解と独立 stress し、反例を property 検査へ戻す。
6. 制約最大付近で benchmark し、naive を含む portfolio から選ぶ。

property は真偽だけでなく scope を持つ。

- `ALL_DECLARED_INSTANCES`: typed AST の規則で全宣言入力へ示した。
- `EXHAUSTIVE_ON_DECLARED_DOMAIN`: 有限 schema の全入力を列挙した。
- `CURRENT_INSTANCE`: 一つの固定入力だけで全 index を調べた。
- `SAMPLED_INSTANCES`: corpus 上で反例がなかっただけ。

一つの cost matrix の全四点を検査しても、未知の judge 入力全体に対する証明へ昇格させない。

### 8.4 evidence

- AST の登録恒等変形から導いた場合: `PROVED_FROM_AST`
- 宣言された有限 domain を全検査した場合: `EXHAUSTIVE_ON_DECLARED_DOMAIN`
- Monge 等の不等式を、宣言 domain 上の非負項・既知符号因子・有限全列挙へ正規化して符号検査できた場合: `ALGEBRAIC_CERTIFICATE`
- sample、乱数、有限 prefix で反例がないだけ: `CONSISTENT`
- ユーザーが property を宣言しただけ: `USER_ASSUMED`

argmin monotonicityを観測しただけで D&C optimization を `EXACT` としない。特定 testcase で成立しても、judge の全入力で成立するとは限らない。

min 用 Monge は `i<k, j<l` に対する `A[i][j]+A[k][l] <= A[i][l]+A[k][j]` で向きを固定する。完全な矩形なら隣接 `2x2` の全検査から全四点へ拡張できるが、ragged domain を `INF` padding しただけでは保存されない。SMAWK は v1 では完全矩形に限定し、実際に渡す `A[i][j]=prev[j]+cost(j,i)` が totally monotone である certificate を要求する。`cost` が Monge なら column potential `prev[j]` の加算後も Monge だが、`cost` が単に totally monotone なだけでは一般の `prev[j]` 加算後の性質を保証しない。

Knuth は標準の区間 recurrence と base に加え、`a<=b<=c<=d` に対する interval monotonicity `W(b,c)<=W(a,d)` と quadrangle inequality `W(a,c)+W(b,d)<=W(a,d)+W(b,c)`、または同等の opt 区間条件を全宣言入力について示せた場合だけ exact codegen する。sample 上の `opt[l][r-1]<=opt[l][r]<=opt[l+1][r]` は `CONSISTENT` の根拠にしかならない。

CHT は typed AST が `prev[k]+slope(k)*x(j)+intercept(k)+offset(j)` に一致した場合を優先する。有限 matrix の低 rank fingerprint だけでは一般式を生成できないため、その場合は report-only とする。傾き順、query 順、重複傾きに応じて monotone deque、座標圧縮 Li Chao、dynamic Li Chao を分ける。

### 8.5 codegen と失敗条件

- divide-and-conquer optimization
- Knuth optimization
- SMAWK
- monotone CHT / Li Chao tree
- monotone queue
- prefix / window rewrite
- sum-product AST に限定した convolution routing

空遷移、無限大、overflow、同点規約、inclusive / exclusive range、前層と同層の取り違えを共通 fixture にする。高速候補の計算量は `cost` callback の計算量も掛けて表示する。property に反例がある、最適化前後で DP 値が異なる、ragged domain へ未証明の SMAWK padding をする、CHT の積や交点比較が `__int128` でも overflow する場合は拒否する。復元経路の一致は `reconstruct: true` かつ canonical tie が出力 contract の一部である場合だけ必須とし、それ以外の argmin 差は診断情報にする。既知反例のある候補は `gamble` でも出さない。

acceptance test は、Monge matrix と一 cell だけ壊した反例、training だけ argmin 単調な偽候補、全候補同値の first / last tie、空 range、左右端最適、Knuth 成立・不成立、矩形 SMAWK、ragged matrix、重複 slope / 同一 query の CHT、all-false / all-true predicate、離散凸 plateau を含める。最終答えだけでなく可能なら全 DP cell と canonical argmin を比較する。

## 9. `graph-dp`: bounded decomposition の研究枠

treewidth 上限 `w` の graph を nice tree decomposition の term とみなす。

~~~~text
leaf
introduce_vertex(v)
introduce_edge(u, v)
forget_vertex(v)
join(left, right)
~~~~

これらを constructor として `tree-dp` の context learnerへ渡し、bag 境界上の有限状態を復元する。入力には正しい decomposition、bag 内頂点の canonical label、width 上限が必要である。

oracle contract:

~~~~cpp
Mint naive(const SmallGraphView& graph);
~~~~

job は directed / undirected、simple / multigraph、self-loop、vertex / edge label、空 graph、join 時の boundary vertex 同一視を明示する。

nice path decomposition は join を持たないが、通常の free-monoid `word-dp` へそのまま渡さない。active slot と slot label signature を sort とする typed word learner、または validity automaton と直積した total semantics を使い、slot lifetime 違反を含む invalid operation 列を scalar oracle へ問い合わせない。nice tree decomposition は join tensorを持つ many-sorted `tree-dp` へ接続する。sort ごとに別 vector space を持ち、join では左右 child の active slot、boundary vertex、slot label が一致しなければならない。

codegen 前に decomposition validator で次を検査する。

- 全頂点が一つ以上の bag に現れる。
- 各辺の両端を同時に含む bag がある。
- 各頂点を含む bag 集合が連結である。
- introduce / forget の slot lifetime が正しい。
- introduce-edge の漏れと重複がない。
- join bag が一致する。

同じ graph の異なる decomposition でも oracle と候補が一致することを構造的 holdout にする。decomposition heuristic が width `w` 以下を発見できなかったことを、treewidth が `w` を超える証明とは扱わない。

sort `s` の context 数を `|P_s|`、rank を `r_s`、constructor `sigma:s_1 x ... x s_m -> s` とすると query 主項は `sum_sigma |P_s| product_i r_{s_i}` である。join は同一 sort なら概ね `Theta(r_s^3)` になる。sort ごとの上限、full-rank minor、全 typed constructor tuple の closure が揃った場合だけ条件付き certificate とする。

初期段階では自動 tree decomposition、任意 graph oracle、可変 `w` を扱わない。canonicalization、join tensor、状態数の二重指数化が大きいため P3 research とし、先に明示状態の nice-tree-decomposition runner を作る。

acceptance test は path、cycle、tree、小 grid、空 graph、非連結 graph の複数 decomposition と、join を使う独立集合・彩色 fixture を含める。introduce-edge 漏れ、running-intersection 違反、join bag / slot label 不一致、slot 再利用違反、sort ごとの rank 上限違反、width 超過、join tensor の左右反転を fail-closed で拒否し、同じ graph の path / tree backend が一致することを検査する。

## 10. 生成物

~~~~text
.ac-work/<job>/
├── input/
│   ├── job.json
│   ├── oracle.cpp
│   └── oracle.sha256
├── cache/
├── models/
│   ├── discovered.json
│   ├── rejected.jsonl
│   └── rank_or_degree_history.json
├── cases/
│   ├── train/
│   ├── validation/
│   ├── holdout/
│   └── counterexamples/
├── candidates/
├── selected/
│   ├── generated.hpp
│   ├── solution.cpp
│   ├── bundled.cpp
│   └── evidence.json
├── report.md
└── replay.sh
~~~~

`generated.hpp` は物理一行目を `#pragma once`、二行目を `#include <bits/stdc++.h>`、三行目以降を `cp/...` の依存 include、その後を `namespace poe` の実装とする。macro 定義は生成しない。可能なら専用式を unroll し、汎用 matrix / tensor header への依存を消す。

## 11. 依存ライブラリ

ここに挙げる path は planned API であり、header を実装・検証してから `library.json` へ登録する。存在しない header を metadata だけ先に登録しない。

### 11.1 共通 P0 dependency

| header | 用途 | 主な直接依存 |
| --- | --- | --- |
| `cp/math/modint.hpp` | prime field と提出コード | core のみ |
| `cp/algebra/monoid.hpp` | 積、単位元、power の contract | core のみ |
| `cp/algebra/power.hpp` | monoid / matrix の二乗法 | `cp/algebra/monoid.hpp` |
| `cp/linalg/matrix.hpp` | dense matrix と matvec | `cp/math/modint.hpp` は template 上必須にしない |
| `cp/linalg/sparse_matrix.hpp` | sparse apply と codegen 比較 | core のみ |
| `cp/linalg/row_reduction.hpp` | pivot、消去、field operation の共通下層 | field-like value type |
| `cp/linalg/gaussian_elimination.hpp` | rank、solve、nullspace | `cp/linalg/row_reduction.hpp` |
| `cp/linalg/incremental_basis.hpp` | adaptive Hankel / probe basis | `cp/linalg/row_reduction.hpp` |
| `cp/math/crt.hpp` | 複数 prime の sanity check | gcd / extended Euclid |

`matrix.hpp` から `modint.hpp` へ固定依存させず、演算可能な value type を template parameter にする。field inverse が必要な algorithm は matrix container と分離する。

### 11.2 sequence / polynomial / FPS

| header | 利用 tool |
| --- | --- |
| `cp/polynomial/polynomial.hpp` | `poly-fit`、`gf-guess`、`operator-id` |
| `cp/polynomial/interpolation.hpp` | `poly-fit`、parameterized model |
| `cp/polynomial/multipoint_evaluation.hpp` | 多数 query の `poly-fit` |
| `cp/math/binomial.hpp` | integer-valued polynomial の binomial basis |
| `cp/convolution/ntt.hpp` | convolution、FPS |
| `cp/convolution/convolution.hpp` | naive / NTT planner の統一 API |
| `cp/convolution/arbitrary_mod_convolution.hpp` | NTT 非対応 prime の候補 |
| `cp/fps/formal_power_series.hpp` | algebraic GF の先頭項 |
| `cp/fps/implicit_equation.hpp` | 非特異 algebraic GF の Newton、P3 |
| `cp/recurrence/berlekamp_massey.hpp` | rational GF 候補 |
| `cp/recurrence/kitamasa.hpp` | 小 order の巨大項 |
| `cp/recurrence/bostan_mori.hpp` | rational GF の巨大項 |
| `cp/recurrence/p_recursive.hpp` | P-recursive 候補、P2 |

`formal_power_series.hpp` は `convolution.hpp` に依存してよいが、recurrence header から巨大な FPS 全体を無条件 include しない。Bostan--Mori は必要な polynomial / convolution だけへ依存する。

### 11.3 operator / subset

| header | 利用 tool |
| --- | --- |
| `cp/convolution/bitwise_convolution.hpp` | XOR / AND / OR convolution |
| `cp/subset/zeta_transform.hpp` | subset / superset zeta と Mobius |
| `cp/subset/subset_convolution.hpp` | ranked zeta / subset convolution |
| `cp/number_theory/divisor_transform.hpp` | divisor / multiple zeta |
| `cp/utility/prefix_sum.hpp` | prefix / difference rewrite |
| `cp/linalg/low_rank.hpp` | rank factorization と `O(rn)` apply |

### 11.4 DP optimization / runtime backend

| header | 利用 tool |
| --- | --- |
| `cp/core/infinity.hpp` | 型安全な有限上限と DP の `INF` |
| `cp/algorithm/binary_search.hpp` | monotone predicate |
| `cp/algorithm/discrete_convex_min.hpp` | 離散凸最小化 |
| `cp/dp/divide_and_conquer_optimization.hpp` | `dp-opt` |
| `cp/dp/knuth_optimization.hpp` | `dp-opt` |
| `cp/dp/smawk.hpp` | totally monotone matrix |
| `cp/structure/convex_hull_trick.hpp` | monotone slope / query の affine minimum |
| `cp/structure/li_chao_tree.hpp` | arbitrary query order の affine minimum |
| `cp/structure/compressed_li_chao_tree.hpp` | query 座標既知の Li Chao adapter |
| `cp/structure/monotone_queue.hpp` | sliding-window minimum |
| `cp/structure/segment_tree.hpp` | dynamic word / grid product |
| `cp/utility/coordinate_compression.hpp` | compressed Li Chao adapter |
| `cp/dp/frontier_state.hpp` | rule-based `grid-dp` の状態 canonicalization |
| `cp/tree/rooted_tree.hpp` | `tree-dp` の iterative postorder adapter |
| `cp/tree/rerooting.hpp` | `tree-dp` P2 |
| `cp/tree/static_top_tree.hpp` | `tree-dp` / `graph-dp` P3 |
| `cp/experimental/graph/tree_decomposition.hpp` | decomposition 型と validator、P3 |
| `cp/experimental/graph/nice_tree_decomposition.hpp` | nice 化と slot canonicalization、P3 |

monotone CHT は生成式が短い場合は codegen で専用実装を出す。安易に Li Chao tree へ統一せず、座標、傾き順、query 順の contract から最小 backend を選ぶ。

segment tree の canonical path は `cp/structure/segment_tree.hpp` とし、`segtree.hpp` との二重命名を作らない。

依存 DAG では `smawk`、D&C、Knuth を相互 include させない。dynamic Li Chao tree は座標圧縮に依存させず、`compressed_li_chao_tree.hpp` だけが `li_chao_tree.hpp` と `coordinate_compression.hpp` を include する。tree decomposition heuristic は validator / nice decomposition より上位の experimental header とし、stable graph primitive から逆依存させない。property checker、AST proof、counterexample shrinker は提出用 `cp/...` ではなく `tools/aclib/opt_scout/` または `tools/helpers/` に置く。

### 11.5 tooling dependency

必須部分は Python 3 標準ライブラリだけにする。

- `argparse` / `dataclasses` / `json`: CLI と schema
- `subprocess` / `concurrent.futures`: batch oracle、compile、portfolio
- `sqlite3`: 再開可能 cache
- `hashlib`: artifact provenance
- `random`: seed 固定 testcase
- `fractions`: 小さい exact rational の補助

重い有限体線形代数、convolution、FPS は `tools/helpers/*.cpp` を GNU++23 で compile し、同じ `cp/...` 実装を利用する。

外部依存は optional plugin とする。

| dependency | 用途 | 無い場合 |
| --- | --- | --- |
| `oj-bundle` | 最終一ファイル化 | generated header と main を出し、bundle step を WARN |
| SymPy | polynomial 恒等式、因数分解、D-finite 補助 | built-in finite-field engine の範囲だけ使う |
| Z3 | bounded CEGIS、property 反例探索 | exhaustive / random generator へ戻る |

自動 install、network access、version 自動更新は行わない。optional plugin の結果は tool 名、version、入力 hash を evidence に残す。

### 11.6 tool ごとの最小依存

| tool | 合成時の必須 header | 生成コードの標準依存 |
| --- | --- | --- |
| `poly-fit` | modint、interpolation | modint、interpolation または unrolled polynomial |
| `gf-guess` | Gaussian、polynomial、BM、FPS | 採用 backend の recurrence / FPS だけ |
| `bag-dp` | modint、incremental basis、matrix | modint、power、compact matrix または unrolled matvec |
| `operator-id` | Gaussian、low-rank、各 transform | 採用した transform 一つだけ |
| `grid-dp` | `state-dp` または `word-dp` の依存 | compact transition、必要なら segment tree |
| `dp-opt` | property checker と各候補 backend | 採用した optimization 一つだけ |
| `tree-dp` | incremental basis、matrix、tool-only tensor | generated sparse tensor、必要なら rerooting |
| `graph-dp` | tree learner、decomposition validator、tool-only tensor | nice decomposition と採用した unary / join map |

tool-only の generic tensor container は `tools/helpers/tensor.hpp` に置き、提出 library の stable API へ急いで追加しない。生成 tensor が小さければ `std::array` と非零項列へ特殊化する。

### 11.7 tooling module の依存方向

~~~~text
tools/aclib/
├── job.py, schema.py, evidence.py, corpus.py
├── oracle.py, oracle_cache.py, deadline.py
├── candidate.py, codegen.py, compile.py, portfolio.py
├── composition/
│   ├── term.py, context.py, hankel.py
│   ├── basis.py, certificate.py, cegis.py
│   └── codec.py
├── sequence_models/
│   ├── poly_fit.py, gf_guess.py
│   └── model_selection.py
├── operator_models/
│   ├── probes.py, families.py, planner.py
│   └── validation.py
└── opt_scout/
    ├── trace.py, properties.py, ast_proof.py
    ├── counterexample.py, shrink.py
    └── planner.py, codegen.py

tools/ac_plugins/
├── bag_dp/, tree_dp/, grid_dp/, graph_dp/
├── operator_id/, sequence/, fps/
└── property_scout/, dp_opt/
~~~~

下位の `job / evidence / corpus` は model plugin を import しない。`composition` は具体的な tree / bag frontend を import しない。plugin が共通 core と C++ helper を組み合わせる一方向依存にする。これにより `tree-dp` の experimental code が `word-dp` や `seq` の必須経路へ混入しない。

### 11.8 planned dependency DAG

~~~~text
core types / contracts
├── math: modint, CRT, binomial
│   ├── linalg containers
│   │   ├── Gaussian / incremental basis -> word, bag, tree learners
│   │   └── low-rank -> operator-id
│   └── polynomial
│       ├── interpolation -> poly-fit
│       ├── convolution / NTT
│       │   ├── FPS -> fps-synth / algebraic gf
│       │   └── Bostan--Mori
│       └── BM / Kitamasa -> seq / rational gf
├── algorithm / DP primitives
│   ├── binary search / discrete convex
│   ├── D&C / Knuth / SMAWK
│   └── CHT / Li Chao / monotone queue -> dp-opt
├── subset / bitwise / divisor transforms -> operator-id
└── graph / tree primitives
    ├── rooted tree / rerooting -> tree-dp backend
    └── decomposition validator -> graph-dp backend

tool-only context, tensor, CEGIS, property proof
└── uses stable headers above; stable headers never depend back on tools
~~~~

この DAG は実装順を示すもので、すべてを一つの generated source が include する意味ではない。例えば `operator-id` が prefix sum を選んだ場合、提出物は Gaussian、NTT、候補探索器を一切 include しない。

## 12. 実装順

### Phase A: 既存 P0 へ統合

1. `seq` の項 cache と family plugin contract
2. `poly-fit` 一変数 polynomial
3. bounded period の `quasi-fit`
4. rational GF と既存 BM / Kitamasa / Bostan--Mori

### Phase B: `word-dp` 基盤の再利用

1. generator を可換に制約した `bag-dp`
2. `grid-dp` の列 token frontend
3. shared context / basis / certificate API の抽出

### Phase C: operator と最適化

1. `operator-id` の diagonal / prefix / Toeplitz / bitwise transform
2. low-rank factorization
3. `dp-opt` の monotonicity、D&C、Knuth
4. SMAWK、CHT、Li Chao、sum-product 限定 convolution routing

### Phase D: 高次 composition

1. arity `0..2` の `tree-dp`
2. sparse / symmetric tensor codegen
3. rerooting context action
4. nice tree decomposition runner
5. `graph-dp` research prototype

### Phase E: advanced generating functions

1. algebraic relation の primitive factor / branch 選択
2. 非特異 implicit Newton
3. characteristic 条件付き P-recursive / D-finite
4. singular residue と target backend の fail-closed planner

## 13. 全体 acceptance 条件

各 command は最低限、次を満たす。

1. positive、境界、zero、rank / degree 落ち、意図的 invalid fixture を持つ。
2. candidate 選択に使わない final holdout がある。
3. oracle、schema、seed、compiler、生成 model から `replay.sh` で再現できる。
4. generated hpp が library header 規約を満たす。
5. `oj-bundle` 前後の出力が一致する。
6. sanitizer、GNU++23 judge flags、最大制約 benchmark を通す。
7. 複合数 mod、非可逆元、overflow、空対象、同点、左右順序を故意に壊す fixture を拒否する。
8. exact と consistent を UI、JSON、生成 source comment の三か所で混同しない。
9. optional dependency がなくても必須範囲が動き、無言で意味の違う fallback をしない。

## 14. 確定した設計判断

- 最初から万能な `object-dp` command は公開しない。具体的な `word-dp`、`bag-dp`、`tree-dp` を作り、検証後に内部共通化する。
- grid、digit、automaton の逐次処理は可能な限り `word-dp` / `state-dp` frontend とし、復元 engine を重複させない。
- `poly-fit` と `gf-guess` は `seq` の cache と holdout を共有し、`fps` は推測済み式の計算だけを担当する。
- scalar oracle だけから任意サイズへの外挿を無条件の証明と呼ばない。
- 解析 tool は多くの header を利用してよいが、生成 code は選ばれた backend の最小依存だけを include する。
- `tree-dp` の generic tensor はまず tool-only とし、実戦で再利用性が確認できるまで stable library にしない。
- max-plus / min-plus、noisy floating、任意 graph は field 上の learner と同じ保証で扱わない。

## 15. 関連文書

- [AC 自動化ツール群仕様](ac_automation_tools_spec.md)
- [Black-box Word DP / Hankel 復元仕様](word_dp_hankel_spec.md)
- [要件・構想](requirements.md)
- [開発支援ツール](development_tools.md)
