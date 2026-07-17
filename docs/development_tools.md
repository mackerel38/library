# 競プロライブラリ開発支援ツール案

- 状態: P0 実装開始
- 作成日: 2026-07-13
- 対象: ライブラリ部品を安全かつ速く追加・変更するためのローカル開発ツール
- 実装状況: P0の最初の縦切りを実装済み

現在実装済み:

- doctor
- new-header
- lint
- deps check / why / affected
- compile-header
- bundle
- check（quick、compiler matrixまでのfull、Git初期化後のchanged）
- stress（seed・具体入力・実行証拠の保存）

new-test、oj-verifyの振り分け、benchmark、promote、catalog等は次段階で実装する。

文書同期は独立した小さいCLIで実装済み。

    python3 tools/docs.py sync
    python3 tools/docs.py check

`sync`は各`docs/...md`へoj-verifyの`documentation_of` Front Matterを付け、
headerの公開`///`コメントと宣言から`APIリファレンス`を更新する。自動生成marker内は直接編集しない。
`check`は190個のheaderページについて、Front Matter、include、公開symbol、APIリファレンス、
実在問題例または明示的な省略を検査する。

静的サイトの生成は次で行う。

    python3 tools/docs.py build

生成先は`.verify-helper/markdown/`である。`.github/workflows/docs.yml`では同じ文書検査と生成を行い、
JekyllでHTML化してGitHub Pagesへdeployする。現在の`verify/*.cpp`は提出用codeであり、
oj-verifyの検証file命名`*.test.cpp`ではないため、site生成時の検証status集計には含めない。
各pageにはdocsに記録したsample確認・judge AC状態を表示する。

## 1. 結論

最初は Python 3 標準ライブラリだけで動く単一 CLI、tools/dev.py を用意するのがよい。Python は header の生成・検査、g++ と oj-bundle の起動、依存解析、test の振り分けを行う司令塔であり、提出コードには入らない。アルゴリズム本体、愚直解、性能が必要な検査は C++ で書く。

最初から必要な P0 は次の七つに絞る。

1. 開発環境確認
2. header・docs・test の雛形生成
3. 固定 header 形式の lint
4. include 依存 DAG の検査
5. header 単体・ODR・usage compile
6. oj-bundle 後の compile・挙動検査
7. 関連 test をまとめて実行する一括 gate

愚直差分、失敗入力保存、oj-verify も、最初の部品を stable にするまでには必要とする。一方、Web UI、mutation test、高度な fuzz、巨大な dashboard は後回しにする。

## 2. ツールが行わないこと

- 問題文から必要なアルゴリズムを自動推測しない。
- 過去問を機械的に解法分類しない。
- library の実装本体を自動生成しない。
- beta / experimental を自動で stable へ昇格しない。
- Git commit、push、公開、提出を暗黙に行わない。
- LLM、生成 AI、cloud API、network access を使わない。
- test の SKIP や未登録を PASS と表示しない。

自動化するのは機械的に正誤を判定できる部分だけとし、API の使いやすさ、前提の妥当性、過去問の核心、stable 昇格の最終判断は人間が行う。

## 3. 単一 CLI と正本

コマンドは次の形へ集約する。

    python3 tools/dev.py <subcommand> [options]

理由:

- path、JSON、subprocess、timeout、一時ファイルを標準ライブラリだけで扱える。
- 複数 shell script に compiler flag や repository root が散らばらない。
- human-readable 出力と将来の --json 出力を同じ実装から出せる。
- C++ で process 管理や設定 parser を作るより、本体ライブラリへ時間を使える。

正本は次のように分ける。

