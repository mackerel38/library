#include <bits/stdc++.h>
#include "math/modularproduct.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long k;
    unsigned long long n, m;
    cin >> k >> n >> m;
    cout << count_modular_product_sequences<998244353>(k, n, m) << '\n';
}
