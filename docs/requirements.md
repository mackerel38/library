# 競技プログラミング AC-first ライブラリ 要件・構想

- 状態: 要件整理ドラフト
- 作成日: 2026-07-12
- 最終更新日: 2026-07-13
- 対象: AtCoder の ABC C〜G、ARC、AGC
- 実装状況: 未着手。本書の合意後に設計・実装へ進む

## 1. 目的

このプロジェクトの最優先目標は、教材として美しいコードを書くことではなく、コンテスト中の **AC 到達時間を短縮すること** である。

問題固有の発想まで自動化できるとは考えない。その代わり、既知アルゴリズムの実装、定型的な変形、愚直解との比較、反例探索、計算量の選択を高速化し、人間が問題固有部分だけに集中できる状態を作る。

対象ごとの目標は次の通り。

| 対象 | 目標 |
| --- | --- |
| ABC C〜E | 頻出する再利用可能な部品をほぼ取りこぼさず、短く安全に書ける |
| ABC F〜G | 標準〜高度なアルゴリズム部分をすぐ組み立て、問題固有部分の検証へ移れる |
| ARC | 高度なデータ構造、グラフ、数論、文字列、DP 最適化を実戦投入できる |
| AGC | 固有の発想は人間が担い、高度部品、愚直検証、数列・遷移推定で実装と仮説検証を支える |

「網羅」は「すべての問題が一行で解ける」という意味ではない。次の状態を網羅と定義する。

1. 過去問の解法で使われる再利用可能な既知アルゴリズムが stable な部品として存在する。
2. 汎用ライブラリ化しにくい定型解法は recipe として即座に参照できる。
3. 問題固有の仮説は、愚直解、反例探索、推定ツールで短時間に検証できる。
4. 問題固有の観察だけが残り、既知部品の再実装がボトルネックにならない。

## 2. 決定済みの前提

- 新しい本体は /home/mackerel38/competitive/library にゼロから設計する。
- 旧 /home/mackerel38/kyopro/library は既知の欠陥を含む未信頼資料として扱い、コードを直接再利用しない。必要になった機能の痕跡、過去問 URL、失敗例だけを慎重に抽出する。
- C++ の header-only ライブラリを中心とする。
- 言語方言は GNU++23（-std=gnu++23）に固定する。
- 公開namespaceはハンドルネームに合わせてpoeとする。物理配置はcp/...のままとし、
  cpディレクトリをinclude rootにして利用時の先頭`cp/`を省く。
- 各公開ヘッダは内部依存を自分で include し、利用側は必要なヘッダを一つ include すればよい。
- 通常の.hppは物理1行目を#pragma once、物理2行目を#include <bits/stdc++.h>、3行目以降をローカル依存include、その後を実装とする。集約専用のall.hppは#pragma onceとローカル依存includeだけで構成する。
- .hpp 内では #define / #undef と macro API を禁止する。個人用 macro が必要なら solution template の .cpp にだけ置く。
- oj-bundle で単一提出ファイルに展開できることを必須とする。
- 公開部品をカテゴリ別に整理し、依存関係を循環のない DAG にする。
- コードの美しさより、実戦での入力量、事故率、検証可能性、改造しやすさを優先する。
- コンテスト用補助ツールの内部では LLM、生成モデル、クラウド AI API を一切動かさない。
- ABC、ARC、AGC の通常問題を先行し、AHC は初期スコープ外とする。

## 3. AC-first の設計原則

### 3.1 単純な解法を捨てない

- 制約内に収まるなら、複雑な最速実装より単純で壊れにくい実装を選べるようにする。
- 高速版だけでなく、差分検証用の愚直版・小制約版も reference として整備する。
- 畳み込みなどは、入力サイズに応じて naive と高速版を安全に切り替える。
- 実用上の閾値は理論値ではなく benchmark で決め、環境更新時に再測定する。

### 3.2 汎用版と即使用版を併設する

- モノイドや作用を指定できる汎用データ構造を持つ。
- 同時に Range Add / Range Sum など頻出用途の preset を持つ。
- 高度に一般化して呼び出し側の記述量やコンパイルエラーを増やさない。
- preset は汎用実装の薄いラッパーとし、別実装の乱立を避ける。

### 3.3 失敗を隠さない

- 未検証機能を stable に混ぜない。
- 前提を満たさない入力を「それらしい値」で処理しない。
- 事前条件、モノイド則、添字境界は tests/ の検査、または通常関数として明示的に呼べる check 機能で確認する。.hpp 内へ #ifdef LOCAL のような条件 macro を持ち込まない。
- 推定ツールは「反例が見つからない」と「正しさが保証された」を明確に区別する。

### 3.4 本番で改造できる

- 実装内部を過剰に抽象化せず、一つのヘッダを読めば局所修正できる。
- 型名・メソッド名は検索しやすく、短すぎる独自略語を避ける。
- グローバルマクロや暗黙の状態をライブラリ本体へ持ち込まない。
- 個人用の rep、all などを使う場合も、提出 template の .cpp に隔離し、.hpp には置かない。

## 4. 三層構成

本プロジェクトは、次の三つを別物として整備する。

| 層 | 役割 | 提出コードへ入るか |
| --- | --- | --- |
| Library | 再利用可能で verify 済みの C++ 部品 | 入る |
| Recipes | 問題固有部分を埋める定型コード、選択表、落とし穴 | 必要部分だけ利用 |
| AC Tools | stress、反例縮小、推定、計算計画、提出前検査 | 入らない。生成した短い呼び出しだけ入る |

Library だけを増やすと、digit DP、全方位木 DP、Aliens trick のように「型へ押し込むほど使いにくい」解法が漏れる。Recipes を独立させることで、実戦で必要な書き方を無理に万能 API にしない。

## 5. 暫定対象環境

- 主対象: AtCoder
- 言語方言: GNU++23（-std=gnu++23、確定）
- 基準コンパイラ: GCC 15.2.0 の AtCoder C++23 環境
- 提出用ヘッダの依存: GCC/libstdc++のbits/stdc++.hと、cpをinclude rootとするローカルヘッダだけ
- GNU 拡張: __int128、#pragma once など、明示して検証したものだけを許可
- 開発時検証: GCC の提出相当ビルド、sanitizer ビルド、libstdc++ debug mode
- 添字: 原則 0-indexed
- 区間: 原則半開区間 [l, r)
- 名前空間: poe（確定）