| 情報 | 正本 |
| --- | --- |
| 実際の依存辺 | hpp内のcp include root相対`#include "category/..."` |
| layer、status、tags、対応test | library metadata |
| API、計算量、前提、頻出事故 | docs |
| 公開問題での検証 | verify/*.test.cpp |
| 愚直差分と境界検査 | tests/ |
| 失敗した具体入力 | tests/regression/ |

依存先を header と metadata の両方へ書かない。二重管理すると必ず不整合が起きるため、依存 graph は include 文から導出する。

metadata は Python 標準ライブラリで安全に読み書きできる JSON を第一案とする。保持するのは path、layer、status、summary、symbols、tags、docs、test、verify、benchmark の対応であり、依存一覧は保持しない。

CLIとmetadataでは物理配置を表すrepository root相対の`cp/...`を使う。一方、C++のinclude文では`cp`をinclude rootとして、先頭の`cp/`を付けない。

新規部品の status は必ず experimental から始める。ツールは昇格条件を検査して報告するが、status 自体を自動変更しない。

## 4. P0: 最初から必要な subcommand

### 4.1 doctor

    python3 tools/dev.py doctor

表示・検査:

- repository root
- Python の実体と version
- g++ の実体と version
- GNU++23 の compile 可否
- oj-bundle の実体と起動可否
- 使用する compile profile
- metadata、docs、tests、verify directory の整合性

不足がある場合は silent skip せず FAIL とし、修正方法を表示する。doctor 自体は install や設定変更を行わない。

### 4.2 new-header

    python3 tools/dev.py new-header cp/structure/fenwicktree.hpp --layer 2 --dep cp/core/types.hpp --with-smoke
    python3 tools/dev.py new-header cp/structure/fenwicktree.hpp --dry-run

生成候補:

- cp/structure/fenwicktree.hpp
- docs/structure/fenwicktree.md
- tests/api/structure/fenwicktree.cpp
- metadata の experimental entry

header は必ず次の形式から始める。

    #pragma once
    #include <bits/stdc++.h>
    #include "core/types.hpp"

    namespace poe {
    }

要件:

- 実装やアルゴリズムは生成しない。
- 既存ファイルを上書きしない。
- path、category、snake_case、依存先の存在、依存 layer を生成前に検査する。
- 直接依存 include は一行一つ、重複なし、path 辞書順にする。
- `cp/<category>/<name>.hpp`には`docs/<category>/<name>.md`を一対一で生成する。
- docsには実在する公開問題の使用例とjudge AC確認状態を書く欄を生成する。
- --dry-run で全生成予定を確認できる。

### 4.3 lint

    python3 tools/dev.py lint cp/structure/fenwicktree.hpp
    python3 tools/dev.py lint --all

検査:

- UTF-8 BOM なし、LF、末尾改行
- 物理1行目が完全に #pragma once
- 通常headerの物理2行目が完全に #include <bits/stdc++.h>
- all.hppは#pragma onceとlocal includeだけで、bits includeや実装を持たない
- 3行目以降のlocal includeが連続し、literal、cp include root相対、一行一つ
- local include の重複、順序、存在
- include 群後の空行
- 実装開始後の include
- 追加 system include、angle bracket の依存 include、条件 include
- #define、#undef、macro API
- using namespace std

単純な grep だけではコメントや文字列を誤検知するため、preprocessor directive とコメントを区別する小さな lexer を使う。assert 等の既存 macro 呼び出しも禁止する場合は、別の明示規則として追加する。

### 4.4 deps

    python3 tools/dev.py deps check
    python3 tools/dev.py deps why cp/fps/exp.hpp cp/convolution/ntt.hpp
    python3 tools/dev.py deps affected cp/math/modint.hpp

検査:

- 存在しない local include
- 自己依存と cycle
- 同 layer 依存と上位 layer 依存
- stable から beta / experimental への依存
- 通常 header から category all.hpp / cp/all.hpp への依存
- deprecated header への新規依存
- all.hpp以外のheaderと同名docsが一対一で存在すること

affected は逆依存 closure を返し、低層 header 変更時に再実行すべき test、verify、bundle test、benchmark を列挙する。初期版では C++ 構文解析を行わず、固定形式の include graph だけを使う。

### 4.5 compile-header

    python3 tools/dev.py compile-header cp/structure/fenwicktree.hpp
    python3 tools/dev.py compile-header --all

一つの header に対して次を検査する。

1. header だけを include した一時 TU の compile
2. 同じ header を二回 include した TU の compile
3. 二つの別 TU から include して link する ODR 検査
4. 対応する usage smoke test の compile・run

空の main から include しただけでは template method が実体化されない。このため各公開 header へ、代表型と主要 method を実際に使う usage smoke test を最低一つ対応させる。

standalone compile が通っても「意味上必要な直接依存をすべて include している」ことまでは完全自動判定できない。この点は review 項目として残す。

### 4.6 bundle

    python3 tools/dev.py bundle verify/fenwicktree.test.cpp --output build/fenwicktree.bundle.cpp
    python3 tools/dev.py bundle --header cp/structure/fenwicktree.hpp

内部では常に repository root を解決し、次の形で実行する。

    oj-bundle -I REPOSITORY_ROOT/cp solution.cpp

検査:

- bundle後にライブラリのlocal includeが残っていない
- bundle 後の一ファイルが GNU++23 で compile できる
- 指定 input で bundle 前後の stdout、stderr、exit code が一致する
- synthetic solution でも対象 header を実際に instantiate する

oj-bundle 未導入や起動失敗を無視しない。local quick check では明示 WARN、stable gate と CI では FAIL とする。

### 4.7 check

    python3 tools/dev.py check cp/structure/fenwicktree.hpp --profile quick
    python3 tools/dev.py check cp/structure/fenwicktree.hpp --profile full
    python3 tools/dev.py check --changed
    python3 tools/dev.py check --all

quick:

- lint
- deps
- standalone / ODR / usage compile
- 保存済み regression
- 短時間 unit / property
- bundle compile

full（現時点ではcompiler matrixまで実装）:

- quick の全項目
- ASan + UBSan
- libstdc++ debug mode
- 長時間 differential test（未統合）
- 対応する offline verify（未統合）
- 最大制約 benchmark（未統合）

出力は PASS / WARN / FAIL / SKIP を分け、失敗工程、具体入力、compiler、flag、seed、再現コマンドを表示する。SKIP がある full check を stable 合格として扱わない。

--changed は Git repository 初期化後に、変更 header と逆依存先だけを検査する。初期化前は対象 path 指定または --all を使う。

## 5. P0: test と検証根拠

### 5.1 new-test（次段階）

    python3 tools/dev.py new-test cp/structure/fenwicktree.hpp --type usage
    python3 tools/dev.py new-test cp/structure/fenwicktree.hpp --type differential
    python3 tools/dev.py new-test cp/structure/fenwicktree.hpp --problem-url URL

作るのは雛形と metadata 対応だけであり、正解ロジックは生成しない。

各 stable 候補は原則として次を持つ。

- API を実体化する usage smoke
- 境界・小領域の unit / exhaustive test
- 独立した愚直解との random differential test
- Library Checker、AOJ 等の直接 verify
- bundle 後の integration test

適切な公開問題がない場合は、公開問題を無理に当てず、理由と独立検証を docs に記録する。

### 5.2 stress と regression corpus

    python3 tools/dev.py stress --generator ./gen --oracle ./brute --candidate ./fast --seed 42
    python3 tools/dev.py regression cp/structure/fenwicktree.hpp

不一致時に保存するもの:

- seed と case index
- 縮小前後の具体入力
- oracle / candidate の stdout、stderr、exit code、実行時間
- compiler、flag、実行ファイル hash
- WA / RE / TLE / NONDETERMINISTIC の失敗種別
- 再現コマンド

seed だけでは不十分である。generator を修正すると同じ seed から別入力が出るため、失敗した入力本体も tests/regression/ に保存する。

oracle と candidate が同じ helper を共有すると同じバグを持ち得るため、愚直解は可能な限り独立に書く。

反例縮小は初期版では token・行・操作列の削除から始め、配列、木、連結 graph、置換、FPS などの schema-aware shrink は後から追加する。不正入力で別の失敗を起こしただけの縮小は採用しない。

### 5.3 build profile

最低限、次を分ける。

- judge: GNU++23、O2
- sanitizer: ASan + UBSan、debug information
- libstdc++ debug: iterator・container 検査

sanitizer や debug build の時間・メモリを AtCoder 制限と比較しない。performance 判定は judge profile だけで行う。

## 6. P1: 部品が増えてから追加

### 6.1 promote

    python3 tools/dev.py promote cp/structure/fenwicktree.hpp

出力例:

    PASS  header format
    PASS  dependency layer
    PASS  usage smoke
    PASS  differential test
    PASS  sanitizer
    PASS  oj-bundle
    FAIL  maximum benchmark missing
    WARN  empty range behavior missing in docs
    RESULT: keep beta

ツールは status を変更せず、機械判定結果と人間 review 項目を分けて表示する。

### 6.2 generate-all と catalog

    python3 tools/dev.py generate-all --check
    python3 tools/dev.py generate-all --write
    python3 tools/dev.py catalog --check

metadata で stable の部品だけから category all.hpp と cp/all.hpp を決定的に生成する。--check は生成結果との差分だけを調べ、--write を明示した場合だけ更新する。

catalog は docs、tags、symbols、status から生成する。依存 graph や problem 解法を推測しない。

### 6.3 find / show

    python3 tools/dev.py find --tag 一点加算 --tag 区間和
    python3 tools/dev.py find "負辺あり 最短路"
    python3 tools/dev.py show cp/structure/fenwicktree.hpp

人間が整備した日本語・英語 tags と reverse index を検索し、include、status、計算量、選び分け、頻出事故、compile 済み使用例を表示する。LLM や意味推測は使わない。

### 6.4 impact

    python3 tools/dev.py impact cp/algebra/monoid.hpp

表示:

- 直接・推移的な逆依存
- 影響する stable 部品
- 再実行する usage、unit、verify、bundle、benchmark
- 影響する category all.hpp と cp/all.hpp

### 6.5 cost / benchmark

    python3 tools/dev.py cost cp/fps/formal_power_series.hpp
    python3 tools/dev.py benchmark cp/convolution/ntt.hpp --compare benchmark/baseline.json

記録:

- header 単体 compile 時間
- bundle 後の行数・bytes
- binary size
- 実行時間の median と大きな揺れ
- 最大 RSS
- naive / 高速版の交差点
- 最大制約での安全率

小さな環境差は WARN とし、制限超過や約二倍化など重大退行だけを FAIL 候補にする。baseline は自動更新しない。

### 6.6 API・example regression

stable 部品だけについて、tests/api/ の最小使用例と実際の過去問 wrapper を compile する。header 全文 hash、compiler 診断文、detail 型は固定しない。

docs の code block を直接抽出する高度な仕組みは後回しにし、最初は compile 可能な実体 .cpp を正本にする。

### 6.7 problems

    python3 tools/dev.py problems new abc420_c
    python3 tools/dev.py problems validate
    python3 tools/dev.py problems report

new が用意するのは contest、task、URL、空の記入欄だけである。必要部品、定型部分、問題固有の核心、不足 API、根拠 URL、確認者は人間が記入する。

validate が行うのは未記入、存在しない header、壊れた URL 形式、backlog と対応済み表示の矛盾などの形式検査だけであり、問題を分類したり解法を判断したりしない。

## 7. P2: 後回しにするもの

- include 順を randomize する衝突検査
- schema-aware fuzz と metamorphic test
- 競プロ固有 mutation test
- local Web UI
- 長時間 nightly test
- advanced benchmark dashboard
- compiler を増やした portability test

mutation test は実装の正しさではなく test の検出力を測る。test が少ない段階で導入しても価値が低く、library 追加開始を遅らせるため P2 とする。

## 8. 一つの部品を追加する流れ

Fenwick Tree を例にする。

    python3 tools/dev.py doctor
    python3 tools/dev.py new-header cp/structure/fenwicktree.hpp --layer 2 --with-smoke
    python3 tools/dev.py new-test cp/structure/fenwicktree.hpp --type differential

人間が API、前提、docs、実装、独立した愚直解を書く。

    python3 tools/dev.py check cp/structure/fenwicktree.hpp --profile quick
    python3 tools/dev.py stress --generator ./gen --oracle ./brute --candidate ./fast --seed 42
    python3 tools/dev.py check cp/structure/fenwicktree.hpp --profile full
    python3 tools/dev.py promote cp/structure/fenwicktree.hpp

promote の全機械検査が通った後も、人間が API、計算量、境界、頻出事故、検証根拠を確認してから status を変更する。

## 9. 推奨実装順

library 本体より先に巨大な tool 群を完成させない。最初の縦切りは次だけにする。

1. doctor と共通設定
2. new-header + lint
3. deps check
4. standalone / ODR / usage compile
5. bundle check
6. check に統合

この時点で最初の小さな library 部品を追加し、実際の不便を見て tool を修正する。最初の部品を stable にするまでに stress、regression corpus、build profile、oj-verify 対応を追加する。

promote、catalog、find、impact、cost は数本の部品ができてから追加し、mutation、Web UI、advanced fuzz は明確な必要が出るまで作らない。

## 10. 確定事項

1. hpp では #define / #undef と macro API を禁止するが、assert 等の既存 macro 呼び出しは許可する。
2. metadata の初期形式は、Python 標準ライブラリで安全に読み書きできる JSON とする。
3. tools/dev.py の必須部分は Python 標準ライブラリ、重い処理と提出先は C++ とする。
