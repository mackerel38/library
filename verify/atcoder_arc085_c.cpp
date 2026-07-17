#include <bits/stdc++.h>
#include "flow/submodular.hpp"

using namespace std;
using namespace poe;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> value(n);
    long long absolute_sum = 0;
    for (auto& x : value) {
        cin >> x;
        absolute_sum += abs(x);
    }
    const long long infinity = absolute_sum + 1;
    submodular<long long> objective(n);
    for (int i = 0; i < n; ++i) objective.add_unary(i, 0, -value[i]);
    for (int divisor = 1; divisor <= n; ++divisor) {
        for (int multiple = divisor * 2; multiple <= n; multiple += divisor) {
            objective.add_implication_cost(multiple - 1, divisor - 1, infinity);
        }
    }
    cout << -objective.minimize().cost << '\n';
}
