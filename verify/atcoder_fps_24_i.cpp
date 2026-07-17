#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_i"
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, k;
    cin >> n >> k;
    vector<fps998244353> factors(n);
    for (auto& factor : factors) {
        modint998244353 value;
        cin >> value;
        factor = {1, value};
    }
    cout << polynomial_product<998244353>(std::move(factors), k + 1)[k] << '\n';
}
