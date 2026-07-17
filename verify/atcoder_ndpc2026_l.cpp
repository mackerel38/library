#include <bits/stdc++.h>
#include "math/divisortransform.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

using mint = modint998244353;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> permutation(n);
    for (auto& x : permutation) cin >> x;
    vector<vector<int>> divisors(n + 1);
    for (int divisor = 1; divisor <= n; ++divisor) {
        for (int multiple = divisor; multiple <= n; multiple += divisor) {
            divisors[multiple].push_back(divisor);
        }
    }
    vector<mint> inverse_gcd(n + 1);
    for (int value = 1; value <= n; ++value) inverse_gcd[value] = mint(value).inv();
    divisor_mobius(inverse_gcd);

    vector<mint> divisible_sum(n + 1);
    vector<mint> paths(n);
    paths[0] = 1;
    for (int index = 0; index < n; ++index) {
        const int value = permutation[index];
        if (index > 0) {
            mint sum = 0;
            for (const int divisor : divisors[value]) {
                sum += inverse_gcd[divisor] * divisible_sum[divisor];
            }
            paths[index] = value * sum;
            cout << paths[index].val() << '\n';
        }
        const mint weighted = paths[index] * value;
        for (const int divisor : divisors[value]) divisible_sum[divisor] += weighted;
    }
}
