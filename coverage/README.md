# ABC過去問監査

`problems.tsv`は、ABCの各問題を単一ジャンルへ自動分類する表ではない。
解説と実装を人間が確認し、再利用できる部品と問題固有の核心を分けて記録する。

## 対象release

最初のreleaseは、2026-07-12をcutoffとして、その時点で終了済みの直近10回
`ABC457`〜`ABC466`のC〜G、計50問を対象にする。

各行の主な列は次の意味を持つ。

- `reusable_parts`: 解法で使う再利用可能なアルゴリズム・構造
- `recipe`: 複数問題へ持ち運べるがcoreより具体的な定型処理
- `problem_specific_core`: その問題固有の観察や式変形
- `tool`: stress、列生成など有効な開発補助
- `current`: 現在対応するheader・recipe・tool
- `missing`: 反復利用できるのに未実装の部品・API
- `pitfalls`: overflow、境界、順序など実装上の事故点
- `status`: `pending`または`reviewed`

`reviewed`にするには、公式問題・解説を確認し、`reusable_parts`、
`problem_specific_core`、`current`、`missing`、`pitfalls`、
`reviewer`、`reviewed_on`を埋める。該当なしは空欄にせず`none`と書く。

## 検査

```bash
python3 tools/coverage.py validate
python3 tools/coverage.py summary
```

`summary`は分母・確認済み数・未確認数・不足あり件数を表示する。
未確認行が残る限り「ABCを網羅した」とは扱わない。
