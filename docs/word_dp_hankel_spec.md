# Black-box Word DP / Hankel 復元 仕様書

## 0. 位置付け

- command 名: `word-dp`
- 内部機能名: `hankel_word_dp`
- 優先度: P0
- 対象: 有限 alphabet 上の scalar oracle から、等価な有限次元線形 DP を復元する。
- 初期係数体: prime mod field
- 実装方針: 参考実装をコピーせず、定理、入出力 contract、独立検証から新規実装する。

この機能は、既存の `state-dp` と `seq` の間を埋める。

| command | ユーザーが書くもの | 復元対象 |
| --- | --- | --- |
| `state-dp` | 内部状態または一ステップ更新 | 既知状態上の一つ以上の作用素 |
| `seq` | `a(n)` | 一種類の作用素を反復した scalar 列 |
| `word-dp` | `f(word)` | alphabet の各 token に対応する複数の未知作用素 |

`word-dp` の価値は、DP state の意味、一ステップ更新、遷移行列を人間が発見しなくても、短い word に対する答えを返す愚直解だけから提出用 DP を作れることである。

## 1. 数学的 contract

有限 alphabet `Sigma`、体 `K`、関数 `f: Sigma* -> K` を考える。次の有限次元線形表現が存在すると仮定する。

~~~~text
f(s1 s2 ... sn) = u^T M[s1] M[s2] ... M[sn] v
~~~~

- `M[a]`: 各 token `a` に対応する、同じ vector space 上の線形作用素
- `u`、`v`: 始点と終点の線形形式
- `D`: 表現次元の既知上限

元の `M[a]`、`u`、`v` をそのまま復元する必要はない。同じ `f(word)` を返す、より小さい可到達・可観測な線形表現を構成する。最小表現の次元は無限 Hankel matrix

~~~~text
H[p, q] = f(p q)
~~~~

の rank に一致する。この仕様では、有限 Hankel table からその表現を構成する。

### 1.1 適用できる例

- 文字列を左から読む定数状態 DP
- digit DP の一桁ごとの作用
- finite automaton の数え上げ
- wildcard を各文字の重ね合わせとして扱う DP
- 同じ有限 operator 集合を用いる range product
- affine DP を定数 1 の状態で線形化したもの
- phase を state に含めた周期依存 DP

### 1.2 そのままでは適用できない例

- DP 値に応じて遷移が変わる非線形更新
- state 数が入力長とともに増える DP
- 未状態化の位置依存
- 無限または実質的に巨大な alphabet
- noisy / floating oracle
- 通常の Gaussian elimination が使えない max-plus / min-plus black-box 推定
- tree merge のような二項・多項 tensor

後二つは別 backend として experimental に扱う。

## 2. 保証の表示

有限個の oracle 値だけから、有限次元表現の存在や本当の次元上限を無条件には証明できない。次の二軸を必ず表示する。

### 2.1 Problem / oracle から model への根拠

| status | 意味 |
| --- | --- |
| `STATE_BOUND_PROVED` | typed state や別の構造から次元上限 `D` を導出できる |
| `USER_ASSERTED_BOUND` | ユーザーが `rank(H) <= D` を仮定した |
| `INFERRED_BOUND` | 有限 table の rank plateau から推測しただけ |
| `UNKNOWN` | 線形表現の存在も次元上限も不明 |

### 2.2 Model から生成表現への根拠

| status | 意味 |
| --- | --- |
| `FINITE_HANKEL_CERTIFICATE` | `D` 上限の下で十分な有限関係を全検査した |
| `FULL_RANK_CERTIFICATE` | rank `D` の非特異 minor を得て、上限と一致した |
| `HANKEL_CONSISTENT` | training / holdout table に一致しただけ |
| `RANK_PLATEAU_ONLY` | 指定深さまで rank が増えなかっただけ |
| `REJECTED` | closure、rank、holdout のいずれかに反例がある |

表示例:

~~~~text
Problem -> model: USER_ASSERTED_BOUND(rank <= 12)
Model -> code:    FINITE_HANKEL_CERTIFICATE
Conditional badge: THEOREM_CONDITIONAL
Global evidence:   CONSISTENT
~~~~

`STATE_BOUND_PROVED + FINITE_HANKEL_CERTIFICATE` は oracle に対して `EXACT` と扱える。`USER_ASSERTED_BOUND` は定理に基づくが未検証の前提付きなので、conditional badge を付け、global evidence は `CONSISTENT` とする。`INFERRED_BOUND` も何項一致しても `CONSISTENT` のままとする。共通 global evidence と plugin 固有 certificate は別 field に保存する。

