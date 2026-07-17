#include <bits/stdc++.h>
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> distance(n - 1);
    for (auto& value : distance) cin >> value;
    map<int, modint998244353> dp;
    dp[n - 1] = 1;
    dp[n - 2] = 1;
    modint998244353 coefficient = 1;
    for (int i = n - 3; i >= 0; --i) {
        const int maximum_position = i + distance[i];
        const modint998244353 actual = dp[maximum_position] * coefficient;
        if (distance[i] == distance[i + 1]) {
            coefficient *= n - i - 2;
            const auto normalized = actual / coefficient;
            dp[maximum_position] += normalized;
            dp[i] += normalized;
        } else {
            dp.clear();
            coefficient = 1;
            dp[maximum_position] = actual;
            dp[i] = actual;
        }
    }
    modint998244353 answer = 0;
    for (const auto& [position, ways] : dp) {
        (void)position;
        answer += ways;
    }
    cout << answer * coefficient << '\n';
}
