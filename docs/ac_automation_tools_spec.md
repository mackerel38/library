# AC 自動化ツール群 仕様書

## 0. 文書の位置付け

- 状態: 設計案。実装着手前の機能仕様であり、実装済みを意味しない。
- 対象: GNU++23、AtCoder の ABC / ARC / AGC を主対象とする。
- 実行環境: Python 3 標準ライブラリを司令塔とし、重い処理は事前ビルドした C++ helper で行う。
- 提出コード: `poe` namespace と `cp/...` header を使い、oj-bundle で一ファイル化できる C++ を生成する。
- 関連文書: [全体要件](requirements.md)、[開発支援ツール](development_tools.md)

この文書の目的は、「解法を説明できること」ではなく「手元に書ける愚直解、有限状態、数式、判定器、小さい観測結果から、提出可能な高速コードを可能な限り機械的に作ること」である。正しさを証明できる変換を優先するが、証明できない候補も検証強度を明示した上で出力できるようにする。

## 1. 目標

### 1.1 最優先の利用場面

1. 小さい `N` なら書ける DP から、状態数定数の遷移行列や線形漸化式を取り出し、巨大 `N` 用コードを生成する。
2. 係数漸化式や FPS 方程式を入力し、式変形を人間が行わずに高速な計算 DAG と C++ を生成する。
3. 愚直解を oracle とし、複数の高速化候補を生成・差分検査・計測して、制約内で最も AC しそうな候補を選ぶ。
4. 証明が間に合わないとき、有限観測に整合する漸化式、周期、単調性、最適化条件などを「推測」と明示して提出候補まで接続する。
5. 桁 DP、profile DP、オートマトン DP、2-SAT / flow など、入力が定型化できる問題を小さな宣言からコード生成する。

### 1.2 成功条件

ツールの成功は、きれいな導出や解説の生成ではなく、次を満たす artifact を短時間で得ることとする。

- oj-bundle 可能な `solution.cpp`
- 使用する `cp/...` header の一覧
- コンパイル済みという記録
- oracle、sample、holdout、stress の実行結果
- 時間・メモリの概算または実測
- 変換の前提と、厳密変換か推測かの表示
- 失敗を再現できる seed と入力

### 1.3 非目標

- 問題文だけから万能に解法を発見すること
- 自然言語の曖昧な数式を推測で解釈すること
- 解説記事や証明文を生成すること
- コンテスト中に LLM、生成 AI、cloud API、network search を呼ぶこと
- 自動提出すること
- 既知の反例があるコードを警告だけで通すこと

任意の C++ 愚直解から一般に高速解を合成することはできない。初期版では、ユーザーが「観測したい値」「一ステップ遷移」「係数式」「出力判定」などを小さな adapter で露出させる。問題固有の核心を完全に消すのではなく、巨大化・式変形・定型帰着・候補検証をツールへ移す。

## 2. 設計原則

### 2.1 AC-first

- `FOUND + EXACT` だけでなく、有限検査を通った推測からも明示的にコード生成する。
- 候補は一つに早決めせず、複数生成して oracle との一致、速度、コード量で選ぶ。
- 理論上最良より、制約内で実装事故が少ない方式を優先する。
- 残り時間を入力でき、探索・検証・生成へ使う時間を自動配分する。
- ツールが分からない場合は、黙ってもっともらしい答えを返さず、利用可能な投機候補と不足している証拠を分けて出す。

### 2.2 決定性と再現性

- 同一 version、入力、seed、profile なら同一候補と同一検査順を得る。
- 乱数を使う全処理は seed を記録する。
- 外部 process の stdin、stdout、stderr、終了コード、時間、timeout を保存する。
- 生成コードへ generator version と job hash をコメントで埋め込む。

### 2.3 生成と検証の分離

候補を選ぶために使った testcase で、その候補の信頼度を主張しない。

1. training: 候補の合成に使用
2. validation: 候補の枝刈りと parameter 選択に使用
3. final holdout: 最終候補を決めるまで非公開
4. stress: 分布を変えた追加反例探索
5. benchmark: 正しさ用とは別の最大制約ケース

final holdout に失敗した候補を、同じ holdout を training へ移して再生成した場合、その testcase はもう final holdout と呼ばない。新しい holdout を作る。

### 2.4 根拠を偽装しない

結果と根拠を二軸で表示する。

| result | 意味 |
| --- | --- |
| `FOUND` | 一つ以上の提出候補がある |
| `REJECTED` | 指定仮説や全候補に反例がある |
| `UNDERDETERMINED` | 複数候補を区別する情報が足りない |
| `ERROR` | 入力不正、資源上限、内部失敗 |

| evidence | 意味 |
| --- | --- |
| `EXACT` | 宣言した前提の下で、構成または代数検査が完了している |
| `EXHAUSTIVE_ON_DECLARED_DOMAIN` | 宣言した有限領域を全探索しただけで、領域外は保証しない |
| `CONSISTENT` | 未使用 testcase でも反例が見つからなかっただけ |

`FOUND + CONSISTENT` は普通に有用な提出候補である。ただし `EXACT` と表示してはならない。

### 2.5 リスクモード

全 generator に共通 option `--risk` を持たせる。

| 値 | 動作 |
| --- | --- |
| `exact` | `FOUND + EXACT` の候補だけ `solution.cpp` を出す |
| `checked` | `EXACT` または規定の validation / holdout / stress を通った `CONSISTENT` 候補を出す。既定値 |
| `gamble` | 標本不足、候補の複数残り、低い検査量でも順位一位を出す。弱点を先頭に表示する |

`gamble` でも、既知の反例、未コンパイル、sanitizer failure、出力形式不正、宣言制約を明白に超える計算量は許可しない。「証拠不足」は許可するが「既知の誤り」は許可しない。

## 3. 競技中の実行条件

AtCoder の生成 AI 対策ルール 20251003 版は、開催中の ABC / ARC / AGC で生成 AI を原則禁止する一方、生成 AI を使わないルールベースの問題文解析・入出力コード生成を対象外の例としている。したがって contest profile では次を必須にする。

- モデル推論、生成 AI、AI 補完を実行しない。
- network を fail-closed で無効にする。
- package の自動 install や update をしない。
- 固定済みの規則、探索 grammar、数式変形、solver だけを使う。
- optional な SymPy / Z3 も、事前導入済みの固定 version を明示的に選んだ場合だけ使う。
- 公式ルール URL、ruleset 名、最終確認日を `tools/RULES.md` と実行 report に残す。
- サイトごとに profile を分け、AtCoder 用 profile を他サイトで安全の根拠にしない。

