#define PROBLEM "https://atcoder.jp/contests/abc284/tasks/abc284_h"
#include <bits/stdc++.h>
#include "graph/unlabeledcount.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, modulus;
    cin >> n >> k >> modulus;
    dynamicmodint<284>::set_mod(modulus);
    using mint = dynamicmodint<284>;
    cout << count_surjectively_colored_unlabeled_graphs<mint>(n, k) << '\n';
}