AtCoder は 2026-06-15 に C++23（GCC 15.2.0）環境を更新している。コンパイラやオプションは今後も変わり得るため、bench と preflight の環境情報を固定値ではなく設定として持つ。Clang/libc++ 互換は stable の合格条件に含めない。

## 6. 推奨ディレクトリ構成

`cp`ディレクトリをinclude rootとし、物理ファイルは次の構成を第一案とする。

    cp/
      core/
      io/
      utility/
      algebra/
      algorithm/
      structure/
      graph/
      tree/
      math/
      convolution/
      fps/
      string/
      geometry/
      dp/
      offline/
      game/
      experimental/
    template/
    recipes/
    reference/
    tools/
    verify/
    tests/
    benchmark/
    coverage/
    docs/

利用例は次の形を目標にする。

    #include "graph/dijkstra.hpp"

    poe::graph<long long> graph(n);
    graph.add_directed_edge(u, v, cost);
    auto result = poe::dijkstra(graph, source);

    #include "structure/rangeaddrangesum.hpp"

    poe::rangeaddrangesum<long long> seg(values);
    seg.apply(l, r, x);
    auto sum = seg.prod(l, r);

配置方針:

- cp/ 以下だけが提出用の公開ライブラリである。
- template/ は contest.cpp などの .cpp 雛形を置く。個人用 macro、型 alias、main はここに置き、.hpp は置かない。
- recipes/ は完成ヘッダではなく、使い分けと問題固有部分を含む定型を置く。
- reference/ は愚直実装や独立実装を置き、通常の bundle には含めない。
- tools/ は C++ ヘッダから完全に分離する。
- experimental/ は stable 部品へ依存してよいが、stable 部品から依存してはいけない。
- 即使用できるpreset型は`cp/structure/rangeaddrangesum.hpp`のように所有カテゴリへ置く。
- 個別 include を正規かつ最小の入口とする。
- cp/graph/all.hpp のようなカテゴリ集約と cp/all.hpp の stable 全体集約も用意する。
- 集約ヘッダは stable だけを読み込み、実装本体や内部ヘッダからは依存しない。

## 7. include と依存関係

### 7.1 依存レイヤ

下位レイヤだけを include できる一方向構造にする。同じレイヤ内の依存も原則禁止し、必要なら部品を一段下へ分離する。

    Layer 0: core
    Layer 1: io / utility / algebra concepts / math/basic / linalg containers / graph/base
    Layer 2: structure / number_theory / modint / algebra algorithms / linalg field algorithms / graph algorithms
    Layer 3: tree / offline / flow / convolution / polynomial algorithms
    Layer 4: fps / multipoint / recurrence / advanced algorithms
    Leaf:    category all.hpp / cp/all.hpp / recipes / experimental / tools

代表的にはconvolutionがmodintを、fpsがconvolutionを、treeとofflineがgraph/baseや
データ構造を利用する。逆方向の依存は許可しない。集約ヘッダ、recipes、experimental、toolsは
stable headerを利用できるが、stable headerからこれらへ依存してはいけない。

カテゴリ path として `cp/linalg/`、`cp/polynomial/`、`cp/recurrence/`、`cp/subset/`、`cp/number_theory/` を採用する。container と algorithm で必要 layer が異なる場合はカテゴリ単位で一つの layer に固定せず、`library.json` の module ごとに layer を持たせる。同カテゴリ内でも上位 module から下位 module への非循環依存だけを許し、共通処理は `row_reduction.hpp` のような下位 header へ分離する。P3 の tree decomposition は stable graph path を先約束せず、当初 `cp/experimental/graph/` に置く。

厳密なレイヤ番号は Phase 0 で manifest に確定し、CI が include graph を解析して循環と逆向き依存を拒否する。

### 7.2 ヘッダ要件

- cp/**/*.hpp の全ファイルに次の物理配置を適用する。detail、experimental、集約ヘッダも例外にしない。

      #pragma once
      #include <bits/stdc++.h>
      #include "依存先.hpp"

      namespace poe {
      // implementation
      }

- UTF-8 BOM、先頭コメント、空行を置かず、物理1行目を完全に #pragma once とする。
- 通常headerは物理2行目を完全に#include <bits/stdc++.h>とし、他のsystem includeを追加しない。all.hppはsystem includeを直接持たない。
- 3行目から必要な直接ローカル依存を、一行一 include で連続して置く。
- ローカルincludeは`#include "category/...hpp"`のliteral、cp include root相対、引用符形式に限定する。
- 依存 include 群の後に空行を一つ置き、それ以降を実装とする。実装開始後に include を追加しない。
- コメントとライセンス注記は依存 include 群より後にだけ置く。
- 公開ヘッダ単体を空の main から include してコンパイルできる。
- angle bracket、macro 展開、#if / #ifdef 内のローカル include を禁止する。
- 全 .hpp で #define、#undef、macro を公開 API にする実装を禁止する。
- using namespace std と非 inline なグローバル変数を禁止する。
- 内部 API は poe::detail に隔離し、公開 API から不要に露出しない。
- 同一処理の重複実装を避ける一方、低層部品へ巨大な依存を持ち込まない。
- include した一機能の展開サイズとコンパイル時間を計測できるようにする。
- ファイル末尾に改行を置き、上記形式を raw text lint で検査する。

### 7.3 oj-bundle の合格条件

1. solution 側は必要な公開ヘッダだけを include する。
2. oj-bundle 後の単一ファイルに未解決のローカル include が残らない。
3. bundle 後のファイルをジャッジ相当設定で再コンパイルできる。
4. bundle 前後でサンプルと回帰テストの出力が一致する。
5. 同じヘッダが複数経路から入っても定義重複が起きない。
6. 生成ファイルや未追跡ファイルへ暗黙に依存しない。
7. 公式wrapperがrepository rootを解決し、`oj-bundle -I REPO_ROOT/cp solution.cpp`の形で実行する。
8. 動作確認した oj-bundle のバージョンを開発環境に記録し、更新時に bundle regression を再実行する。

oj-bundle の include 検索先は実行時のカレントディレクトリが既定値であり、solution の位置から repository root を自動探索する前提にはしない。手動実行位置に依存させず、wrapper を正規の入口とする。

cp/**/*.hpp の固定先頭形式と新規実装方針を優先し、ACL、Boost、PBDS を提出ヘッダの依存にしない。これらや旧ライブラリをテスト時の比較対象に使うことはできるが、比較対象の一致だけで正しさを確定せず、仕様、愚直解、公開問題による独立検証を必須とする。