2026-07-13 時点の根拠は [AtCoder 生成AI対策ルール 20251003版](https://info.atcoder.jp/entry/llm-rules-ja) とする。規約は変わり得るため、主要コンテスト前と公式更新時に再確認する。

## 4. 全体アーキテクチャ

### 4.1 CLI

開発用 `tools/dev.py` と分け、提出候補生成は `tools/ac.py` に集約する。

~~~~text
python3 tools/ac.py <command> [input] [options]
~~~~

初期 command:

| command | 役割 |
| --- | --- |
| `state-dp` | 明示遷移、templated step、有限状態から遷移作用素を構築 |
| `word-dp` | 短い word の scalar oracle から、token 別の未知遷移行列を復元 |
| `seq` | scalar oracle の先頭項から漸化式・周期・多項式候補を合成 |
| `fps` | 係数式・FPS 方程式を解析し計算 DAG と C++ を生成 |
| `portfolio` | 複数候補を compile、stress、benchmark して選択 |
| `preflight` | bundle 後の提出一ファイルを最終検査 |

後続 command:

| command | 役割 |
| --- | --- |
| `poly-fit` | `seq` の alias として多項式・準多項式へ探索を限定 |
| `gf-guess` | 係数列から有理・代数的・D-finite 方程式候補を探索 |
| `bag-dp` | 個数 vector の scalar oracle から可換遷移を復元 |
| `operator-id` | 遅い線形作用素を convolution、transform、低 rank 等へ同定 |
| `grid-dp` | 固定幅 grid を `state-dp` / `word-dp` へ変換 |
| `property-scout` | DP 最適化条件と最小反例を report |
| `dp-opt` | 検査済み性質から高速 DP 候補を生成 |
| `tree-dp` | 小さい根付き木の scalar oracle から merge tensor を復元 |
| `graph-dp` | nice tree decomposition 上の many-sorted DP、研究枠 |

これらの contract、保証、依存ライブラリは [Non-word 愚直解高速化ツール群仕様](nonword_automation_tools_spec.md) に置く。

### 4.2 共通 option

~~~~text
--job NAME
--workspace PATH
--risk exact|checked|gamble
--deadline-ms INTEGER
--seed INTEGER
--profile atcoder-abc-arc-agc-20251003
--emit report,cpp,replay
--keep-all-candidates
--json
~~~~

`--deadline-ms` は提出プログラムの制限時間ではなく、ツール自身が候補生成と検証へ使える残り時間である。deadline 到達時は、完了済み検査だけを report に記録して最良候補を返す。

### 4.3 workspace

問題ごとの中間生成物を `.ac-work/` に置き、library source と混ぜない。

~~~~text
.ac-work/<job>/
├── input/                 # spec、oracle、sample、constraint
├── build/                 # helper と候補の build
├── candidates/
│   ├── 0001/
│   │   ├── solution.cpp
│   │   ├── bundled.cpp
│   │   ├── plan.json
│   │   └── evidence.json
│   └── ...
├── cases/
│   ├── train/
│   ├── validation/
│   ├── holdout/
│   └── counterexamples/
├── selected/solution.cpp
├── report.md
├── report.json
└── replay.sh
~~~~

`.ac-work/` は既定で gitignore 対象にする。保存したい問題固有 generator や regression は、ユーザーが明示的に `verify/` へ昇格させる。

### 4.4 共通 pipeline

1. parse: schema と型を検査する。
2. normalize: 状態、式、制約を canonical form にする。
3. synthesize: 決定的な候補 grammar から列挙する。
4. static check: 前提、型、法、次数、計算量を検査する。
5. codegen: `poe` API を使う GNU++23 を出す。
6. compile: header 依存と bundle 後の両方をコンパイルする。
7. validate: training とは独立の testcase で oracle と比較する。
8. refine: 反例を縮小し、候補を除外または再合成する。
9. benchmark: 最大制約近辺を測る。
10. select: 正しさの根拠、速度、メモリ、コード量の順に選ぶ。
11. report: 選んだ理由と捨てた理由を記録する。

合成と反例追加を交互に行う部分は CEGIS として共通化する。候補が反例に負けたら、その反例を全候補の必須 testcase に追加する。

## 5. 共通入出力 contract

### 5.1 oracle contract

oracle は次のいずれかとする。

- executable: stdin に一 testcase、stdout に正解を出す。
- batch executable: testcase ID と parameter 列を読み、複数結果を出す。
- C++ adapter: helper と link し、typed function を直接呼ぶ。
- Python adapter: contest profile で許可した標準ライブラリだけを使う。

各呼び出しに timeout と出力上限を設ける。浮動小数比較、複数解、順不同出力は comparator を明示する。oracle 自身の非決定性を同一入力の再実行で検査し、揺れた場合は候補合成を止める。

### 5.2 制約 contract

最低限、次を機械可読にする。

- parameter の型、最小値、最大値
- 法、係数環、overflow 意味論
- testcase の構造制約
- 提出時の時間・メモリ制限
- すべての係数が必要か、一つの値だけ必要か
- 複数 testcase の合計制約

制約未指定でも候補探索はできるが、計算量判定の evidence は弱くなる。

### 5.3 candidate contract

各候補は次を持つ。

- 一意な ID と生成規則名
- `solution.cpp` と必要 header
- 適用前提とその由来
- 漸近計算量、概算 memory
- training / validation / holdout / stress の件数
- 最初に見つかった反例、または反例なしという記録
- codegen に使った全 parameter
- `result`、`evidence`、選択順位

## 6. 状態数定数 DP 自動合成

command 名は `state-dp` とする。内部更新を見せられない場合の `word-dp` と並ぶ、本ツール群の最優先機能である。

### 6.1 解きたい形

基本形は、有限次元 vector `v_n` と一ステップ作用素 `T` に対する次である。

~~~~text
v_(n+1) = T(v_n)
answer(n) = project(v_n)
~~~~

`T` が時刻非依存かつ指定半環上で線形なら、遷移行列、線形漸化式、行列累乗などへ厳密に変換できる。有限観測しかない場合も、同じ出力形を推測として作る。

### 6.2 入力方式

#### A. emit による明示遷移

状態数、初期重み、答えへの射影と、一状態からの遷移を列挙する C++ kernel を渡す。

~~~~cpp
template<class Emit>
void transitions(int phase, int state, Emit emit) {
    // emit(next_state, weight);
}
~~~~

`emit(to, weight)` は `next[to] += current[state] * weight` を意味する。到達状態だけを列挙する場合も、列挙外状態への遷移を捨てずに拒否する。全遷移が構造として得られるため、最も強い `EXACT` を付けやすい。

#### B. templated step の symbolic lifting

ユーザーが愚直 DP の一ステップを scalar 型に依存しない C++ function として書く。

~~~~cpp
template<class Scalar>
std::vector<Scalar> step(
    int phase,
    const std::vector<Scalar>& current
) {
    std::vector<Scalar> next(S);
    // 普段の愚直遷移
    return next;
}
~~~~

helper は各 `current[i]` を symbolic basis に差し替え、各出力を次の形へ持ち上げる。

~~~~text
next[j] = b[j] + sum_i A[j][i] current[i]
~~~~

許可するのは線形式の加減算、定数倍、および可逆性を確認できる定数除算である。入力変数同士の積、DP 値に依存する分岐・比較・添字、unsupported な型変換を検出したら厳密抽出を拒否する。affine 項 `b` は常に 1 の補助次元を加えて線形化する。

min-plus など通常の演算子と意味が異なる半環では、無理な operator overload に頼らず `sr_add`、`sr_mul` などの明示 API または emit mode を使う。

#### C. numeric probe

既存 C++ をほぼ変えられない場合、`step(vector)` を basis vector と random vector で呼び、観測組 `y = A x` から行列候補を解く。

- field 上で probe が全空間を張る場合だけ候補を一意にする。
- rank 不足なら任意の一解を真の行列として返さない。
- 加法性と斉次性の追加 probe を行う。
- 値依存分岐を有限 probe だけで排除できないため evidence は最大でも `CONSISTENT`。

#### D. traced recursive state graph

一状態を受け取り、次状態と重みを返す遅い再帰・メモ化解へ adapter を付ける。初期状態から BFS して explicit transition へ変換する。state canonicalizer と列挙上限を必須にする。

#### E. scalar sequence oracle

`solve(n)` しか露出できない場合は第 7 節の `seq` へ渡す。そこから得る companion matrix は観測列の一つの実現であり、元 DP の真の内部状態とは表示しない。

#### F. scalar word oracle / Hankel realization

有限 alphabet の短い word に対する `f(word)` だけを書ける場合、`word-dp` へ渡す。次元上限 `D` 以下の線形表現

~~~~text
f(s1...sn) = u^T M[s1] ... M[sn] v
~~~~

が存在するという contract の下で、有限 Hankel table `H[p,q]=f(pq)` から各 token の遷移行列と最小の可観測表現を復元する。内部 state vector や一ステップ関数は不要であり、numeric probe `y=Ax` とも異なる。

証明 mode は長さ `D-1` 以下の全左 context で関係を検査し、最大長 `2D-1` の oracle 値から有限 certificate を作る。adaptive mode は prefix / suffix basis を CEGIS で増やし、少ない query で候補を得る。次元上限自体が推測なら evidence は `CONSISTENT` である。

詳細は [Black-box Word DP / Hankel 復元仕様](word_dp_hankel_spec.md) に置く。

### 6.3 model schema

JSON を正本とし、kernel は別 C++ file に置く。

~~~~json
{
  "kind": "linear_dp",
  "states": 6,
  "period": 2,
  "algebra": {
    "kind": "mod_prime",
    "mod": 998244353
  },
  "initial": [1, 0, 0, 0, 0, 0],
  "output": [0, 0, 1, 0, 0, 0],
  "kernel": "work/model.hpp",
  "kernel_mode": "symbolic",
  "step_convention": "answer_after_n_steps"
}
~~~~

`a_0` の時点、`N = 0`、vector の向き、初期状態、answer projection、mod / 半環、period、runtime parameter、複数 testcase 間で共有できる前計算を明示する。off-by-one を防ぐため `N = 0, 1, 2` は常に比較する。

### 6.4 対応する代数

初期版で次を扱う。

| algebra | 主な用途 | 高速化 |
| --- | --- | --- |
| prime field / modint | 個数 DP | 行列、漸化式、Bostan–Mori |
| checked integer ring | 小さい厳密値 | 行列、必要なら多倍長 helper |
| Boolean semiring | 到達可能性 | bitset、Boolean matrix |
| min-plus | 最小コスト固定長 walk | min-plus matrix |
| max-plus | 最大値固定長 walk | max-plus matrix |

行列累乗は加算、乗算、零元、単位元があれば一般半環上で使える。BM、Kitamasa、Bostan–Mori と probe の線形方程式は初期版では体上に限定する。複合数 mod へ field の手法を黙って適用しない。

### 6.5 時刻依存と周期遷移

`A_t = A_(t mod P)` の場合、列 vector 規約を固定して次を構成する。

~~~~text
v_(t+1) = A_t v_t
C = A_(P-1) ... A_1 A_0
N = qP + r
v_N = A_(r-1) ... A_0 C^q v_0
~~~~

phase を状態へ埋め込む方式と、一周期の積を累乗する方式を比較する。period をユーザーが宣言し全 phase を抽出できた場合は厳密に扱える。値列から period を推測しただけなら `CONSISTENT` である。非周期の時刻依存は初期版で拒否する。

### 6.6 状態削減

常に安全に行うもの:

- 初期状態から到達不能な state の削除
- answer へ寄与できない state の逆向き削除
- 重複辺の半環加算
- 全 phase で不変な孤立 state の削除

追加で、deterministic Boolean automaton の partition refinement と、各 block への遷移重み和が一致する weighted lumping を行う。一定長先まで値が同じだっただけの state merge は `gamble` でも独立候補として扱い、元状態版との差分検査を要求する。

field 上で answer が一つの scalar `c^T A^n b` なら、reachable Krylov subspace 上の多項式関係を使い、状態全体より低次数の漸化式へ落とせる。単なる有限列一致ではなく、既知の `A` と `b` に対して関係を代数的に検査できた場合は `EXACT` にできる。

### 6.7 backend 選択

| 条件 | 候補 |
| --- | --- |
| `N` が小さい | 元の step を反復 |
| state 数が小さい一般半環 | dense matrix exponentiation |
| 遷移が疎、`N` も中程度 | sparse matvec 反復 |
| 一般半環かつ巨大 `N` | dense / sparse matrix powers |
| field 上で scalar answer だけ | Krylov 列から漸化式を構成 |
| 漸化式次数が小さい | Kitamasa |
| 高速 convolution が使え第 `N` 項だけ | Bostan–Mori |
| 多数の `N` query | recurrence / matrix power の前計算共有 |

目安は dense matrix `O(S^3 log N)`、sparse 先頭列生成 `O(SE)`、通常 BM `O(S^2)`、Kitamasa `O(L^2 log N)` とする。最終選択は固定閾値だけでなく judge profile 上の実測 calibration を使う。

### 6.8 CLI と生成物

~~~~text
python3 tools/ac.py state-dp model.json \
  --target 1000000000000000000 \
  --risk checked \
  --emit cpp,report,replay
~~~~

生成器は dense / sparse、vector の向き、行列積順、`N = 0`、必要 header を固定し、`poe::matrix_pow` や `poe::linear_recurrence_nth` を呼ぶ GNU++23 を作る。`solution.cpp` と別に、小さい `N` で元 step と生成 backend を比較する verifier を必ず作る。

### 6.9 モデルと生成コードの二段階評価

tool が厳密に保証できるのは、多くの場合「入力されたモデルから生成コードへの変換」である。愚直モデル自体が問題を正しく表すかは別軸にする。

~~~~text
Problem -> Model: RANDOM_DIFFERENTIAL (100000 cases)
Model -> Code:    STRUCTURAL_EXACT
Overall:          model correctness remains user responsibility
~~~~

前者には `USER_ASSERTED`、`SAMPLE_MATCHED`、`BOUNDED_EXHAUSTIVE`、`RANDOM_DIFFERENTIAL`、`UNKNOWN` を使う。後者には `STRUCTURAL_EXACT`、`THEOREM_CONDITIONAL`、`TESTED_ONLY`、`INFERRED_ONLY`、`REJECTED` を使う。

### 6.10 失敗条件

- 状態数が `N` とともに増える。
- 遷移先が列挙状態集合の外へ出る。
- 未状態化の時刻依存がある。
- symbolic value 同士の積など非線形操作がある。
- numeric probe の rank が不足する。
- 係数環と backend の前提が合わない。
- period 宣言と trace が不一致。
- state canonicalizer が非決定的。
- 元 kernel、抽出作用素、生成コードのいずれかが不一致。
- 推定計算量が宣言制約を明白に超える。

失敗時は phase の追加、affine lift、emit mode、状態削減、scalar sequence、別 backend のどれを試せるか表示する。

### 6.11 acceptance test

- 6 state 程度の mod DP を symbolic lifting し、`10^18` step 用 C++ を一コマンドで生成できる。
- Fibonacci、finite automaton walk、linear cellular automaton を exact 生成できる。
- affine DP を補助次元で再現できる。
- 偶奇遷移を phase 未指定では拒否し、指定後は再現できる。
- `next[0] = current[0] * current[1]` を exact と誤認しない。
- rank 不足 probe を `UNDERDETERMINED` にする。
- dense、sparse、recurrence backend が愚直と一致する。
- `N = 0, 1` と最大 `N` を検査する。

### 6.12 word-dp との接続

`state-dp`、`word-dp`、`seq` は次の順で入口を選ぶ。

1. 内部遷移を typed / symbolic に実行できるなら `state-dp`。構造的保証が最も強い。
2. 内部状態は見えないが `f(word)` を計算できるなら `word-dp`。
3. alphabet が一種類、または `f(n)` しか得られないなら `seq`。

`word-dp` が復元した rank `r` の token matrices は、explicit scan、sparse matvec、run-length matrix power、matrix-product segment tree へ接続する。一文字 alphabet では BM / Kitamasa / Bostan–Mori も portfolio に加える。

## 7. 数列からの高速コード合成

command 名は `seq` とする。black-box oracle が小さい `n` の答えしか返せなくても、巨大 index 用の提出候補を作る。

### 7.1 候補 family

候補は安いものから次の順で試す。

1. 定数列、等比、交代符号、短い周期
2. 有限差分が消える多項式
3. residue class ごとの準多項式
4. prime field 上の定係数線形漸化式
5. 有理母関数
6. 完全な有限状態再訪からの前周期 + 周期
7. 後期機能として多項式係数漸化式、線形微分方程式、代数的母関数

最初の候補へ早決めせず、表現サイズ、追加項への安定性、目標 index の計算費用を比較する。

### 7.2 CLI

~~~~text
python3 tools/ac.py seq \
  --oracle ./brute \
  --mod 998244353 \
  --target 1000000000000000000 \
  --train 256 \
  --validation 256 \
  --holdout 512 \
  --max-order 128 \
  --risk checked
~~~~

oracle は一回に一項を返す方式と、`0..m` を batch 出力する方式を持つ。adaptive mode では prefix 長を倍増し、候補の次数・周期・係数が安定するか調べる。

### 7.3 Berlekamp–Massey 経路

初期版は prime field に限定する。

1. training prefix から最短候補漸化式を求める。
2. validation で候補の選択と backend の parameter を決める。
3. prefix 長を増やして同じ次数・係数が再現するか調べる。
4. 未使用 final holdout を初めて評価する。
5. Kitamasa、Bostan–Mori、companion matrix を生成して比較する。
6. 追加 seed、別の小 parameter、bundle 後の C++ で stress する。

次数が prefix 長の半分近くへ張り付く、prefix を増やすたび次数が増える、初期値が不足する、holdout に不一致がある場合は棄却する。

有限列への BM が保証するのは、その有限列に整合する漸化式であって未知の続きを保証しない。したがって black-box 経路は常に `CONSISTENT` である。一方、既知の有限次元線形作用素 `v_(n+1)=A v_n` から正確な項を作り、作用素または到達 Krylov 空間上で漸化式関係を検査した場合は `EXACT` にできる。

### 7.4 多項式・準多項式

- 人間が次数上限 `d` を保証し、`d + 1` 点から補間した場合は、その仮定付きの厳密変換とする。
- 次数上限を有限差分から自動推定した場合は `CONSISTENT`。
- 準多項式は period を指定する mode と、自動探索する mode を分ける。
- 自動探索時は最小 period だけでなく、その倍数による過適合を比較する。
- 離れた index、各 residue の境界、複数 prime でも検査する。
- integer-valued polynomial は monomial basis だけでなく binomial basis も候補にする。

### 7.5 周期

値が再び現れただけでは周期としない。遷移が直前 `K` 個だけから決まると構造的に分かる場合、直前 `K` 個と phase を含む完全状態の再訪から以後の周期を厳密に確定できる。black-box の値列が繰り返しただけなら `CONSISTENT` のままである。

### 7.6 複数 mod と整数列

- 整数列は複数の独立 prime へ射影して recurrence order の安定性を調べる。
- 複数 prime 上で一致しても証明とは表示しない。
- exact integer / rational mode と modular mode を分離する。
- CRT reconstruction は係数の絶対値上限がある場合だけ厳密にする。
- 複合数 mod へ BM を直接適用せず、必要なら prime-power 分解を後期 plugin とする。

### 7.7 出力

- 漸化式、初期値、母関数 `P(x)/Q(x)`
- training / validation / holdout の index 範囲
- prefix 長ごとの次数と係数 hash
- 採用 backend、計算量、必要 header
- 生成した `solution.cpp`
- oracle 項と生成項の差分表
- 「元 DP の遷移を復元したわけではない」という表示

## 8. FPS・数式変形省略 compiler

command 名は `fps` とする。係数漸化式・母関数・FPS 方程式を小さな DSL で入力し、登録済みの決定的変形だけで計算 DAG と提出用 C++ を作る。

### 8.1 入力の意味論

曖昧な自然言語や LaTeX を直接受けず、初期版は ASCII DSL と JSON IR を正本にする。最低限、次を明示する。

- coefficient domain と法
- OGF / EGF の別
- `mod x^N` の truncation
- 全係数 `[0,N)` が必要か、第 `K` 項だけ必要か
- runtime input と codegen 時の定数
- 既知 series、未知 series、初期値
- 疎性、次数、定数項などの user assumption
- 任意の小次数 oracle

OGF と EGF は次の意味で固定する。

~~~~text
OGF A(x) = sum_n a[n] x^n
EGF A(x) = sum_n a[n] x^n / n!
~~~~

### 8.2 DSL 例: 直接 FPS

~~~~text
domain mod 998244353
truncate N
var x

ogf G = x + x^2 / 2
ogf F = exp(G)

emit coeffs F [0, N)
~~~~

想定 report:

~~~~text
result: FOUND
evidence: EXACT
strategy: sparse_exp
complexity: O(2N)
assumptions:
  - 2 is invertible modulo 998244353
verification:
  - naive coefficients 0..127 matched
  - generated and bundled C++ matched
~~~~

### 8.3 DSL 例: 係数漸化式

~~~~text
domain mod 998244353
size N

sequence g[0..N)
sequence f[0..N)
assume g[0] = 0
init f[0] = 1

forall n in [1, N):
  n * f[n] =
    sum(k = 1..n, k * g[k] * f[n-k])

emit f
~~~~

登録規則により次へ変換する。

~~~~text
n f[n] = sum_k k g[k] f[n-k]
F'(x) = G'(x) F(x)
F(x) = exp(G(x))
~~~~

`g` が疎なら sparse exp、密なら Newton 法 + convolution を候補にする。

### 8.4 型付き AST と式 DAG

処理順:

~~~~text
DSL
  -> typed AST
  -> normalized FPS DAG
  -> exact rewrite saturation
  -> needed-degree backward propagation
  -> implementation candidates
  -> precondition checks
  -> cost planning
  -> naive verification
  -> C++ code generation
~~~~

DAG では共通部分式、valuation、定数項、既知次数、疎な非零 index、各 node に必要な precision を保持する。同じ convolution や NTT を再利用し、peak memory も見積もる。式展開で node 数が上限を超えた場合は rewrite を打ち切り、展開前の候補へ戻す。

### 8.5 登録する exact rewrite

初期規則:

| 係数・和の形 | FPS 上の形 |
| --- | --- |
| 通常の畳み込み | OGF の積 |
| 二項係数付き畳み込み | EGF の積 |
| prefix sum | `1 / (1-x)` の乗算 |
| index shift | `x^k` の乗除 |
| `n a[n]` | `x A'(x)` |
| `(n+1)a[n+1]` | `A'(x)` |
| `a[n]/(n+1)` | 積分 |
| alternating sign | `A(-x)` |
| 定係数線形漸化式 | 有理母関数 |
| factorial normalization | OGF と EGF の相互変換 |

式 AST に完全一致した規則だけを適用する。似て見える loop や曖昧な添字範囲を推測で書き換えない。変形履歴には各規則名、変形前後、必要前提を残す。

### 8.6 対応演算

P0/P1 の公開対象:

- 加減算、scalar 倍、shift、truncate
- convolution
- derivative、integral
- `inv`、`log`、`exp`、`pow`、`sqrt`
- polynomial division、remainder
- coefficient extraction
- rational GF
- evaluation、interpolation
- multipoint evaluation / interpolation

後期対象:

- 暗黙方程式 `P(x,F)=0` の Newton 法
- 制限付き composition と compositional inverse
- Taylor shift、chirp-z
- P-recursive recurrence、線形微分方程式、代数的母関数の推測

### 8.7 OGF / EGF と階乗正規化

例えば

~~~~text
c[n] = sum(k=0..n, choose(n,k) * a[k] * b[n-k])
kind c = egf
~~~~

を `C(x)=A(x)B(x)` へ変換する。内部では `a[n]/n!`、`b[n]/n!` を通常 convolution し、最後に `n!` を戻す。`0..N` の階乗と逆階乗が domain で定義できるかを検査する。

### 8.8 sparse / dense / online の選択

疎な式を早期に dense array へしない。非零次数数を `K` として次を比較する。

- sparse × dense: `O(KN)`
- sparse inverse / exp / log / pow の係数漸化式
- naive dense: `O(N^2)`
- NTT / arbitrary-mod convolution: `O(M(N))`
- 過去項への convolution 型依存: CDQ
- 一括で全入力が既知: 一回または少数回の convolution

依存 graph から online 性を調べる。`a[n] = c a[n] + known` のような自己参照は `1-c` の可逆性を確認して分離する。未来項に依存する、または因果順を決められない場合は codegen を拒否する。

### 8.9 一係数だけ必要な場合

すべての先頭係数を作る計画と、第 `K` 項だけを求める計画を最初から分ける。

- 有理母関数 `P/Q`: Bostan–Mori
- 小次数の線形漸化式: Kitamasa
- 同じ分母への多数 query: 前処理共有
- 小さい `K`: 直接 DP
- 代数的 FPS: 後期の専用 coefficient algorithm

全係数用 FPS exp を巨大 `K` の一係数 query へ誤って選ばない。

### 8.10 前提 provenance

各前提に次の由来を付ける。

| provenance | 意味 |
| --- | --- |
| `PROVED_FROM_AST` | AST の構造と domain から証明 |
| `CHECKED_FROM_COEFFICIENTS` | 指定した有限係数だけで確認 |
| `USER_ASSUMED` | ユーザー宣言のみ |
| `UNKNOWN` | 確定不能 |
| `VIOLATED` | 反例または不成立 |

代表的な contract:

- `inv(F)`: `F[0]` が可逆
- `log(F)`: 原則 `F[0] = 1`
- `exp(F)`: `F[0] = 0`
- integral: `1..N` が可逆
- EGF: 必要な factorial が可逆
- `sqrt(F)`: valuation が偶数で、定数項の平方根と枝を決定できる
- `pow`: valuation、負指数、`0^0`、指数分母の意味が決まる
- Bostan–Mori: `Q(0)` が可逆
- interpolation: 点が相異なり差が可逆
- NTT: 法、原始根、必要長が対応
- recurrence: 初期値が十分で、未来依存がない

`VIOLATED` を含む plan は全 risk mode で棄却する。`USER_ASSUMED` や `UNKNOWN` は evidence を下げ、`checked` / `gamble` の規則に従う。

### 8.11 cost planner

辞書順で次を優先する。

1. evidence が強い。
2. 時間・メモリ制約に入る。
3. 実装と前提が単純で壊れにくい。
4. judge profile の推定実行時間が短い。
5. peak memory と生成コード量が小さい。

小さい `N` なら高度な NTT より愚直を選ぶ。calibration は直近の正解率を推測するものではなく、その PC / compiler 上での演算時間を測って backend の crossover を決めるために使う。

### 8.12 独立検証

式 AST を直接 `O(N^2)` で評価する小次数 interpreter を最適化側と分離して持つ。

- 小さい `N` では全係数比較
- parameter 付きなら複数の未使用 parameter
- optimized helper との比較
- 生成した C++ との比較
- oj-bundle 後の再コンパイルと比較
- seed、入力、最初の不一致次数を保存

変形が `EXACT` でも、codegen 結果が不一致なら出力しない。

### 8.13 推測 mode

係数 oracle や既知の初項から、bounded grammar の範囲で次を探す。

- BM による定係数線形漸化式
- rational GF
- bounded order / degree の P-recursive recurrence
- bounded degree の代数方程式 `P(x,F)=0`
- bounded order の線形微分方程式

有限項からの候補は何項一致しても `CONSISTENT` のままである。曖昧な「信頼度 98%」は出さず、method、order、training 項数、holdout 範囲、不一致数を出す。`checked` は規定 holdout を要求し、`gamble` は不足量を明記して候補を生成する。

### 8.14 CLI と出力

~~~~text
python3 tools/ac.py fps formula.poe \
  --risk checked \
  --emit cpp,plan,report \
  --verify-coefficients 128
~~~~

出力:

- `solution.cpp`
- 正規化式と計算 DAG
- `plan.json`: algorithm、計算量、memory、必要 header、前提
- `evidence.json`: rewrite、provenance、検証範囲、seed
- 変形前の愚直 evaluator
- replay command

生成 C++ は GNU++23、`poe` namespace の stable header のみを使う。推測 code には先頭コメントで `CONSISTENT` と検証範囲を残す。

### 8.15 明示的な失敗条件

- OGF / EGF、和の範囲、添字式が曖昧。
- 非可逆元、zero division がある。
- `sqrt` / `pow` の枝や意味が決まらない。
- 漸化式の初期値が不足する。
- 未来依存または解けない自己参照がある。
- 必要 precision が入力範囲を超える。
- NTT の対応長を超える。
- 制約内に入る plan がない。
- 独立 interpreter と生成 code が不一致。

「変形不能」だけでなく、失敗した部分式、前提、代替候補を表示する。

## 9. すべての合成器を支える共通ツール

### 9.1 ac-schema

`problem.json` に入力の型と制約を書き、一度の宣言から次を作る。

- random generator
- 小領域の exhaustive generator
- 最小・最大・重複・空・負数・mod 境界 generator
- schema を壊さない counterexample shrinker
- C++ input parser
- 入力 size metric
- testcase hash と corpus deduplication

標準型として array、string、permutation、tree、forest、connected graph、DAG、interval、query sequence、grid を持つ。例えば tree の縮小は単に辺を消して非 tree にせず、leaf 除去と頂点再番号付けを使う。制約 predicate を満たさない縮小結果は採用しない。

この schema は問題を分類したり解法を選んだりするものではない。ユーザーの宣言を、生成・全列挙・縮小・parser へ再利用する基盤である。

### 9.2 candidate portfolio

各合成器は共通 candidate interface を実装し、`portfolio` が次を一括実行する。

- compile と oj-bundle 後 compile
- sample、regression、未使用 holdout
- 新規 seed の stress
- sanitizer と提出相当最適化の差分
- 最大制約と adversarial generator の benchmark
- 非決定性の再実行
- source size、compile time、peak memory
- parameter tuning

選択順は「既知反例なし」「evidence」「制約内」「単純さ」「実測速度」とする。理論計算量が良くても、最大 NTT 長や memory が危険なら選ばない。

### 9.3 全領域 precompute compiler

入力領域が小さいなら、アルゴリズムを発見せず brute の答え自体を提出 code へ埋め込む。これは「理解せずに AC」へ最も直接的で、宣言領域を完全列挙できれば `EXHAUSTIVE_ON_DECLARED_DOMAIN` を得られる。

候補 representation:

- dense multi-dimensional table
- mixed radix index
- sorted table + binary search
- perfect hash
- run-length / delta encoding
- bit packing
- 小さい decision tree
- 起動時 precompute

code size、compile time、memory、lookup time を実測し、提出制限を超える候補は棄却する。入力領域の宣言漏れがあれば保証外なので、schema の全領域と problem constraints が一致することはユーザー責任として表示する。

### 9.4 attack orchestration

入口として `attack` command を用意する。

~~~~text
python3 tools/ac.py attack \
  --schema problem.json \
  --oracle ./brute \
  --time-limit-ms 2000 \
  --memory-limit-mib 1024 \
  --deadline-ms 90000 \
  --risk checked
~~~~

`attack` は arbitrary C++ を理解するのではなく、schema と optional adapter から適用可能な plugin を判定し、独立に候補を出させる。例えば scalar sequence adapter があれば `seq`、一ステップ DP adapter があれば `state-dp`、scalar word oracle があれば `word-dp`、集合式 AST があれば subset transform compiler を起動する。最後に portfolio が選ぶ。

残り時間の既定配分:

1. 10%: schema / oracle sanity
2. 35%: candidate synthesis
3. 35%: validation / CEGIS
4. 15%: benchmark / bundle / preflight
5. 5%: report と予備

途中で exact candidate が制約内に入った場合は推測探索を打ち切れる。`gamble` では deadline 直前に検査済み範囲が最大の候補を返す。

## 10. 定型問題 compiler

### 10.1 digit-dp-gen

桁ごとの制約を automaton として宣言し、leading zero、tight、started、桁位置、状態直積を含む DP を生成する。

入力できる predicate:

- 桁和・値の mod
- 使用 digit mask / 出現回数上限
- forbidden / required substring
- 隣接 digit 関係
- automaton state
- 区間 `[L,R]` と base

状態到達性を削減し、通常 DP、matrix、digit block のどれかを選ぶ。宣言 DSL からの状態直積は exact rewrite である。leading zero を「数字 0 の列」と扱うか「未開始」と扱うかは必ず明示する。

### 10.2 automaton-transfer

禁止部分文字列、Aho–Corasick、明示 DFA、限定 regex から automaton を構築し、長さ `N` の受理列数や weight 和を生成する。

- 短い `N`: automaton DP
- 巨大 `N`: state-dp へ接続して matrix / recurrence
- 多数 query: power 共有

regex parser が対応外構文を近似してはならない。DFA minimization 後も元 automaton と bounded exhaustive comparison を行う。

### 10.3 frontier-dp-gen

幅固定 grid の局所配置規則から bitmask frontier state と遷移を全列挙する。

P1:

- domino / tile 配置
- 独立集合・被覆など隣接局所制約
- 1 列または少数列を跨ぐ状態
- 列ごとの obstacle pattern

P2:

- connectivity partition を持つ plug DP
- canonical relabeling
- component を閉じる条件

横幅と局所 rule が固定なら state graph を `state-dp` へ渡し、長さ巨大時は matrix 化する。state explosion、canonicalization collision、閉じた component の取り扱いを検査する。

scalar grid oracle しか書けない場合の列 token 化と `word-dp` 接続は、[Non-word 仕様の `grid-dp`](nonword_automation_tools_spec.md#4-grid-dp-固定幅-frontend) に従う。

### 10.4 subset-transform

集合式 AST から次を識別してコード生成する。

- subset / superset zeta
- Möbius inversion
- OR / AND / XOR convolution
- subset convolution と ranked zeta
- divisor / multiple zeta

~~~~text
h[S] = sum(T subseteq S, f[T])
h[S] = sum(T subseteq S, f[T] * g[S xor T])
~~~~

AST が登録 pattern に一致すれば `EXACT`。brute の fingerprint だけから変換種を推測する mode は `CONSISTENT`。bit 順、inverse coefficient、XOR inverse の `2^k` 可逆性、mod、in-place 可否を検査する。

### 10.5 bitset-compiler

限定 Boolean DP DSL を word-level shift / and / or へ変換する。

対象:

- subset sum / Boolean knapsack
- 値域上の到達可能性
- Shift-And 型 string matching
- 小 automaton の状態一括更新
- grid の行・列方向更新

`next[s] = dp[s] or dp[s-weight]` を `bits |= bits << weight` へ変換するだけでなく、64 bit word を跨ぐ shift、末尾 mask、負方向、in-place 更新順、動的長を生成する。登録規則に一致した変換だけ exact とする。

### 10.6 meet-in-the-middle compiler

各要素の選択肢、半分ごとの summary、結合条件、existence / count / min / max を宣言する。

候補:

- hash lookup
- sort + binary search
- two pointers
- duplicate run counting
- Pareto / dominance pruning
- subset enumeration

空集合、重複、mod、overflow と、`n` が奇数の分割を自動処理する。小さい `n` の全探索との比較を生成する。summary が結合条件に十分であることは DSL の型規則かユーザー仮定として記録する。

### 10.7 csp-reduce

人間は algorithm 名ではなく typed variable と constraint を書く。

~~~~text
bool selected[i]
selected[a] -> selected[b]
selected[x] xor selected[y]
potential[v] <= potential[u] + cost
flow_conservation(v)
~~~~

backend:

- 2-SAT
- GF(2) linear equations
- difference constraints
- bipartite matching
- max-flow / min-cut
- lower-bound circulation
- min-cost flow
- exact cover / DLX
- bounded backtracking

最初は 2-SAT、XOR、difference constraints、flow の順に増やす。生成解を元 DSL の全 constraint へ戻して検査する runtime checker も生成する。DSL から backend への変換は exact にできるが、問題文から DSL への写し間違いは保証できない。

### 10.8 query-planner

safe mode では人間が演算の性質を宣言する。

- associative / commutative / idempotent
- identity / inverse
- static / online / offline
- point / range update
- point / range query

そこから prefix sum、imos、Fenwick tree、segment tree、lazy segment tree、sparse table、disjoint sparse table、sqrt decomposition、Mo、offline sort、parallel binary search、CDQ を選ぶ。宣言した性質は assumption ledger へ残し、小さい domain があれば結合則、単位元、作用合成を全探索する。

### 10.9 loop-rewrite

限定した loop / sum AST を、意味保存規則で変形する。

- prefix sum / difference array
- 同じ key による grouping
- coordinate compression
- complement counting
- inclusion-exclusion
- offline event sort
- divisor / subset zeta
- convolution
- meet-in-the-middle

arbitrary C++ source-to-source optimization は行わない。ユーザーが loop body と index range を typed AST へ移した場合だけ、範囲・overflow・更新順を保った C++ を生成する。

## 11. 推測・探索による救済ツール

### 11.1 property-scout / opt-scout

愚直解が作る表、cost function、argmin から次の反例を探す。

- 単調性、二分探索 predicate の単調性
- convex / concave / unimodal
- argmin monotonicity
- Monge、quadrangle inequality
- Knuth / divide-and-conquer optimization の条件
- submodularity
- symmetry、translation invariance、separability
- low rank、periodicity、parity

性質が DSL 上で代数的に検査できるか、問題の全有限 domain を列挙した場合以外は `CONSISTENT`。通った仮説から binary search、D&C DP、SMAWK、CHT、min-cut、state compression の候補を出し、元 DP との stress へ接続する。

trace contract、scope、同点規約、Monge / SMAWK / CHT の codegen 条件は [Non-word 仕様の `dp-opt`](nonword_automation_tools_spec.md#8-dp-opt-愚直-dp-最適化候補の生成) に置く。

### 11.2 monoid / action synthesizer

区間への brute function と、候補 summary field を与え、短い merge 式を grammar から探索する。

候補 field 例:

- sum、min、max、length
- first、last
- prefix、suffix、best
- small count vector

小配列で merge 候補を合成し、brute 一致と結合則の counterexample を CEGIS で探す。最大部分配列和のような summary を発見できる可能性がある。有限領域を全列挙できない限り `CONSISTENT` であり、生成 segment tree は元 brute との独立 stress を必須にする。

### 11.3 CEGIS program synthesizer

typed grammar の中から oracle と一致する最短式・小 program を探索する。

grammar:

- small integer constant
- 四則、min / max、bit operation
- comparison と conditional
- 近傍 array access
- fixed-size loop
- lookup table
- state update、sort key、greedy priority

流れ:

1. 少数 example で候補を列挙する。
2. 候補同士の出力が異なる入力を schema 内で探す。
3. oracle へ問い合わせる。
4. counterexample を全候補の constraint へ追加する。
5. 候補が一つになるか deadline まで繰り返す。
6. C++ を生成し、新規 holdout と stress を行う。

有限 grammar と全入力を完全検査できる場合以外は `CONSISTENT`。状態遷移表、短い判定式、threshold、greedy の穴埋めに範囲を限定し、万能 program synthesis を目指さない。

### 11.4 checker-guided constructive

出力 checker を C++ callable または DSL として渡し、入力ごとに通る構成を探す self-contained C++ を生成する。

backend:

- constraint DSL からの exact solver
- greedy repair
- random restart
- hill climbing / simulated annealing
- 部分固定と再探索
- 小 grammar の構築規則 CEGIS

white-box checker は違反数と違反位置を返せるようにする。合否しか返さない black-box checker は探索力が弱い。heuristic backend は成功率と最悪時間を benchmark し、`gamble` 用候補とする。sample 固定出力や judge への不正アクセスは行わない。

### 11.5 game-periodicity

brute で win / lose または Grundy 列を作り、前周期と周期を探す。有限 move set で次状態が直前 `K` 状態だけから決まる場合、完全な `K`-window の再訪から周期を厳密化できる。約数や任意に遠い state へ依存する場合、値の反復は推測にすぎない。

### 11.6 numerical rescue

`precision-lab` は同じ式を long double、整数化、exact rational、小さい多倍長で比較し、誤差入力を探索する。`overflow-scout` は interval arithmetic と sanitizer で overflow 候補を示し、`complexity-calibrator` は候補の操作単価と最大入力を測る。

これらは解法合成そのものではないが、正しそうな code が precision、overflow、定数倍で落ちる事故を減らすため、portfolio の必須 plugin とする。

## 12. candidate plugin contract

新しい「AC をもぎ取る手口」を追加しやすいよう、各 generator は次の lifecycle を実装する。

~~~~text
applicable(job) -> reason
normalize(job) -> typed IR
synthesize(IR, budget) -> candidates
static_check(candidate) -> assumptions, complexity
emit_cpp(candidate) -> source, headers
validate(candidate, corpus) -> evidence
benchmark(candidate, judge_profile) -> measurements
explain_rejection(candidate) -> reason
~~~~

plugin は生成時に network を要求せず、同じ IR と seed から同じ候補順を返す。optional dependency がない場合は明確に `not available` とし、silent fallback で意味の違う変換をしない。

すべての generator は、最低一つの小さい positive fixture、境界 fixture、意図的な invalid fixture、generated C++ の compile test を持つ。

## 13. 優先度評価

入力負担は 1 が「brute と schema 程度」、5 が「専用 solver を書くのと大差ない」。誤推定 risk は exact mode ではなく、経験的候補を使った場合の危険度である。

| ツール | 優先度 | AC への即効性 | 入力負担 | 誤推定 risk | 実装難度 |
| --- | --- | --- | ---: | --- | --- |
| ac-schema + shrink | P0 | 全ツールの検証速度を上げる | 2 | 低 | 中 |
| candidate portfolio | P0 | 最終候補選択を自動化 | 1 | 中 | 中 |
| state-dp emit / lift | P0 | 定数状態 DP を直接巨大化 | 2 | 低 | 高 |
| word-dp / Hankel realization | P0 | scalar 愚直解だけから多文字 DP を復元 | 1 | 中〜高 | 高 |
| seq + BM / Kitamasa | P0 | black-box 数列を巨大 index 化 | 1 | 高 | 中 |
| poly-fit / quasi-fit | P0 | 多項式・準多項式を即時評価 | 1 | 中 | 低〜中 |
| FPS exact DSL / codegen | P0 | 式変形と実装を省略 | 2 | 低 | 高 |
| 全領域 precompute | P0 | 小 domain なら解法不要 | 1 | 低 | 中 |
| FPS 係数式 rewrite | P1 | 係数 DP を FPS へ変換 | 2 | 中 | 高 |
| rational gf-guess | P1 | 項列から母関数候補を発見 | 1 | 高 | 中 |
| bag-dp | P1 | 順序なし個数 DP を巨大化 | 1 | 中〜高 | 中〜高 |
| operator-id | P1 | 二重 loop を transform 等へ同定 | 2 | 中〜高 | 高 |
| grid-dp frontend | P1 | 固定幅 grid を既存 learner へ接続 | 2 | 中 | 中 |
| digit / automaton compiler | P1 | 状態直積事故を減らす | 2 | 低 | 中 |
| frontier DP | P1 | 幅固定問題を状態列挙 | 3 | 低〜中 | 高 |
| subset transform | P1 | 変換式を即コード化 | 2 | 低 | 中 |
| bitset compiler | P1 | `O(NW)` を word 並列化 | 2 | 低 | 高 |
| meet-in-the-middle | P1 | 指数全探索を半分化 | 2 | 低〜中 | 中 |
| periodicity | P1 | brute 列の周期化 | 1 | 高 | 中 |
| CSP reduction | P2 | 2-SAT / flow 等へ自動帰着 | 3 | 低〜中 | 高 |
| query planner | P2 | データ構造選定とboilerplateを省略 | 3 | 中 | 高 |
| property-scout / dp-opt | P2 | DP 最適化条件を発見し候補生成 | 2 | 高 | 高 |
| tree-dp | P2 experimental | 木 oracle から merge tensor を復元 | 1〜2 | 高 | 最高 |
| monoid synthesizer | P2 | 区間 summary を発見 | 3 | 高 | 最高 |
| CEGIS program synthesis | P2 | 短い遷移・判定を穴埋め | 3 | 高 | 最高 |
| checker-guided constructive | P3 | 最終手段の探索 code | 4 | 最高 | 最高 |
| implicit / algebraic FPS | P3 | 高難度式を一係数化 | 3 | 中〜高 | 最高 |
| graph-dp | P3 research | decomposition 上の状態を復元 | 3 | 最高 | 最高 |

最初から巨大な万能 synthesizer を作らない。小さな generator を同じ schema、candidate、evidence、stress 基盤へ載せ、portfolio で競わせる方が早く実戦投入できる。

## 14. library 側に必要な部品

code generator より先、または同時に次の `poe` library primitive が必要になる。

### P0 dependency

- modint と安全な inverse contract
- dense / sparse matrix と semiring power
- prime field 上の incremental Gaussian elimination
- polynomial / convolution、naive と NTT
- FPS `inv/log/exp/pow/sqrt`
- Berlekamp–Massey、Kitamasa、Bostan–Mori
- process-independent comparator 用 utility
- dynamic bitset の shift

### P1 dependency

- multipoint evaluation / interpolation
- subset transforms
- low-rank factorization、bitwise / divisor transforms
- divide-and-conquer optimization、Knuth、SMAWK、CHT、Li Chao
- automaton / Aho–Corasick
- Fenwick / segment tree / lazy segment tree
- 2-SAT、flow、matching
- exact rational または小さな多倍長 helper

P2 / experimental では tree context 用の tool-only tensor、rerooting、Static Top Tree、nice tree decomposition validator を追加する。planned header path、直接依存、tool-only と提出用の分離は [Non-word 愚直解高速化ツール群仕様の依存表](nonword_automation_tools_spec.md#11-依存ライブラリ) を正とする。

generator は stable header だけを既定利用する。beta / experimental header が必要な候補は evidence と別に dependency risk を表示し、stable candidate があればそちらを優先する。

## 15. 実装 layout

~~~~text
tools/
├── ac.py
├── RULES.md
├── aclib/
│   ├── job.py
│   ├── schema.py
│   ├── corpus.py
│   ├── candidate.py
│   ├── evidence.py
│   ├── codegen.py
│   ├── compile.py
│   ├── portfolio.py
│   └── deadline.py
├── ac_plugins/
│   ├── state_dp/
│   ├── word_dp/
│   ├── bag_dp/
│   ├── tree_dp/
│   ├── grid_dp/
│   ├── graph_dp/
│   ├── operator_id/
│   ├── sequence/
│   ├── fps/
│   ├── property_scout/
│   ├── dp_opt/
│   ├── precompute/
│   └── ...
├── helpers/
│   └── *.cpp
└── templates/
    └── *.cpp.in

tests/
└── ac_tools/
    ├── fixtures/
    ├── golden/
    └── test_*.py
~~~~

Python core は標準ライブラリだけを必須とする。heavy algebra は C++ helper で `cp/` を直接検査する。SymPy、Z3 等は explicit plugin とし、未導入時に install しない。

生成する `.hpp` がある場合も library 規約に従い、物理一行目を `#pragma once`、二行目を `#include <bits/stdc++.h>`、三行目以降を local include、その後を実装とする。header 内へ `#define` / `#undef` を生成しない。`assert` は前提と内部不変条件の検査に使ってよい。

## 16. 実装ロードマップ

### Phase 0: 共通の縦切り

1. job / candidate / evidence JSON schema
2. deadline、process、compile、bundle、corpus 保存
3. 既存 stress runner との接続
4. `--risk exact|checked|gamble`
5. report.md / report.json / replay.sh
6. contest profile の network fail-closed 検査

完了条件は、手書き候補二つを portfolio へ渡し、oracle、bundle、benchmark で一つを選べること。

### Phase 1: 定数状態 DP の最短縦切り

1. prime mod の emit mode
2. symbolic linear scalar
3. affine lift
4. dense matrix backend
5. 小 `N` 差分
6. `10^18` step の `solution.cpp`

完了条件は「6 state 程度の普段の愚直更新だけを書き、一コマンドで巨大 `N` 用 bundle 可能 code を得る」こと。

### Phase 1B: scalar oracle からの Word DP 復元

1. prime field と finite alphabet
2. batch `f(word)` oracle と disk cache
3. proof 用 shortlex context
4. adaptive suffix / prefix Hankel basis
5. token matrix と実 rank `r` の復元
6. finite certificate と CEGIS holdout
7. dense / sparse scan codegen

完了条件は「内部 state を一切露出しない短い word 用の愚直解だけから、複数 token の非可換遷移行列と bundle 可能 code を得る」こと。詳細は [Black-box Word DP / Hankel 復元仕様](word_dp_hankel_spec.md) に従う。

### Phase 2: 数列と FPS の実用品

1. prime mod BM + adaptive prefix + final holdout
2. Kitamasa / Bostan–Mori
3. 一変数 `poly-fit` と bounded `quasi-fit`
4. rational `gf-guess`
5. FPS typed DSL と naive interpreter
6. convolution、`inv/log/exp/pow/sqrt`
7. OGF / EGF、微分・積分、prefix、shift rewrite
8. cost planner と codegen

### Phase 3: brute + schema の攻撃範囲を広げる

1. ac-schema と domain-aware shrink
2. 全領域 precompute
3. digit / automaton compiler
4. subset transform
5. bitset
6. meet-in-the-middle
7. simple frontier DP
8. `bag-dp` と `grid-dp` frontend
9. `operator-id` の diagonal、prefix、Toeplitz、bitwise transform

### Phase 4: 高リスク・高報酬

1. `property-scout` report と D&C / Knuth の `dp-opt`
2. `tree-dp` の binary sparse tensor prototype
3. game periodicity
4. CSP reduction
5. query planner
6. monoid synthesis
7. CEGIS
8. algebraic / D-finite `gf-guess` と implicit FPS
9. checker-guided constructive

### Phase 5: research composition

1. tree rerooting context action
2. Static Top Tree backend
3. nice tree decomposition validator / runner
4. many-sorted `graph-dp`

Phase 2 以降の個別完了条件と依存順は [Non-word 愚直解高速化ツール群仕様](nonword_automation_tools_spec.md) に従う。

## 17. end-to-end 利用例

### 17.1 遷移を整理できない 5 state DP

1. 普段どおり `step<Scalar>(current)` を書く。
2. `state-dp` が symbolic scalar で実行する。
3. 非線形操作がなければ matrix を抽出する。
4. 元 step と matrix を `N=0..100` で比較する。
5. matrix、Krylov recurrence、愚直反復を benchmark する。
6. 制約内で最も単純な `solution.cpp` を選ぶ。

人間は遷移行列を手で書かず、積順や off-by-one も生成器に任せられる。

### 17.2 小さい n の答えだけ出せる

1. `brute(n)` を batch oracle にする。
2. `seq` が 256 項を training、256 項を validation に使う。
3. BM の order が prefix 倍増後も安定した候補を残す。
4. 未使用 512 項を final holdout に使う。
5. `checked` なら `FOUND + CONSISTENT` として Bostan–Mori code を出す。
6. report 冒頭に「証明なし、holdout 512 項一致」と残す。

この経路は、理解していなくても AC する可能性を取りに行くための意図的な投機機能である。

### 17.3 係数 DP は書けるが FPS 変形ができない

1. sum の範囲と係数を DSL に転記する。
2. naive AST interpreter で小さい係数を作る。
3. rewrite engine が convolution、derivative、EGF normalization を試す。
4. sparse、CDQ、Newton + NTT を cost 比較する。
5. 生成 code を naive 係数と比較して bundle する。

### 17.4 入力空間が実は小さい

1. problem schema を完全列挙可能と宣言する。
2. precompute compiler が brute を全入力へ実行する。
3. table、圧縮表、runtime precompute を生成する。
4. source size と memory に入る最小候補を選ぶ。

解法の一般化を一切せず、problem domain 全体の答えを埋め込んで AC を狙う。

## 18. 検証・受け入れ条件

### 18.1 共通

- 同じ job、seed、version で byte-identical な plan と code を出す。
- LLM / model runtime / network 依存がないことを dependency audit する。
- generated source と oj-bundle 後 source の両方を GNU++23 で compile する。
- training と final holdout の file hash が分離されている。
- timeout、RE、WA、non-determinism を区別する。
- counterexample と replay command から失敗を再現できる。
- `checked` / `gamble` code から evidence 警告を消せない。
- known counterexample がある候補は `gamble` でも選ばない。

### 18.2 故意に騙す fixture

- 最初の 100 項だけ Fibonacci で、その後変わる列
- 長い prefix だけ周期的な列
- numeric probe 点では線形だが別の点で分岐する step
- `N=0` だけ off-by-one の DP
- 先頭係数では可逆だが別 parameter で非可逆になる FPS
- training generator が作らない graph 構造
- sanitizer build と optimized build で挙動が違う oracle

これらを `EXACT` と誤表示せず、holdout または stress で可能な限り棄却する。

### 18.3 performance

- tool 自身の deadline を超えたら process を回収し、完成済み artifact を返す。
- oracle call を cache し、同一 input を重複実行しない。ただし非決定性検査分は除く。
- 10 秒、30 秒、90 秒 profile で候補生成の degradation を検査する。
- judge calibration は演算時間と memory の測定だけに使い、問題正答率の「校正」には使わない。

## 19. 確定した設計判断

1. 推測結果からも code を生成する。既定は `--risk checked` とし、さらに攻める `gamble` を持つ。
2. exact と empirical を同じ status にしない。
3. core は非 LLM、offline、決定的にする。
4. Python 3 標準ライブラリ CLI + C++ helper で実装する。
5. 入力は JSON / ASCII DSL / typed C++ adapter とし、自然言語解釈を core に入れない。
6. generated C++ は GNU++23、`poe`、`cp/...`、oj-bundle 対応とする。
7. 自動提出は行わない。
8. 旧ライブラリの実装や verify 結果を oracle にせず、新規に仕様と独立愚直から作る。

## 20. 参考

- [AtCoder 生成AI対策ルール 20251003版](https://info.atcoder.jp/entry/llm-rules-ja)
- [Bostan–Mori: A Simple and Fast Algorithm for Computing the N-th Term of a Linearly Recurrent Sequence](https://arxiv.org/abs/2008.08822)
- [SymPy Formal Power Series documentation](https://docs.sympy.org/latest/modules/series/formal.html)
- [えこってさんによるdpの推定手法の正当性について](https://maspypy.com/ecottea_dp_estimate_method)
- [maspypy library: ecottea_matrix_dp.hpp](https://maspypy.github.io/library/linalg/ecottea_matrix_dp.hpp.html)
- [ecotteaCP: 自動 DP](https://scrapbox.io/ecotteaCP/%E8%87%AA%E5%8B%95_DP)
- [Black-box Word DP / Hankel 復元仕様](word_dp_hankel_spec.md)
- [Non-word 愚直解高速化ツール群仕様](nonword_automation_tools_spec.md)
- [全体要件](requirements.md)
- [開発支援ツール](development_tools.md)
