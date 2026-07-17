# Math

個別文書: [math](math.md)、[modint](modint.md)、[convolution](convolution.md)。

## 整数関数

- `pow_mod(x, n, mod)`: `x^n mod mod`、`O(log n)`。
- `inv_mod(x, mod)`: 逆元、`O(log mod)`。互いに素であること。
- `crt(r, m)`: 中国剰余。解なしは`{0, 0}`。
- `floor_sum(n, mod, a, b)`: floor sum、`O(log mod)`。負の`a,b`にも対応する。
  ACLと同じく`0<=n<2^32`、`1<=mod<2^32`で、溢れた答えは`mod 2^64`となる。

## Modint

`staticmodint<mod>`、`dynamicmodint<id>`と、`modint998244353`、
`modint1000000007`、`modint`を提供する。動的版は使用前に`set_mod`する。
除算・`inv`には値と法が互いに素という前提がある。`raw`は剰余を取らない。

## Convolution

`convolution`はNTT可能な素数modで`O(n log n)`、`convolution_ll`は3法CRTで
符号付き64bit整数畳み込みを行う。返る全係数が`long long`範囲内であること。