## 8. API 設計

### 8.1 共通規則

- 名前、配置、APIの共通方針は[ライブラリ実装規約](library_conventions.md)を正とする。
- 公開型は原則として小文字の`struct`とし、関数・メソッドも小文字を基本とする。
- 競プロで広く定着している leader、same、prod、all_prod、max_right、min_left などは、理由なく別名にしない。
- 無向辺と有向辺は add_undirected_edge、add_directed_edge のように曖昧さを避ける。
- 到達不能、解なし、負閉路、復元情報はカテゴリ内で統一したresult構造体にする。
- 距離だけでなく、必要に応じて親、辺 ID、復元メソッドを返す。
- 破壊的操作は名前または文書で明示する。
- 乱数を使う機能は seed を注入でき、失敗ケースを再現できる。
- 例外を通常フローに使わず、事前条件、明示的な戻り値、必要に応じて通常関数の check API を使う。assert の利用可否は macro 方針の確定後に決める。
- INF は加算で overflow しにくい型安全な定義を用意する。
- long long 固定にしすぎず、必要な箇所は重み型・値型を template にする。

### 8.2 汎用 API と preset

例:

- `segtree<monoid>`と`rangeminquery<T>`
- `lazysegtree<monoid, action>`と`rangeaddrangesum<T>`
- `graph<weight>`とunweighted用の簡潔なoverload
- `formalpowerseries<mint>`とmod 998244353用の短いalias

preset には次を明記する。

- 何を保持するか
- 更新と取得の区間規約
- overflow の可能性
- 空区間の返り値
- 汎用版へ移るべき条件

### 8.3 改造可能性

- 一機能は原則一ヘッダで追える大きさを目指す。
- パフォーマンス上の分岐や非自明な不変条件だけを短くコメントする。
- 教科書的な証明は docs へ分離するが、実装を壊しやすい前提はコード付近にも残す。
- 内部型を必要以上に private 化し、コンテスト中の局所改造を妨げない。

## 9. 収録カテゴリと優先度

以下の P0〜P2 は「どの問題帯を支える収録候補か」を示す coverage tier であり、そのまま次の実装順を意味しない。実装順は「頻度 × 事故率 × 実装時間 × 既存部品で代替できない度合い」で別に決め、小さな縦切りで投入する。

### P0: 基盤、ABC C〜E

| カテゴリ | 主な候補 |
| --- | --- |
| core / io | 型 alias、chmin/chmax、型安全 INF、int128 I/O、fast I/O |
| utility | 座標圧縮、累積和、imos、RLE、argsort、grid 方向、mex、部分集合・部分マスク列挙 |
| algorithm | 整数二分探索、実数二分探索、二点法、LIS、転倒数、meet-in-the-middle |
| structure | DSU、weighted DSU、Fenwick、SegTree、Sparse Table、区間集合、monotone queue、SWAG |
| graph | BFS、multi-source BFS、0-1 BFS、Dijkstra、Bellman-Ford、Floyd、topological sort、SCC、二部判定、functional graph |
| tree | 木の直径、部分木情報、Euler Tour、LCA、基本 rerooting |
| math | gcd、拡張 Euclid、CRT、floor sum、sieve、素因数分解、modint、階乗・二項係数、行列累乗、XOR 基底 |
| string | Z、prefix/KMP、rolling hash、Manacher、Trie |
| dp / recipe | bitset knapsack、復元補助、digit DP、区間 DP、bit DP の雛形 |

### P1: ABC F〜G、ARC 標準

| カテゴリ | 主な候補 |
| --- | --- |
| advanced structure | Lazy/Dual SegTree、Segment Tree Beats、rollback/persistent DSU、Treap、Binary Trie、Wavelet Matrix、Li Chao Tree |
| offline | Mo、Mo with updates、Hilbert order、parallel binary search、CDQ、offline dynamic connectivity |
| graph / flow | bridge、articulation、二重辺・二頂点連結成分、2-SAT、max flow、min-cost flow、lower-bound flow、matching、Euler trail |
| tree | HLD、汎用 rerooting、centroid decomposition、virtual tree、DSU on tree |
| math / linalg | Miller–Rabin、Pollard Rho、離散対数、mod sqrt、Gauss elimination、行列式、補間 |
| convolution | NTT、任意 mod 畳み込み、整数畳み込み、AND/OR/XOR・subset transform |
| string | suffix array/LCP、Aho–Corasick、suffix automaton、palindromic tree、run enumeration |
| geometry | 整数座標を優先した point、ccw、線分交差、凸包、点包含、回転キャリパー、最近点対 |

### P2: ARC 上位、AGC

| カテゴリ | 主な候補 |
| --- | --- |
| polynomial / FPS | polynomial 基本演算、inv、log、exp、pow、sqrt、微積分、Taylor shift、composition、multipoint evaluation、interpolation |
| recurrence | Berlekamp–Massey、Kitamasa、Bostan–Mori、有理型母関数の係数抽出 |
| DP optimization | divide-and-conquer optimization、Knuth、SMAWK、Monge、slope trick、Aliens trick |
| advanced graph/tree | directed MST、dominator tree、general matching、Link-Cut Tree、Euler Tour Tree |
| advanced algebra | polynomial gcd、linear recurrence over multiple moduli、subset convolution、nim product |
| game / reduction | Grundy、ゲーム和、flow・matching・2-SAT・CSP への定型帰着 |
| advanced geometry/string | half-plane intersection、Minkowski sum、高度な辞書順・周期構造 |

低頻度かつ壊れやすい部品は、数を揃えるためだけに stable へ入れない。experimental で複数の実戦検証を経てから昇格させる。

## 10. Recipes の役割

次のようなものは、万能 class より recipe として保持する方が実戦的である。

- digit DP、桁和・leading zero の扱い
- 全方位木 DP と復元
- bit DP、部分集合列挙、包除
- 座標圧縮と sweep line の組み合わせ
- parallel binary search、CDQ、Mo with updates
- sqrt decomposition、small-to-large、DSU on tree
- flow、matching、2-SAT への典型帰着
- 二分探索できる答え、Aliens trick
- divide-and-conquer / Knuth 最適化の適用形
- 行列累乗、線形漸化式、Bostan–Mori の選び分け
- sparse / dense、naive / NTT、全係数 / 一係数の選択
- constructive problem の出力検査

