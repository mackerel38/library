#include <bits/stdc++.h>
#include "dp/intervalmaximum.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, maximum, q;
    cin >> n >> maximum >> q;
    vector<intervalmaximumconstraint> constraints(q);
    for (auto& [left, right, value] : constraints) {
        cin >> left >> right >> value;
        --left;
    }
    cout << count_interval_maximum_sequences<998244353>(n, maximum, constraints) << '\n';
}
