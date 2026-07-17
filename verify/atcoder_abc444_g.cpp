#include <bits/stdc++.h>
#include "math/gaussiannorm.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, modulus, factor_count;
    cin >> a >> b >> modulus >> factor_count;
    vector<pair<int, unsigned long long>> factors(factor_count);
    for (auto& [prime, exponent] : factors) cin >> prime >> exponent;
    const auto count = gaussian_norm_residue_distribution<modint998244353>(
        factors, modulus);
    const int x = (modulus - a) % modulus;
    const int y = (modulus - b) % modulus;
    cout << count[x * modulus + y].val() << '\n';
}