各 recipe は長い解説ではなく、次の一画面情報を優先する。

1. 使える形
2. 制約目安
3. 必要な include
4. 最小骨格
5. 壊れやすい前提
6. stress で比較する愚直形
7. 過去問タグ

## 11. AC 支援ツール

### 11.1 共通の流れ

1. 人間が愚直解、曖昧性なく解析できる数式表現、仮説、制約を入力する。
2. ツールが候補、反例、計算計画を決定的アルゴリズムで探索する。
3. 推定に使わなかったデータや独立実装で検証する。
4. 根拠、前提、失敗例とともに必要ヘッダと短い C++ 呼び出しを出力する。

ツールは答えを魔法のように生成するものではなく、仮説を高速に捨て、残った候補を AC 可能なコードへ接続するものとする。

### 11.2 結果と根拠の二軸表示

推定の成否と、その根拠の強さを同じ status に混ぜない。全ツールで次の二軸を共通表示する。

| 結果 | 意味 |
| --- | --- |
| FOUND | 候補、構成、計算計画が見つかった |
| REJECTED | 仮説への反例または不整合が見つかった |
| UNDERDETERMINED | 情報不足または rank 不足で一意に決まらない |
| ERROR | 入力不正、資源上限、内部失敗により判定できない |

| 根拠 | 意味 |
| --- | --- |
| EXACT | 明記した前提下で、代数変形や完全な構成により保証できる |
| EXHAUSTIVE_ON_DECLARED_DOMAIN | 宣言された有限領域だけを全探索した。その領域外への保証はない |
| CONSISTENT | 未使用データでも反例がなかっただけで、保証ではない |

全出力へ、前提、探索範囲、独立検証範囲、乱数 seed、最小反例、ツールバージョンを付ける。AC-first の目的に合わせ、EXHAUSTIVE_ON_DECLARED_DOMAIN と CONSISTENT からも提出用コードを生成できるようにする。共通の `--risk exact|checked|gamble` を持ち、既定の `checked` は規定の validation / final holdout / stress を通った推測候補まで許可する。`gamble` は証拠不足の候補も許可するが、既知の反例、未コンパイル、明白な制約超過がある候補は選ばない。詳細 contract は [AC 自動化ツール群仕様](ac_automation_tools_spec.md) に置く。

### 11.3 P0: ac-stress

最優先で整備する差分テスト・反例縮小基盤。

入力:

- 愚直解
- 高速解
- generator または入力 schema
- comparator
- 制約述語、timeout、seed

出力:

- WA、RE、TLE、非決定動作の分類
- seed 付き再現コマンド
- 制約を保った最小反例
- 両実装の出力、終了コード、時間

配列、木、連結グラフ、置換、区間、クエリ列などを理解する domain-aware generator と shrinker を段階的に持つ。縮小が制約を壊した場合は採用しない。

### 11.4 P0: ac-preflight

oj-bundle 後の、実際に提出する一ファイルを対象にする。

- AtCoder 相当設定でのコンパイル
- sample、保存済み回帰ケース、短時間 stress
- sanitizer と最適化ビルドの差分
- 未解決 local include、debug 出力、巨大静的領域、深い再帰の警告
- 時間、メモリ、NTT 長、overflow 候補の概算
- PASS / WARN / FAIL と根拠の一画面表示

自動修正や自動提出は初期スコープに含めない。時間推定は保証ではなく、安全率付きの参考値として表示する。

### 11.5 P1: seq2rec

小さい n まで愚直計算できる数列から、高速化候補を探す。

- Berlekamp–Massey
- Kitamasa
- Bostan–Mori
- companion matrix
- 有限差分、多項式、準多項式、周期列

必須要件:

- 候補選択用 validation と、最後まで候補選択に使わない final holdout を分ける。
- 次数が入力長を増やしても安定するか調べる。
- 整数列は複数素数上でも比較する。
- `checked` では標本不足や次数の不安定化を拒否する。`gamble` では不足量を明記して、既知の全項と整合する最上位候補を生成してよい。
- validation / final holdout で不一致になった候補は全 risk mode で棄却し、同じ候補を警告だけで通さない。
- 素数体モードと exact integer / rational モードを分離する。
- 初期版では複合数法を明示的に拒否し、体上の手法を誤適用しない。
- 複数素数上での一致は追加証拠であり、証明とは表示しない。
- 値が繰り返しただけでは周期としない。完全な決定的状態の再訪を確認した場合だけ厳密な周期とする。

Berlekamp–Massey が有限入力から厳密に求めるのは、その有限列に整合する最小漸化式であり、未知の無限列に対する正しさではない。有限標本だけから得た漸化式と companion matrix の根拠は常に CONSISTENT とする。また companion matrix は「観測列を再現する一つの実現」であり、元の DP の真の状態遷移とは限らない。この二つを UI と文書で混同しない。

### 11.6 P0: word-dp、P1: state2matrix、P3 experimental: probe2matrix

用途を明確に分ける。

- word-dp: 有限 alphabet 上の短い word に対する scalar oracle `f(word)` だけから、有限 Hankel table を使って各 token の未知遷移行列と最小可観測表現を復元する。
- state2matrix: 状態一覧と一ステップ遷移関数から、全状態を列挙して疎遷移行列を厳密に構築する。
- probe2matrix: 観測した y = A x の組から線形方程式を解き、行列候補を復元する。

word-dp は内部 state や一ステップ更新を要求しない。prime field 上で、`f(S)=u^T M[S_1]...M[S_n]v` という次元上限 `D` 以下の線形表現が存在するという contract の下では、長さ `D-1` 以下の全 context を使った有限検査により、最大長 `2D-1` の oracle 値から任意長 word に対する等価表現を構成できる。上限 `D` や線形性を有限観測から推測しただけなら CONSISTENT とする。

proof mode、adaptive Hankel / CEGIS mode、rank `r` へ縮約した codegen、oracle cache、sparse basis、RLE・segment tree backend を持つ。詳細は [Black-box Word DP / Hankel 復元仕様](word_dp_hankel_spec.md) に置く。

state2matrix が EXACT になるのは、状態集合が有限かつ遷移で閉じ、更新が時刻非依存で、指定した半環上で線形な場合だけである。列挙外状態への遷移は捨てずに拒否する。時刻や偶奇に依存する遷移は、それらを状態へ含めてから構築する。

