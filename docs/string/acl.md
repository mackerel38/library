# String

個別文書: [suffixarray](suffixarray.md)、[lcp](lcp.md)、
[suffixquery](suffixquery.md)、[prefix](prefix.md)、[zalgorithm](zalgorithm.md)、
[manacher](manacher.md)、[substringorder](substringorder.md)。

## `suffix_array`

文字列版はSA-ISで`O(n)`。値域上限付き整数列版は`O(n+upper)`、一般列版は
座標圧縮込み`O(n log n)`。空列にも対応する。

## `lcp_array`

列とそのsuffix arrayから、辞書順で隣接するsuffix間のLCP長を`O(n)`で返す。
空列と要素数1では空配列を返す。

## `z_algorithm`

`z[i] = LCP(sequence, sequence[i..])`となる配列を`O(n)`で返す。
空列は空配列、非空列では`z[0] == n`。

## 検索と回文

- `prefix_function`: KMPのprefix functionを`O(n)`で返す。
- `find_all`: patternの全出現位置を`O(n+m)`で返す。空patternは`0..n`。
- `manacher_odd`、`manacher_even`: 奇数長・偶数長回文半径を`O(n)`で返す。

## `suffixquery`

文字列からsuffix array、rank、LCPのRMQを構築する。構築`O(n log n)`、
suffix間LCPと部分文字列比較は`O(1)`、pattern検索は`O(|pattern| log n)`。

## `substringorder`

一つまたは複数の文字列に現れる全部分文字列を、出現の重複を区別して辞書順に並べる。
合計長を`L`として構築`O(L)`、順位から出現位置を引くqueryは`O(log L)`。
