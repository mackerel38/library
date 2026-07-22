# Recipe選択ガイド

`recipe`は、強い適用条件まで含めて提出コードを短くする完成形です。`all.hpp`には含まれないため、
一致するものを明示的にincludeします。表の「使える条件」が問題と一致しない場合は使わないでください。

| header | 使える条件 | 求めるもの・操作 |
| --- | --- | --- |
| [`connectivityfrontier.hpp`](connectivityfrontier.md) | 幅が小さい盤面を順に走査できる | 連結性を状態とするフロンティアDP |
| [`prefixgrid.hpp`](prefixgrid.md) | 行ごとの使用部分がprefixになる | prefix形状の盤面DP |
| [`nearestactive.hpp`](nearestactive.md) | 木上で頂点を有効化し、最近点を尋ねる | 有効頂点までの最短距離 |
| [`treepathpartition.hpp`](treepathpartition.md) | 木を条件付きのpathへ分割する | path分割の可否・最適化 |
| [`translation.hpp`](translation.md) | 全点を同方向へ動かし、盤外で消し、禁止点を踏めない | 全点消去の最小操作数 |
| [`monotonegridshortestpath.hpp`](monotonegridshortestpath.md) | 高さが小さく、列を戻らない経路だけを扱う | 壁一点更新とgrid最短距離 |
| [`monochromatic.hpp`](monochromatic.md) | 固定木、二色、頂点色反転、加算可能な重み | 指定頂点の同色連結成分和 |
| [`setsegtree.hpp`](setsegtree.md) | 各位置が64要素以下の集合 | 区間一斉追加・削除と最大集合サイズ |
| [`thresholdsequence.hpp`](thresholdsequence.md) | 各写像が同じ`up`,`down`と一つの閾値を持ち、末尾追加だけ | 全写像を通した値 |
| [`stacksequence.hpp`](stacksequence.md) | 固定長のpush/pop操作列を一点更新する | 部分区間を実行したstack |
| [`gaussiannorm.hpp`](gaussiannorm.md) | $N$の素因数分解があり、法$C$ごとの二平方和が必要 | $x^2+y^2=N$の剰余分布 |
| [`insertioncomponents.hpp`](insertioncomponents.md) | 相異なる値を昇順挿入し、挿入済み隣接成分数で状態が決まる | 許可遷移を満たす順列数 |
| [`compassmove.hpp`](compassmove.md) | 障害物なし格子で、許可した8近傍方向を費用1で何度でも使う | 原点から目的地への最小手数 |

各ページには、返り値、失敗時の値、計算量、使えない条件を記載しています。
