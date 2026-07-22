---
title: "オンライン畳み込みDP"
documentation_of: //cp/fps/online.hpp
---

# オンライン畳み込みDP


## 概要

`i`番目を左から確定するとき、過去の確定値からの寄与が差`i-j`だけで決まるDPを分割統治畳み込みで
計算する。`decide(i,raw)`には過去からの寄与をすべて加えた値が渡されるため、禁止位置なら0にする、
その場で正規化する、といった問題側の判断を挟める。

確定した係数`a[i]`から新しい一次因子`(1+a[i]x)`を作り、その因子が将来の係数へ影響するonline積も
`online_factor_product_coefficients`で扱える。

## 厳密な定義

- `relaxedconvolution`: relaxedconvolution conv(n); conv.append(a[i],b[i])で積のi次係数を返す。 全n回でO(M(n) log n)、各append時点ではそれ以前の係数だけを使う。
- `online_convolution`: O(M(n) log n)。online_convolution(source,kernel,decide): 左から確定する畳み込みDPを返す。 raw[i]=source[i]+sum_{j<i} committed[j]*kernel[i-j]を受け、decide(i,raw[i])が確定値を返す。
- `convolution_recurrence`: O(M(n) log n)。convolution_recurrence(source,kernel): f=source+f*kernel、kernel[0]=0を解く。
- `online_factor_product_coefficients`: $a_i=[x^i]B(x)\prod_{s\leq j<i}(1+a_jx)$で定まる係数列を返す。

`relaxedconvolution`は二列の係数がどちらも左から確定する場合に使う。
`append(a[i], b[i])`は、その時点で確定する積の`i`次係数を返すため、
自己畳み込みや相互に依存する二列のonline DPにも使える。

`online_factor_product_coefficients(base,start)`は、`i<start`では`base[i]`をそのまま返し、
`i>=start`では

$$
a_i=[x^i]B(x)\prod_{j=start}^{i-1}(1+a_jx),
\qquad B(x)=\sum_i\mathrm{base}_i x^i
$$

を満たす`a[i]`を返す。固定kernelとの畳み込みではなく、確定値が新しい選択可能要素を一つ追加する
集合選択型の再帰に使う。

## Include

```cpp
#include "fps/online.hpp"

auto dp = poe::online_convolution<998244353>(source, kernel,
    [&](int index, mint value) {
        return blocked[index] ? mint{0} : value;
    });

poe::relaxedconvolution<998244353> product(n);
mint coefficient = product.append(a[i], b[i]);

auto coefficients = poe::online_factor_product_coefficients<998244353>(
    base, first_dynamic_degree);
```

`kernel[0]`は使わず、`kernel[d]`が距離`d`の遷移係数である。未来の値や確定後に変わる遷移係数へは
使えない。`decide`は同じindexで一度だけ呼ばれる。

計算量は`M(n)`を畳み込み時間として、いずれも全体で`O(M(n) log n)`、
領域`O(n log n)`を目安とする。`relaxedconvolution`はNTT可能な法を使う。

<!-- BEGIN AUTO-GENERATED API REFERENCE -->
## APIリファレンス

この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。

### `relaxedconvolution`

```cpp
template<int mod> struct relaxedconvolution
```

relaxedconvolution conv(n); conv.append(a[i],b[i])で積のi次係数を返す。
全n回でO(M(n) log n)、各append時点ではそれ以前の係数だけを使う。

### `relaxedconvolution`

```cpp
explicit relaxedconvolution(int size) : size_(size), index_(0), left_(size), right_(size), product_(size)
```

O(n)。係数をsize個追加できる空のonline畳み込みを作る。

### `append`

```cpp
mint append(mint left_value, mint right_value)
```

全呼び出しでO(M(n) log n)。次の二係数を追加して積の同じ次数の係数を返す。

### `online_convolution`

```cpp
template<int mod, class Decide> std::vector<staticmodint<mod>> online_convolution( std::vector<staticmodint<mod>> source, const std::vector<staticmodint<mod>>& kernel, Decide decide )
```

O(M(n) log n)。online_convolution(source,kernel,decide): 左から確定する畳み込みDPを返す。
raw[i]=source[i]+sum_{j<i} committed[j]*kernel[i-j]を受け、decide(i,raw[i])が確定値を返す。

### `convolution_recurrence`

```cpp
template<int mod> std::vector<staticmodint<mod>> convolution_recurrence( std::vector<staticmodint<mod>> source, const std::vector<staticmodint<mod>>& kernel )
```

O(M(n) log n)。convolution_recurrence(source,kernel): f=source+f*kernel、kernel[0]=0を解く。

### `online_factor_product_coefficients`

```cpp
template<int mod> std::vector<staticmodint<mod>> online_factor_product_coefficients( std::vector<staticmodint<mod>> base, int start )
```

O(M(n) log n)。a[i]=[x^i]base(x)prod_{start<=j<i}(1+a[j]x)で定まる係数列を返す。

<!-- END AUTO-GENERATED API REFERENCE -->

## 実在問題での使用例

[FPS 24題 J - スゴロク](https://atcoder.jp/contests/fps-24/tasks/fps_24_j)の、禁止マスで到達確率を
0へ確定するDPに使える。愚直`O(n^2)`DPとのproperty testを行う。

[AtCoder ABC315 Ex - Typical Convolution Problem](https://atcoder.jp/contests/abc315/tasks/abc315_h)では、
`append(F[n], F[n])`で自己畳み込みの次係数を受け取り、そのprefix和から次の`F`を決める。
`verify/atcoder_abc315_h.cpp`で公式sampleを確認済み。

[ABC281 Ex - Alchemy](https://atcoder.jp/contests/abc281/tasks/abc281_h)では、
$B(x)=\sum_i\binom Ai x^i$、`start=2`として`online_factor_product_coefficients`を使う。
