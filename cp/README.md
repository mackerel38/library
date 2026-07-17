# cp

提出用のGNU++23 header-onlyライブラリをカテゴリ別に置きます。
この`cp`ディレクトリ自体をinclude rootとし、利用側とヘッダ内部では
`#include "graph/graph.hpp"`のように`cp/`を付けずに指定します。

通常のhppは物理1行目を`#pragma once`、2行目を`#include <bits/stdc++.h>`とし、
3行目以降にcp基準の短いパスで直接依存を並べます。集約専用の`all.hpp`は
`#pragma once`とlocal includeだけで構成し、重複するbits includeや空namespaceを置きません。
公開namespaceは`poe`です。
