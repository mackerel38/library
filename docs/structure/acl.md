# Structure

個別文書: [dsu](dsu.md)、[fenwicktree](fenwicktree.md)、[segtree](segtree.md)、
[lazysegtree](lazysegtree.md)。

## `dsu`

素集合の併合、同一集合判定、代表元、集合サイズ、連結成分数、全グループ列挙を提供する。
`operator[]`は代表元を返す。併合・判定はならし`O(alpha(n))`、列挙は`O(n)`。

## `fenwicktree`

[専用文書](fenwicktree.md)を参照。一点加算・代入・要素取得・区間和と、非負列の
`lower_bound`・`upper_bound`を提供する。配列からの`O(n)`構築と`sum()`による全体和にも対応する。

## `segtree`

`segtree<S, op, e>`としてモノイドの一点代入と区間積を扱う。
`set/get/operator[]/prod/all_prod/max_right/min_left`を提供する。
`max_right`と`min_left`へ渡す述語は`g(e()) == true`で、副作用を持たないこと。

## `lazysegtree`

`lazysegtree<S, op, e, F, mapping, composition, id>`としてモノイド作用を扱う。
一点・区間`apply`、一点代入、区間積、二分探索を提供する。
`composition(newer, older)`は「olderの後にnewer」を適用する作用を返す。