probe2matrix の初期版は体上に限定する。既知次元の probe 入力が空間全体を張る場合だけ、線形モデルを仮定した一意解を返す。rank 不足時は適当な行列を一つ返さず、UNDERDETERMINED と nullspace を示す。full rank でも元遷移の線形性自体は証明されず、加法性・斉次性 probe は反例探索と CONSISTENT の根拠にしかならない。

追加候補:

- 到達不能状態の削除
- partition refinement による同値状態の併合
- 線形性を加法性・斉次性の probe で検査
- sparse matrix exponentiation
- 出力だけへの射影から線形漸化式を求める

### 11.7 P1: fps-plan

入力は曖昧性なく解析できる表現に限定する。第一案は小さな数式 DSL だが、C++ callable、係数 oracle、簡易数式記法も候補とし、使いやすさを比較して確定する。LLM を使わずに解釈が揺れる自然言語入力は採用しない。

入力:

- 法と必要次数 N
- 先頭 N 項すべてが必要か、第 K 項だけ必要か
- 加減乗、inv、log、exp、pow、微分、積分、係数抽出
- 既知 FPS、疎性、次数、定数項の前提
- 任意で小次数の愚直 evaluator
- 係数環と mod x^N による切り詰め意味論

探索候補:

- naive / NTT / 任意 mod convolution
- Newton 法による FPS 演算
- CDQ convolution
- sparse FPS
- multipoint evaluation / product tree
- 有理型母関数 + Bostan–Mori
- 線形漸化式 + Kitamasa
- 後期候補として陰関数型 FPS 方程式

出力:

- 正規化した式と計算 DAG
- 候補ごとの時間・メモリ計算量
- 採用した変形と、その前提
- 必要な include とライブラリ呼び出し
- 愚直評価との係数比較

各前提には由来を付ける。

- PROVED_FROM_AST: 式の構造から証明できた
- CHECKED_FROM_COEFFICIENTS: 指定範囲の係数で検査した
- USER_ASSUMED: ユーザーが宣言しただけ
- UNKNOWN / VIOLATED: 不明または違反

演算ごとに contract を持ち、少なくとも inv の定数項の可逆性、log の定数項 1、exp の定数項 0、積分で必要な整数の可逆性、pow / sqrt の分岐、Bostan–Mori の Q(0) の可逆性、NTT 長と原始根条件を検査する。「全係数」と「一係数」で計画を分ける。VIOLATED を含む計画は常に棄却し、USER_ASSUMED または UNKNOWN は evidence を下げて risk mode に従う。

### 11.8 P2: opt-scout

遷移コストや判定関数を小領域で全探索し、次の仮説の反例を探す。

- Monge 性、四辺形不等式
- argmin の単調性
- divide-and-conquer optimization
- Knuth optimization
- 凸性、凹性、unimodality
- 二分探索する述語の単調性
- eventual periodicity
- greedy の交換可能性

有限の全対象領域を列挙した場合以外は、FOUND + CONSISTENT としか表示しない。通った最適化案も ac-stress へ接続して高速版と愚直版を比較する。

### 11.9 P2: schema-casegen と meta-stress

schema-casegen は、手入力した構造から境界値、同値クラス、pairwise、乱数ケースを作る。

    n: int[1, 200000]
    a: array(n, int[-1000000000, 1000000000])
    constraint: sum(a) >= 0

木、DAG、連結グラフ、置換、区間、クエリ列の標準 generator を用意する。

meta-stress は愚直解がない場合に、人間が指定した metamorphic relation を検査する。

- 頂点番号の置換
- 座標の平行移動
- 辺やクエリの並べ替え
- 入力の反転
- 重みの定数倍

性質自体をツールが推測したことにはせず、ユーザーが指定した仮説として扱う。

### 11.10 その他の候補

| ツール | 目的 |
| --- | --- |
| poly-fit / gf-guess | 多項式、準多項式、有理・代数的母関数の候補探索 |
| ac-find | N、Q、更新・取得内容、graph 性質から候補ヘッダと recipe を検索 |
| assumption-ledger | 連結、素数法、単調性などの仮定を一覧化し、tests/ または solution.cpp 側の検査へ接続 |
| complexity-calibrator | 実測した操作単価から、時間・メモリの危険域を警告 |
| monoid-check | 単位元、結合則、作用の合成則を小領域で検査 |
| csp-fallback | bitset backtracking、branch-and-bound、DLX、2-SAT、flow への小制約帰着 |
| interactive-harness | ローカル judge、通信ログ、query 回数、flush、protocol の検査 |
| mutation-check | 典型バグを注入し、generator と comparator の検出力を測る |
| precision-lab | long double、整数化、exact rational を比較し、誤差ケースを探索 |

### 11.11 Non-word 愚直解高速化

`word-dp` の次に、対象を組み立てる操作または遅い遷移 kernel が見える場合を扱う。

- P0: `poly-fit / quasi-fit`。`seq` の項 cache から多項式・準多項式を復元する。
- P1: rational `gf-guess`。式候補の発見は `seq`、意味保存計算は `fps` が担当する。
- P1: `bag-dp`。個数 vector oracle から可換 generator を復元する。
- P1: `operator-id`。作用素を convolution、bitwise transform、低 rank 等へ同定する。
- P1: `grid-dp`。固定幅 grid を列 token または明示 frontier state へ変換する frontend とする。
- P2: `property-scout / dp-opt`。Monge、argmin 単調性、Knuth、CHT 等の根拠と反例を管理して高速候補を生成する。
- P2 experimental: `tree-dp`。tree context から leaf vector と merge tensor を復元する。
- P3 research: `graph-dp`。検証済み nice tree decomposition 上の many-sorted DP に限定する。

`bag-dp`、`tree-dp`、`word-dp` は context、Hankel basis、CEGIS、certificate を内部共有する。ただし最初から公開万能 `object-dp` を作らず、個別 frontend が安定してから共通化する。`grid-dp` は独立 learner を持たず、rule mode は `state-dp`、scalar oracle mode は `word-dp` へ接続する。

必要 library は、少なくとも modint、dense / sparse matrix、incremental Gaussian basis、interpolation、BM / Kitamasa / Bostan--Mori、convolution / FPS、subset transforms、低 rank 分解、D&C / Knuth / SMAWK / CHT / Li Chao、segment tree である。tree / graph の generic tensor は当初 tool-only とし、提出 code は非零項列へ特殊化する。planned header path と依存 DAG は [Non-word 愚直解高速化ツール群仕様](nonword_automation_tools_spec.md) を正とし、未実装 header を `library.json` へ先行登録しない。