## 3. 入力

### 3.1 CLI

~~~~text
python3 tools/ac.py word-dp oracle.cpp \
  --oracle-fn naive \
  --alphabet 0,1 \
  --mod 998244353 \
  --dimension-bound 16 \
  --target input.txt \
  --risk checked \
  --backend auto
~~~~

自動次元 mode:

~~~~text
python3 tools/ac.py word-dp oracle.cpp \
  --alphabet a,b,c \
  --mod 998244353 \
  --auto-dimension \
  --max-dimension 64 \
  --risk gamble
~~~~

### 3.2 job schema

~~~~json
{
  "kind": "word_linear_dp",
  "alphabet": ["0", "1"],
  "field": {
    "kind": "mod_prime",
    "mod": 998244353
  },
  "oracle": {
    "source": "work/oracle.cpp",
    "function": "naive",
    "batch": true
  },
  "dimension": {
    "kind": "user_upper_bound",
    "value": 16
  },
  "target": {
    "kind": "explicit_word"
  }
}
~~~~

alphabet token は内部では整数 ID にする。`char` と `string` は convenience adapter とし、alphabet に `?` や delimiter を含めても serialization が壊れない形式を使う。duplicate token と unknown token を拒否する。

### 3.3 oracle contract

~~~~cpp
Mint naive(std::span<const int> word);
~~~~

- 空 word を受け取れる。
- 同じ word には同じ値を返す。
- prime field の正規化された値を返す。
- batch adapter を既定とし、process 起動回数を減らす。
- optional に `snapshot(prefix)` と `extend(snapshot, token)` を提供できる。

oracle の結果は word を key に disk cache し、deadline 後に再開可能にする。cache key へ oracle executable hash、mod、parameter、tool version を含める。

## 4. 証明付き復元 engine

### 4.1 test context

次元上限を `D` とする。左 context 集合を shortlex 順に

~~~~text
U = { x in Sigma* : |x| <= D - 1 }
~~~~

とする。right word `t` の signature を

~~~~text
h(t) = (f(x t)) for x in U
~~~~

と定める。

### 4.2 suffix basis の適応構築

1. `T = [epsilon]` から始める。
2. `h(epsilon)` が零なら、定理の前提下では zero function として別処理する。
3. 新しく独立になった `t` だけについて、全 `a in Sigma` の `a t` を queue へ入れる。
4. `h(a t)` を現在の `h(T_i)` で消去する。
5. 独立なら `a t` を basis に追加する。
6. basis 数が `D` を超えたら dimension contract 違反として拒否する。
7. 独立 word が増えなくなるまで繰り返す。

追加ごとに rank が一つ増えるため basis 数 `r` は高々 `D` である。すべての word を right candidate として列挙せず、独立 basis の一文字拡張だけを調べる。

### 4.3 遷移係数

各 token `a` と basis `T_i` について、次を解く。

~~~~text
h(a T_i) = sum_j C[a][i][j] h(T_j)
~~~~

`U` の全 row で残差零を検査する。pivot row の `r x r` 系を解く処理と、残り row の certificate 検査を分ける。同じ基底分解を全 token と全 `i` で再利用する。

### 4.4 生成 DP

~~~~text
q_i(p) = f(p T_i)
q_i(p a) = sum_j C[a][i][j] q_j(p)
q_i(epsilon) = f(T_i)
~~~~

`T_0 = epsilon` を必ず保持し、word `p` の答えは `q_0(p)` とする。zero function は rank 0 の専用 code を生成する。

### 4.5 有限 certificate

hidden representation の次元が高々 `D` なら、長さ `D-1` 以下の全 context `x` で残差が零であることから、任意長 `x` でも同じ関係が成立する。oracle に問い合わせる word 長は最大 `2D-1` で足りる。

必要 membership query 数の目安:

~~~~text
|U| * (1 + |Sigma| * r)
|U| = 1 + |Sigma| + ... + |Sigma|^(D-1)
~~~~

同じ word は cache する。全長 `2D-1` 以下の全 word を先に列挙する方式より少ないが、`D` に対する指数性は残る。実行前に query 数、推定 oracle 時間、memory を表示する。

### 4.6 full-rank 早期終了

有限 Hankel submatrix から rank `D` の非特異 minor を得た場合、既知上限も `D` なので hidden rank はちょうど `D` である。左右 basis が全空間を張った certificate を構成できれば、`U` 全列挙を完了する前に exact engine を終了できる。

