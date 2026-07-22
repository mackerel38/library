#include <bits/stdc++.h>
#include "dp/distinctrepresentatives.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long limit;
    cin >> n >> limit;
    vector<long long> divisors(n);
    for (auto& divisor : divisors) cin >> divisor;
    cout << count_distinct_bounded_multiples<modint998244353>(limit, divisors) << '\n';
}
