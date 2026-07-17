#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "math/prime.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    vector<vector<int>> tests(test_count);
    int maximum = 1;
    for (auto& values : tests) {
        int n;
        cin >> n;
        values.resize(n);
        for (int& value : values) cin >> value, maximum = max(maximum, value);
    }

    primetable primes(maximum);
    vector<int> largest(maximum + 1), second(maximum + 1);
    for (const auto& values : tests) {
        vector<vector<pair<int, int>>> factors(values.size());
        vector<int> appeared;
        for (int i = 0; i < static_cast<int>(values.size()); ++i) {
            factors[i] = primes.factorize(values[i]);
            for (const auto [prime, exponent] : factors[i]) {
                if (largest[prime] == 0) appeared.push_back(prime);
                if (exponent > largest[prime]) {
                    second[prime] = largest[prime];
                    largest[prime] = exponent;
                } else if (exponent > second[prime]) {
                    second[prime] = exponent;
                }
            }
        }

        modint998244353 lcm = 1;
        for (int prime : appeared) lcm *= modint998244353(prime).pow(largest[prime]);
        for (int i = 0; i < static_cast<int>(values.size()); ++i) {
            modint998244353 answer = lcm;
            for (const auto [prime, exponent] : factors[i]) {
                if (exponent == largest[prime]) {
                    answer /= modint998244353(prime).pow(largest[prime] - second[prime]);
                }
            }
            if (i) cout << ' ';
            cout << answer.val();
        }
        cout << '\n';
        for (int prime : appeared) largest[prime] = second[prime] = 0;
    }
}