rank `r < D` の plateau だけでは、長い word で初めて現れる hidden state を否定できない。exact mode は `U` 全検査へ戻る。

## 5. Adaptive / CEGIS engine

`D` が大きく proof table が現実的でない場合、`P = Q = {epsilon}` から小さな Hankel table を作る。

1. `P Sigma` と `Sigma Q` のうち rank を増やす word を追加する。
2. 現在の table から候補行列を作る。
3. training に使っていない word を oracle と生成 model で比較する。
4. 反例 word を prefix / suffix に分割し、区別に必要な context を `P`、`Q` へ追加する。
5. rank と候補が安定するか deadline まで繰り返す。

validation word:

- 空 word と長さ 1
- 一文字の長い run
- 交互列
- 短い block の反復
- 一文字 mutation
- token 頻度が極端な word
- training より十分長い random word
- de Bruijn 風に短い substring を多く含む word

`checked` は独立 final holdout と長さを伸ばした検査を要求する。`gamble` は rank plateau だけでも code を出せるが、`RANK_PLATEAU_ONLY`、最大学習長、未検査長を先頭に表示する。

## 6. 参考方式からの改良

### 6.1 oracle 計算

- word ごとの cache
- batch query
- prefix trie 上での snapshot 共有
- BFS / memoized oracle の中間状態再利用
- query cost の長さ別実測
- deadline 内に完了する context depth の選択

### 6.2 basis 選択

同じ rank でも basis により生成行列の密度が変わる。複数候補を作り、次を辞書順で最小化する。

1. certificate の強さ
2. rank
3. 全行列の非零要素数
4. 一 step の乗算・加算回数
5. 係数の表現長
6. generated source size

候補:

- deterministic shortlex pivot
- user-pinned context / suffix
- sparse pivot heuristic
- `checked` で seed 固定の randomized basis
- similarity transform による sparsification

rank `r` だけの compile-time array を生成し、上限 `D` まで零 padding しない。

### 6.3 code generation

- 汎用 dense matvec
- nonzero entry だけを出す sparse / unrolled matvec
- 同じ行列を持つ token の class 統合
- 重み付き token class の線形結合
- wildcard matrix を不変な local constant として生成
- 複数回 `solve` しても内部 model を変更しない
- alphabet、rank、mod、basis、orientation を JSON と C++ comment に残す
- 独自 delimiter 文字列ではなく versioned JSON と hash を使う

### 6.4 runtime backend

| target | backend |
| --- | --- |
| 一つの明示 word | dense / sparse scan, `O(r^2 * length(S))` |
| run-length encoded word | matrix power per run |
| 同一 block の反復 | block matrix power |
| unary alphabet | `seq` / BM / Kitamasa / Bostan–Mori |
| point update + 全体値 | matrix product segment tree |
| static substring composition | product tree / prefix-suffix、条件次第 |
| grammar / SLP 圧縮 word | DAG 上の matrix product |

行列積は非可換なので順序を compile fixture で検査する。

## 7. 拡張

### P1

- vector / multiple-output oracle の block Hankel
- sparse basis portfolio
- RLE、block power、segment tree codegen
- 位置 weight を変えた複数 sample から transition entry を Lagrange 補間
- degree bound 付き一変数 polynomial transition
- target mod と別 prime での rank sanity check

### P2

- 多変数 polynomial weight
- rational coefficient と CRT reconstruction
- 係数を `0/1` に制約した automaton reconstruction
- state feature を一部 user-pinned した hybrid reconstruction
- FPS divide-and-conquer product との接続
- binary tree context から merge tensor を復元する `tree-dp` prototype

### P3 experimental

- max-plus / min-plus の制約付き推定
- rerooting / Static Top Tree codegen
- higher-order tensor と多項式 DP

field 上の Hankel / Gaussian elimination と、max-plus や tensor の探索を同じ保証で扱わない。別 plugin、別 evidence とする。

可換 count vector、固定幅 grid frontend、tree context / merge tensor への拡張は [Non-word 愚直解高速化ツール群仕様](nonword_automation_tools_spec.md) に置く。

## 8. 失敗条件

- `D` を超える独立 basis が見つかる。
- complete certificate の residual が非零。
- oracle が同じ word で異なる値を返す。
- oracle が RE / TLE / output overflow になる。
- prime field でないのに field elimination を要求する。
- alphabet token が重複する、または target に未知 token がある。
- empty word の規約が未定義。
- query 見積もりが deadline / memory を超え、risk mode が fallback を許可しない。
- training model と final holdout が不一致。
- matrix product の左右規約 test が不一致。
- parameter ごとに別 model が必要なのに一つの model として混ぜた。