有限 sample から model family や性質を推測した場合は `CONSISTENT` である。rank、次数、周期などの上限がユーザー仮定で、その下で一意復元できた場合は plugin metadata に `THEOREM_CONDITIONAL` を残すが、共通 global evidence は `CONSISTENT` とする。typed AST の登録変換や機械的に導いた上限下の完全 certificate だけを `EXACT`、有限宣言 domain の全検査を `EXHAUSTIVE_ON_DECLARED_DOMAIN` とする。

## 12. コンテスト規約とツール安全性

2026-07-13 に確認した AtCoder 生成 AI 対策ルール 20251003 版では、開催中の ABC、ARC、AGC で生成 AI の利用は原則禁止されている。一方で、次は許容例として明記されている。

- 生成 AI を使わない、ルールベースの問題文解析や入出力コード生成ツール
- コンテスト開始前に生成 AI を用いて作成済みのコード等
- 非 AI ベースの補完

本プロジェクトのコンテスト用 profile は「AtCoder ABC/ARC/AGC ruleset 20251003-compatible」と適用範囲を明記する。公式ルールが要求する範囲よりも判定を単純かつ保守的にするため、独自 policy として次を必須とする。

- モデル推論、LLM、生成 AI、AI コード補完を実行しない。
- クラウド API を呼ばない。
- contest profile では network access を fail-closed で無効化し、固定済みコードと決定的アルゴリズムだけを実行する。
- 使用する依存パッケージとバージョンを列挙し、AI 機能を含まないことを監査する。
- 決定的モードを持ち、seed と全入力を保存する。
- 問題文を外部へ送信しない。
- 自動 editorial 検索、自動 submission 検索、自動提出をコア機能に含めない。
- tools/RULES.md に公式 URL、確認日、適用 contest、依存監査結果を記録する。
- 主要コンテスト前、または公式ルール更新時に確認日を更新する。

ルールは将来変更され得るため、「一度安全だったので永久に安全」とは扱わない。AtCoder 以外で使う場合は、そのサイトと各 contest の個別規約を優先し、この profile 名だけで利用可能とは判断しない。

## 13. 検証と信頼性

### 13.1 ステータス

| 状態 | 利用方針 |
| --- | --- |
| stable | 本番利用対象。即使用型、集約ヘッダ、catalog に掲載 |
| beta | verify 済みだが実戦・境界検証が不足。明示 include のみ |
| experimental | 推測・試作段階。stable から依存禁止 |
| deprecated | 置換先を示し、新規利用を止める |

### 13.2 stable 昇格条件

- 公開ヘッダ単体でコンパイルできる。
- oj-bundle 後の一ファイルもコンパイル・実行できる。
- 機能を直接使う Library Checker、AOJ、AtCoder 等の verify がある。
- 適切な公開問題がなければ、独立した愚直解との exhaustive / random differential test がある。
- seed と失敗入力を保存し、再現できる。
- n = 0/1、空区間、非連結、多重辺、自己ループ、負辺、mod 境界、overflow をカテゴリ別に検査する。
- sanitizer と提出相当最適化の両方で通る。
- 最大制約付近の時間・メモリ benchmark がある。
- docs に API、計算量、前提、頻出 WA、verify 先、依存を記載する。
- 乱数・確率的機能は複数 seed と adversarial case を持つ。

一つの A+B 型 verifier で複数機能をまとめて「verify 済み」としない。高度機能は、オンライン judge と愚直差分の二つの独立な根拠を原則とする。

### 13.3 CI の責務

- 全公開ヘッダの standalone compile
- include graph の循環・禁止レイヤ検査
- verify と unit / property / fuzz regression
- bundle smoke test
- GCC の提出相当 / sanitizer / libstdc++ debug mode
- generated file、巨大 binary、precompiled header の混入検査
- benchmark の重大退行検出
- docs のヘッダパス・依存・ステータス整合性検査

verify、docs 生成、commit、push は別コマンドに分ける。検証スクリプトが暗黙に Git 操作や公開操作まで行わない。

## 14. ドキュメントと検索性

教材向けの長い証明より、本番中に一画面で判断できる情報を優先する。

各部品の docs:

1. include path
2. 何をしたいときに使うか
3. 最小使用例
4. API と戻り値
5. 計算量と実用的な制約目安
6. 前提条件
7. 頻出 WA / TLE / overflow
8. 類似手法との選び分け
9. 依存ヘッダと bundle サイズ
10. verify 済み問題と status

検索入口を二つ用意する。

- カテゴリから探す catalog
- 「N が 20」「区間加算・区間和」「k 番目」「一係数だけ」「負辺あり」のような制約・操作から探す reverse index

reverse index の最初の形は、例えば次のようにする。パスは API RFC で確定するまで第一案である。

| やりたいこと・条件 | 第一候補 | 選び分け |
| --- | --- | --- |
| 静的な区間和 | cp/utility/prefix_sum.hpp | 更新があるなら Fenwick / SegTree |
| 一点加算・区間和 | cp/structure/fenwicktree.hpp | 二分探索も必要ならFenwickの`lower_bound` |
| 区間加算・区間和 | cp/structure/range_add_range_sum.hpp | custom 作用なら汎用 LazySegTree |
| 区間の k 番目・x 以下の個数 | cp/structure/wavelet_matrix.hpp | 更新ありなら offline 化も検討 |
| 連結性を merge しながら判定 | cp/structure/dsu.hpp | 削除ありなら rollback + offline dynamic connectivity |
| 非負辺の最短路 | cp/graph/dijkstra.hpp | 0/1 辺は 0-1 BFS、負辺は Bellman–Ford |
| 木の path query | cp/tree/hld.hpp + SegTree | subtree だけなら Euler Tour で十分 |
| 一文字列の一致・周期 | Z / KMP | 多パターンなら Aho–Corasick |
| 和の畳み込みを全係数ほしい | convolution | 法、疎性、サイズで naive / NTT / arbitrary mod |
| 有理母関数の第 K 項だけほしい | Bostan–Mori | 全係数なら FPS、低次数漸化式なら Kitamasa |
| 小さい n の答えだけ愚直に出せる | seq2rec | 出力は有限列に対する推測として stress する |
| 状態遷移を列挙できる | state2matrix | 観測だけからの probe2matrix は experimental |
| DP 最適化条件が怪しい | opt-scout | 通過は証明でなく、まず反例探索として使う |
| 愚直解が書けない | meta-stress | 人間が不変な変換と答えの対応を指定する |

