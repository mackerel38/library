#include <bits/stdc++.h>

#include "fps/fps.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    long long m;
    cin >> n >> m;
    fps998244353 digit(min(n, 9) + 1, 1);
    auto coefficients = digit.pow(m - 1, n + 1);
    for (int degree = 1; degree <= n; ++degree) coefficients[degree] += coefficients[degree - 1];
    modint998244353 answer = 0;
    for (int sum = n % 9; sum <= n; sum += 9) answer += coefficients[sum];
    answer -= n / 9;
    cout << answer.val() << '\n';
}
