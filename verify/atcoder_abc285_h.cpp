#define PROBLEM "https://atcoder.jp/contests/abc285/tasks/abc285_h"
#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/paritymatrix.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> exponents(k);
    for (int& exponent : exponents) cin >> exponent;
    using mint = staticmodint<1000000007>;
    cout << count_matrices_without_even_rows<mint>(n, exponents) << '\n';
}