日本語 docs を第一言語とし、API、ファイル名、検索 alias は一般的な英語名も保持する。同義語、旧称、典型タグから同じ部品へ到達できるようにする。

## 15. 過去問によるライブラリ不足の確認

過去問を機械的に解法分類するのではなく、人間が解説・解法・実装を確認し、ライブラリに不足している再利用部品を書き出す。スクリプトで自動化してよいのは、対象問題一覧の取得、未記入行の生成、リンク確認、集計までとし、解法上の判断は自動化しない。

coverage/problems.tsv などに、各問題について次を人間が記録する。

- contest / task / problem URL
- 解法確認に使った editorial 等の URL
- 必要な再利用ライブラリ部品
- ライブラリ化しにくい定型部分・recipe
- 問題固有の核心・観察
- 有効そうな補助 tool
- 現在対応する header / recipe / tool
- 不足している機能または API
- 実装時に事故しやすい点
- 確認者、確認日、自由記述

例:

    contest: abcXXX
    task: f
    reusable_parts: LazySegTree, coordinate_compression
    recipe: sweep_line
    problem_specific_core: 区間を別の軸で見る観察
    missing: RangeAssignRangeMin の即使用型
    notes: 空区間と同値座標に注意

この記録は問題を一つのラベルへ押し込むためのものではない。「問題固有の発想が核心だが、実装には LazySegTree が必要」のように、核心と再利用部品を別々に文章で残す。

進め方:

1. まず直近 10 回、最大 50 問を人間が確認し、記録項目に過不足がないか確かめる。
2. 記録欄を確定した後、複数年代の問題を含む 30〜40 回程度へ広げる。
3. 新しい種類の不足が続く場合だけ対象を追加し、記録方法が安定したら全過去 ABC C〜Gへ進む。
4. ARC は頻出・代表解法と近年問題から始め、AGC は高度部品や補助 tool が有効な問題を優先する。
5. 確認方法を途中で変えた場合は、影響する既確認問題も見直す。

各確認 release は対象 contest の終了日 cutoff を固定する。最初の全件確認候補は 2026-07-12 までに終了した ABC の、存在する C〜G とする。

不足確認完了の条件:

1. cutoff までの対象問題一覧が固定され、未確認が 0 件である。
2. 全問題に再利用部品、定型部分、問題固有の核心、不足、根拠 URL、確認者が記録されている。
3. 見つかった不足が stable header、recipe、tool、または明示的な backlog に結びついている。
4. backlog だけに結びつく項目を対応済みとは数えない。
5. 件数を分母・分子付きで報告し、「ほぼ網羅」のような曖昧な完了表現を使わない。

ABC C〜E では未実装の反復部品を 0 件にすることを v1 の完了目標とする。ABC F〜G、ARC、AGC は不足件数と backlog を公開し、新しい反復パターンが見つかるたびに実戦価値で優先順位を更新する。

## 16. 開発ロードマップ

### Phase 0: 要件確定と検証基盤

- 本書の未決事項を確定する。
- Git、ライセンス、C++ 設定、命名、dependency manifest を用意する。
- standalone compile、oj-bundle、random differential test の最小基盤を作る。
- 過去問不足確認の記録項目を確定し、直近 ABC の人手確認を始める。

### Phase 1: ABC C〜E stable

- Phase 1A: compile / bundle / differential test の縦切り、template、core、ac-stress 最小版
- Phase 1B: 座標圧縮・累積和・二分探索、DSU、Fenwick、SegTree、BFS、Dijkstra、modint、組合せ
- Phase 1C: LazySegTree、SCC、LCA、基本文字列、行列、ac-preflight 最小版
- 各小段階を実戦投入し、API の不便と事故を次へ反映する

### Phase 2: ABC F〜G、ARC 標準

- 高度なデータ構造、flow、tree、offline、数論、convolution
- recipes と reverse index
- 過去問を解き直し、不足 API を追加する

### Phase 3: FPS・漸化式・DP 最適化

- FPS の stable core
- Bostan–Mori、Berlekamp–Massey、Kitamasa
- seq2rec と、曖昧性なく解析できる入力に限定した fps-plan
- opt-scout

### Phase 4: AGC 向け高度部品と推定

- state2matrix / probe2matrix
- 高度 graph/tree/algebra
- schema-aware generator、metamorphic test
- experimental の実戦検証と昇格

Phase の番号は実装順であり、すべてを完成させてから使い始めるという意味ではない。各 Phase で stable な小さい縦切りを作り、実戦で検証する。

## 17. 非目標

- 初学者向け教科書や長い証明集
- あらゆる問題を一行で解く万能 framework
- 問題固有の観察・構築・証明の完全自動化
- LLM や外部 AI サービスをコンテスト中に使う機能
- 検証不足の理論上最速実装を stable として大量収録すること
- 美しい抽象化のために記述量やコンパイルエラーを増やすこと
- submission header から外部 runtime、network、別ビルドを要求すること
- 初期段階で AHC と通常問題を同じ設計へ詰め込むこと
- 自動提出や judge への過剰アクセス

## 18. 旧ライブラリから得た比較材料

旧 /home/mackerel38/kyopro/library は欠陥を含むことが分かっており、正しい既存資産や oracle として扱わない。数論、データ構造、verify のまとまった形跡はある一方、graph、string、geometry の実装がほぼなく、FPS の一部が二乗時間かつ未 verify、補助ツールも未整備である。

新規側では旧コード、旧 API、旧 docs、旧 verify 結果のいずれも正しさの根拠にしない。dirty worktree の未 commit ファイルはもちろん、追跡済み HEAD も未信頼資料として慎重に読む。その上で、次だけを監査材料として利用する。

- 実戦で必要になった機能一覧
- 過去に必要になったらしい API 操作
- 独立に再確認するための過去問 URL
- 未 verify、低速、依存不整合だった箇所
- 巨大 template、生成物混入、verify と publish の結合といった避けるべき点

旧実装と同じ結果になっただけでは検証完了としない。各部品は仕様から再設計し、別に書いた愚直解、境界テスト、公開問題で独立検証する。この方針により、互換性や旧来の欠陥のために新設計を歪めず、過去に必要だった機能だけを見落とさない。