`D` が過小でも、長さ `2D-1` まで別の小次元 model と区別できない場合がある。したがって dimension upper bound の由来を隠さない。

## 9. artifact

~~~~text
.ac-work/<job>/
├── input/
│   ├── job.json
│   └── oracle.cpp
├── oracle_cache/
├── hankel/
│   ├── basis_prefix.json
│   ├── basis_suffix.json
│   ├── pivots.json
│   └── rank_history.json
├── candidates/
├── selected/
│   ├── solution.cpp
│   ├── model.json
│   └── certificate.json
├── cases/
│   ├── validation/
│   ├── holdout/
│   └── counterexamples/
├── report.md
└── replay.sh
~~~~

`certificate.json`:

- alphabet と token order
- field / mod
- dimension bound と由来
- recovered rank
- prefix / suffix basis
- pivot rows
- token matrices
- initial vector と answer coordinate
- oracle query count と最大 word 長
- closure residual 検査数
- holdout 件数
- oracle / generator hash

## 10. acceptance test

1. 非可換な 2 operator model を復元し、積順を間違えない。
2. hidden state 数より Hankel rank が小さい例で最小 rank を出す。
3. zero function を rank 0 で処理する。
4. affine DP を lift した oracle を復元する。
5. dimension bound 違反を basis 増加または residual で検出する。
6. 過小 `D` で exact を誤表示しない。
7. unary alphabet を `seq` backend と比較する。
8. wildcard の重ね合わせを oracle と比較する。
9. 同じ model instance の `solve` を複数回呼んでも結果が変わらない。
10. dense、sparse、unrolled code が一致する。
11. bundle 前後で validation と holdout が一致する。
12. AGC027 E 型の、内部 state を露出しない BFS oracle から提出 code を生成する。

## 11. 参考実装をそのまま採用しない理由

指定された `ecottea_matrix_dp.hpp` は重要な参考実装だが、本 project の規約・目的に合わせて再設計する。

- template parameter `d` をそのまま生成 code の次元にせず、実 rank `r` へ縮約する。
- test context の境界を定理どおり最大 `D-1`、oracle word を最大 `2D-1` として明文化する。
- 参考コードの context list は、深さ `max_append` の node から子を追加してから次の反復で停止するため、既定値 `D-1` でも一層深い word を含む。生成時点で深さを検査して余分な query を作らない。
- oracle cache、batch、deadline、holdout、certificate がない。
- delimiter 連結文字列ではなく JSON を使う。
- alphabet、rank、matrix、internal state を build / deserialize ごとに検査・初期化する。
- wildcard 行列を `solve` 内で map へ追加しない。参考コードの形では、同一 instance の二回目以降に既存 wildcard が再び和へ混ざり得る。
- 参考コードの「行 vector に行列を掛ける」という comment と実際の `newdp = M * dp` の向きを混同せず、数式、model JSON、fixture で固定する。
- 生成 code から reference library 固有 macro と alias を除く。
- prime field 以外へ同じ elimination を適用しない。
- `assert` だけでなく、job report に具体的な失敗原因を返す。

## 12. 実装順

### P0-A: proof engine

1. prime field
2. batch scalar oracle
3. shortlex context generator
4. cached signature
5. incremental Gaussian basis
6. token matrix reconstruction
7. finite certificate
8. rank `r` の C++ codegen

### P0-B: contest engine

1. adaptive `P/Q` table
2. structured validation word
3. CEGIS counterexample refinement
4. dense / sparse portfolio
5. bundle / compile / stress
6. `checked / gamble` report

### P1

1. sparse basis search
2. RLE / block power
3. matrix product segment tree
4. multiple outputs
5. polynomial weight interpolation

## 13. 参考資料

- [えこってさんによるdpの推定手法の正当性について](https://maspypy.com/ecottea_dp_estimate_method)
- [maspypy library: ecottea_matrix_dp.hpp](https://maspypy.github.io/library/linalg/ecottea_matrix_dp.hpp.html)
- [ecotteaCP: 自動 DP](https://scrapbox.io/ecotteaCP/%E8%87%AA%E5%8B%95_DP)
- [AC 自動化ツール群仕様](ac_automation_tools_spec.md)
- [Non-word 愚直解高速化ツール群仕様](nonword_automation_tools_spec.md)
