このコードは Codex を用いて作成されています。

# poe competitive library

GNU++23 / AtCoder向けのAC-first競技プログラミングライブラリです。

公開namespaceは`poe`です。物理ファイルは`cp/`以下に置き、`cp`ディレクトリを
include rootとして使うため、提出コードでは先頭の`cp/`を書きません。

    #include "graph/graph.hpp"
    #include "tree/lca.hpp"

コンパイル時は、このリポジトリ内なら次のように指定します。

    g++ -std=gnu++23 -Icp main.cpp

別ディレクトリから使う場合は、`-I/home/mackerel38/competitive/library/cp`のように
`cp`の絶対パスを渡します。

ACL相当の `structure`・`math`・`graph`・`tree`・`flow`・`string`、DP部品の`dp`、
形式的冪級数の`fps`、2次元幾何の`geometry`、用途別の`utility`と、
一括include用の物理ファイル`cp/all.hpp`は、`#include "all.hpp"`で使います。
狭い問題族向けの完成形は`recipe`に分離し、意図しない依存を避けるため`all.hpp`には含めません。

## 開発CLI

環境確認:

    python3 tools/dev.py doctor

新規headerのdry-run:

    python3 tools/dev.py new-header cp/core/example.hpp --layer 0 --with-smoke --dry-run

検査:

    python3 tools/dev.py lint --all
    python3 tools/dev.py deps check
    python3 tools/dev.py check --all --profile quick

愚直差分:

    python3 tools/dev.py stress --name example --generator "./gen" --oracle "./brute" --candidate "./main" --seed 42

## 文書

GitHub Pages用の文書はoj-verify形式で生成する。

    python3 tools/docs.py sync
    python3 tools/docs.py check
    python3 tools/docs.py build

生成物は`.verify-helper/markdown/`に置かれる。GitHubでは
`.github/workflows/docs.yml`がJekyll buildとPagesへのdeployを行う。
リポジトリのSettings > PagesでSourceを`GitHub Actions`に設定すればよい。

- [要件・構想](docs/requirements.md)
- [ライブラリ拡張予定](docs/roadmap.md)
- [ライブラリ実装規約](docs/library_conventions.md)
- [DPライブラリ選択ガイド](docs/dp/guide.md)
- [EDPC・TDPC・NDPC分類](docs/dp/catalog.md)
- [ACL相当ライブラリ一覧](docs/acl.md)
- [開発支援ツール](docs/development_tools.md)
- [AC 自動化ツール群仕様](docs/ac_automation_tools_spec.md)
- [Black-box Word DP / Hankel 復元仕様](docs/word_dp_hankel_spec.md)
- [Non-word 愚直解高速化ツール群仕様](docs/nonword_automation_tools_spec.md)

## ツール自身のテスト

    python3 -m unittest discover -s tests/tooling -v