## 19. 残る確認事項と具体例

GNU++23、AtCoder 主対象、固定ヘッダ形式、.hpp 内の macro 定義禁止、ACL・Boost・PBDSへ提出ヘッダから依存しないことは確定済みとする。

### 19.1 補助ツールの実装形態

この質問は提出言語を尋ねるものではない。提出物はどの案でも oj-bundle 済みの C++ 一ファイルである。確認したいのは、提出前にローカルで動かす stress、推定、反例縮小などを何で実装し、コンテスト PC へ何を準備するかである。

操作イメージ:

    python3 tools/ac_stress.py --generator ./gen --brute ./brute --fast ./main --seed 42

generator、愚直 C++、高速 C++ を繰り返し実行し、不一致を見つけたら seed、両出力、縮小した反例を返す。

    python3 tools/seq2rec.py --oracle ./brute --mod 998244353 --terms 200 --target 1000000000000000000

愚直 C++ から有限列を集め、漸化式候補を validation / final holdout で検査し、必要ヘッダ、係数、C++ 呼び出し例を返す。`checked` では規定検査済みの推測から警告付き提出コードまで生成し、`gamble` では証拠不足も明記して生成する。

    python3 tools/fps_plan.py formula.toml

formula.toml に法、必要次数、全係数か第 K 項だけか、式の構造を記述し、naive / NTT / FPS / Bostan–Mori などの候補と前提検査を返す。

| 方式 | 向いていること | 主な弱点 |
| --- | --- | --- |
| Python 標準ライブラリ CLI | process起動、timeout、入出力、設定解析、反例縮小を短く安全に実装 | Python 3 の事前準備が必要 |
| C++ CLI | BM、FPS、行列など重い計算を高速に実行し、提出ライブラリを直接再利用 | process管理、設定parser、エラー表示、縮小器の開発が重い |
| 任意 SymPy plugin | 有理式整理、因数分解、微分、母関数変形 | 起動・式膨張・version差・導入忘れ |
| 任意 Z3 plugin | 制約を保つ小反例や CSP の探索 | solver式の準備が必要で、探索不能は証明にならない |
| local Web UI | 数式、遷移、反例、計算 DAG を視覚的に入力・確認 | server、port、browser が故障点になり得る |

第一案は「Python 3 標準ライブラリだけで動く CLI を必須部分とし、重い計算は C++ helper を呼ぶ」である。SymPy / Z3 は事前導入済みの場合だけ使える optional plugin とし、未導入時に自動 install や network access を行わない。Web UI は同じ CLI を呼ぶ薄い画面として後から追加する。

決定: この第一案で進める。必須部分は Python 3 標準ライブラリ、重い計算と提出先は C++ とし、optional dependency は明示的に分離する。

### 19.2 過去問による不足確認（確定）

問題を機械的に分類しない。人間が解説・解法・実装を確認し、必要な再利用部品、定型部分、問題固有の核心、現在不足している API を文章で記録する。

スクリプトは問題一覧と空の記入欄を作り、未記入、存在しない header、URL 形式などを検査するだけとする。問題の解法、核心、必要 algorithm は判断しない。

まず直近 10 回程度で記録項目を試し、使いにくければ直してから複数年代と全過去問へ広げる。この小規模確認は分類精度を測るためではなく、人間が書く記録欄の過不足を早期に見つけるために行う。

### 19.3 macro 禁止の厳密な範囲（確定）

.hpp 内で #define / #undef を書かず、macro を API として提供しない。#ifdef LOCAL も使わない。一方、標準の assert のように、他のヘッダで既に定義された既存 macro を呼ぶことは許可する。事前条件や壊れやすい内部不変条件には、実戦上有用な assert を積極的に利用してよい。

### 19.4 推測結果からのコード生成

決定: CONSISTENT にすぎない漸化式や最適化候補からもコード生成する。

- `--risk exact`: EXACT だけを生成する。
- `--risk checked`: 規定の validation / final holdout / stress を通った CONSISTENT も生成する。既定値。
- `--risk gamble`: 検証量不足や候補の非一意性が残っても、既知の反例がない最上位候補を生成する。

どの mode でも evidence と検証範囲を report と生成コードの先頭へ残す。既知の反例がある候補を `gamble` で強制採用する機能は作らない。詳細は [AC 自動化ツール群仕様](ac_automation_tools_spec.md) に従う。

後から決めてもよいもの:

- 公開 repository とライセンス
- local docs の CLI / Web UI
- interactive problem の優先度
- Clang や C++20 への portability
- API の破壊的変更を止める stable 宣言の時期

## 20. 次の要件整理ステップ

実装へ入る前に次を行う。

1. [AC 自動化ツール群仕様](ac_automation_tools_spec.md) の Phase 0 に従い、job / candidate / evidence の共通 contract を実装する。
2. state-dp の emit / symbolic lifting を最初の縦切りとして実装する。
3. word-dp の scalar oracle、証明付き Hankel 復元、adaptive CEGIS、compact codegen を実装する。
4. seq と FPS の独立愚直 interpreter、holdout、codegen に `poly-fit` と rational `gf-guess` を統合する。
5. word-dp の共通 basis を再利用して `bag-dp` と `grid-dp` frontend を実装する。
6. `operator-id` の小さい exact family と、report-only `property-scout` を実装する。
7. `dp-opt`、`tree-dp`、`graph-dp` は focused spec の段階的完了条件に従い、低リスク候補から追加する。
8. ABC 過去問監査の schema と「対応済み」の判定基準を確定する。
9. P0 library 部品を優先順位付き backlog にし、generator が必要とする API から整備する。

## 参考

- AtCoder 生成 AI 対策ルール 20251003 版: https://info.atcoder.jp/entry/llm-rules-ja
- AtCoder C++23（GCC 15.2.0）環境更新: https://atcoder.jp/posts/language_update20260616_jp
- online-judge-verify-helper reference: https://online-judge-tools.github.io/verification-helper/document.ja.html
- 開発支援ツール案: [development_tools.md](development_tools.md)
- AC 自動化ツール群仕様: [ac_automation_tools_spec.md](ac_automation_tools_spec.md)
- Black-box Word DP / Hankel 復元仕様: [word_dp_hankel_spec.md](word_dp_hankel_spec.md)
- Non-word 愚直解高速化ツール群仕様: [nonword_automation_tools_spec.md](nonword_automation_tools_spec.md)
